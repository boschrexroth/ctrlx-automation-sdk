/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
use ctrlx_datalayer::metadata_builder::{OPERATION_READ, OPERATION_WRITE};
use ctrlx_datalayer::provider::node::{NodeEvent, ProviderNodeCallback};
use ctrlx_datalayer::{MetadataBuilder, Value};
use ctrlx_fbs::metadata_generated::comm::datalayer::NodeClass;
use log::info;
use std::collections::HashMap;

use crate::ClientPublisher;

/// Callback implementation for a ctrlX Data Layer folder node.
///
/// Handles all node events (create, remove, read, write, metadata, browse, subscribe/unsubscribe) and
/// manages a [`ClientPublisher`] per active subscription.
pub struct ProviderNodeFolderCallback {
    /// Active subscriptions keyed by their unique subscription ID.
    sub_nodes: HashMap<u64, ClientPublisher>,
    /// Incrementing counter returned on every Read request.
    index: i32,
    /// Shared mutex passed to every [`ClientPublisher`] so that publish calls
    /// and unsubscribe teardown are mutually exclusive.
    mutex: std::sync::Arc<std::sync::Mutex<()>>,
}

impl ProviderNodeFolderCallback {
    /// Creates a new `ProviderNodeFolderCallback` with an empty subscription map.
    pub fn new() -> Self {
        Self {
            sub_nodes: HashMap::new(),
            index: 4567,
            mutex: std::sync::Arc::new(std::sync::Mutex::new(())),
        }
    }
}

impl ProviderNodeCallback for ProviderNodeFolderCallback {
    /// Dispatches incoming node events to the appropriate handler branch.
    fn on_response(&mut self, event: NodeEvent) {
        match event {
            // A new child node is being created under this folder — echo the value back.
            NodeEvent::Create(addr, val, callback) => {
                info!("Create event received for address: {}", addr);
                callback.callback(ctrlx_datalayer::Result::Ok, &val);
            }
            // A child node is being removed — acknowledge with an empty value.
            NodeEvent::Remove(addr, callback) => {
                info!("Remove event received for address: {}", addr);
                callback.callback(ctrlx_datalayer::Result::Ok, &Value::new_default());
            }
            // A client reads the node — increment the counter and return it.
            NodeEvent::Read(addr, _val, callback) => {
                info!("Read event received for address: {}", addr);
                self.index = self.index.wrapping_add(1);
                let value = Value::new(self.index);
                callback.callback(ctrlx_datalayer::Result::Ok, &value);
            }
            // A client writes to the node — store the new value and echo it back.
            NodeEvent::Write(addr, input, callback) => {
                info!("Write event received for address: {}", addr);
                self.index = *input.as_ref::<i32>().unwrap_or(&0);
                callback.callback(ctrlx_datalayer::Result::Ok, &input);
            }
            // A client requests the node's metadata — build and return it.
            NodeEvent::Metadata(addr, callback) => {
                info!("Metadata event received for address: {}", addr);
                let metadata =
                    MetadataBuilder::new("sdk_folder".to_string(), "sdk_folder_url".to_string())
                        .with_node_class(NodeClass::Folder)
                        .with_operations_mask(OPERATION_READ | OPERATION_WRITE)
                        .build();

                callback.callback(ctrlx_datalayer::Result::Ok, &metadata);
            }
            // A client browses the folder — return an empty child list for now.
            NodeEvent::Browse(addr, callback) => {
                info!("Browse event received for address: {}", addr);
                let value = Value::new(vec!["".to_string()]);
                callback.callback(ctrlx_datalayer::Result::Ok, &value);
            }
            // A client opens a new subscription — create and start a ClientPublisher for it.
            NodeEvent::Subscribe(subscription) => {
                info!(
                    "Subscribe event received for address: {} id: {}",
                    subscription.address(),
                    subscription.unique_id()
                );

                let len = subscription.nodes().len();
                let unique_id = subscription.unique_id();

                // Guard against duplicate subscriptions with the same ID.
                if self.sub_nodes.contains_key(&unique_id) {
                    info!("Subscription {} already exists", unique_id);
                    return;
                }

                // Create the publisher, hand it the shared mutex, then start the
                // background publish thread.
                let mut client_publisher = ClientPublisher::new(subscription, self.mutex.clone());
                client_publisher.start();
                self.sub_nodes.insert(unique_id, client_publisher);

                info!("Published subscription {} with nodes: {}", unique_id, len);
            }
            // A client closes a subscription — stop and remove the matching publisher.
            NodeEvent::Unsubscribe(subscription) => {
                info!(
                    ">Unsubscribe event received for address: {} id: {} nodes: {}",
                    subscription.address(),
                    subscription.unique_id(),
                    subscription.nodes().len()
                );

                // Hold the mutex only during remove() so that no in-flight tick()
                // races against the map removal. The mutex is released before stop()
                // is called to avoid a deadlock: the background thread acquires the
                // same mutex inside its publish loop, and stop() waits for that
                // thread via join().
                let maybe_publisher = {
                    let _guard = self.mutex.lock().unwrap();
                    if subscription.nodes().is_empty() {
                        info!(
                            "Removing subscription {} as no nodes are subscribed anymore",
                            subscription.unique_id()
                        );
                        self.sub_nodes.remove(&subscription.unique_id())
                    } else {
                        None
                    }
                }; // mutex released here
                if let Some(mut client_publisher) = maybe_publisher {
                    client_publisher.stop();
                }
                info!(
                    "<Unsubscribe event received for address: {} id: {} nodes: {}",
                    subscription.address(),
                    subscription.unique_id(),
                    subscription.nodes().len()
                );                
            }
        }
    }
}
