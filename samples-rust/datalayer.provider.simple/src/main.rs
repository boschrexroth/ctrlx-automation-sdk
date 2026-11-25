/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use log::{error, info};
use ctrlx_datalayer::System;
use ctrlx_datalayer::factory::Connection;
use ctrlx_datalayer::provider::Provider;
use ctrlx_datalayer::provider::node::ProviderNode;

use crate::common::{
    ADDRESS_FBS, ADDRESS_INT32, ADDRESS_STRING, ADDRESS_TIMESTAMP, TYPE_ADDRESS_FBS,
};

mod provider_node_int32;
mod provider_node_str;
mod provider_node_timestamp;
mod provider_node_fbs;
mod common;

#[allow(warnings)]
mod sampleSchema_generated;

fn main() {

    env_logger::init();

    // Resolve the path to the FlatBuffers binary schema file used for type registration.
    let bfbs_path = bfbs_path();

    // Shared flag that is set to true when a termination signal is received.
    let shutdown = Arc::new(AtomicBool::new(false));
    // Register OS signals so the main loop exits cleanly on SIGINT / SIGTERM / SIGABRT.
    signal_hook::flag::register(signal_hook::consts::SIGINT, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGTERM, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGABRT, Arc::clone(&shutdown)).unwrap();

    // Create the ctrlX Data Layer system and start it in non-blocking mode.
    let system = System::new("");
    system.start(false);
    {
        // Create a provider connected via the virtual IP interface.
        let provider = system.factory().create_provider(Connection::IpVirtual);

        // Register the int32 node and configure its response timeout.
        let node_int32 = create_node_int32();
        register_node(&provider, ADDRESS_INT32, &node_int32);

        // Register the string node and configure its response timeout.
        let node_str = create_node_str();
        register_node(&provider, ADDRESS_STRING, &node_str);

        // Register the timestamp node and configure its response timeout.
        let node_timestamp = create_node_timestamp();
        register_node(&provider, ADDRESS_TIMESTAMP, &node_timestamp);

        // Register the custom FlatBuffers type with the Data Layer.
        let result = provider.register_type(TYPE_ADDRESS_FBS, bfbs_path.as_os_str().to_str().unwrap());
        if let Err(e) = result {
            error!("Failed to register type: {:?}", e);
            return;
        }

        // Register the FlatBuffers inertial-value node and configure its response timeout.
        let node_fbs = create_node_fbs();
        register_node(&provider, ADDRESS_FBS, &node_fbs);

        // Start the provider, making all registered nodes visible in the Data Layer.
        let result = provider.start();
        if let Err(e) = result {
            error!("Failed to start provider: {:?}", e);
            return;
        }   
        
        while !shutdown.load(Ordering::Relaxed) && provider.is_connected() {
            // park_timeout returns immediately on EINTR (unlike thread::sleep which
            // retries with the original absolute deadline). When SIGINT or SIGTERM
            // arrives, the signal handler sets `shutdown` to true and EINTR wakes
            // this thread, so the loop exits without waiting the full tick.
            std::thread::park_timeout(std::time::Duration::from_secs(1));
        }

        info!("Shutting down provider...");
        
        // Unregister all nodes and stop the provider before the system is torn down.
        provider.unregister_node(ADDRESS_FBS).expect("Failed to unregister node");
        provider.unregister_node(ADDRESS_STRING).expect("Failed to unregister node");
        provider.unregister_node(ADDRESS_INT32).expect("Failed to unregister node");
        provider.unregister_node(ADDRESS_TIMESTAMP).expect("Failed to unregister node");
        provider.stop().expect("Failed to stop provider");
    }
}

/// Registers a node with the provider and sets the default response timeout of 2 s.
fn register_node(provider: &Provider, address: &str, node: &ProviderNode) {
    provider
        .register_node(address, node)
        .expect("Failed to register node");
    provider
        .update_timeout_node(node, 2000)
        .expect("Failed to set timeout for node");
}

/// Creates and wraps the FlatBuffers inertial-value provider node.
fn create_node_fbs() -> ProviderNode {
    ProviderNode::new(Box::new(
        provider_node_fbs::ProviderNodeFbs::new(ADDRESS_FBS.to_string()),
    ), false)
}

/// Returns the path to the FlatBuffers binary schema file.
/// When running as a snap package, the path is relative to `$SNAP`; otherwise
/// it is relative to the current working directory.
fn bfbs_path() -> std::path::PathBuf {
    let bfbs_path = if let Some(snap) = std::env::var("SNAP").ok().filter(|s| !s.is_empty()) {
        std::path::PathBuf::from(snap)
    } else {
        std::env::current_dir().expect("Failed to get current working directory")
    }
    .join("bfbs")
    .join("sampleSchema.bfbs");
    info!("bfbs path: {}", bfbs_path.display());
    bfbs_path
}

/// Creates and wraps the timestamp provider node.
fn create_node_timestamp() -> ProviderNode {
    ProviderNode::new(Box::new(
        provider_node_timestamp::ProviderNodeTimestamp::new(ADDRESS_TIMESTAMP.to_string()),
    ), false)
}

/// Creates and wraps the string provider node.
fn create_node_str() -> ProviderNode {
    ProviderNode::new(Box::new(
        provider_node_str::ProviderNodeStr::new(ADDRESS_STRING.to_string()),
    ), false)
}

/// Creates and wraps the int32 provider node with an initial value of 42.
fn create_node_int32() -> ProviderNode {
    ProviderNode::new(Box::new(
        provider_node_int32::ProviderNodeInt32::new(ADDRESS_INT32.to_string(), 42),
    ), false)
}
