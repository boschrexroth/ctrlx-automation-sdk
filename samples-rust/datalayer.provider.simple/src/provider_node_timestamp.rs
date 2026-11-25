/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

use chrono::{DateTime, Utc};
use ctrlx_datalayer::metadata_builder::{OPERATION_READ, ReferenceType};
use ctrlx_datalayer::provider::node::{NodeEvent, ProviderNodeCallback};
use ctrlx_datalayer::{MetadataBuilder, Value};
use ctrlx_fbs::metadata_generated::comm::datalayer::NodeClass;

/// A Data Layer provider node that returns the current UTC timestamp on every read.
pub(crate) struct ProviderNodeTimestamp {
    /// The Data Layer address this node is registered under.
    address: String,
}

impl ProviderNodeTimestamp {
    /// Creates a new `ProviderNodeTimestamp` with the given address.
    pub(crate) fn new(address: String) -> Self {
        ProviderNodeTimestamp { address }
    }

    /// Returns the Data Layer address of this node.
    pub(crate) fn address(&self) -> &str {
        &self.address
    }
}

impl ProviderNodeCallback for ProviderNodeTimestamp {
    /// Handles incoming Data Layer node events (Read, Metadata).
    fn on_response(&mut self, event: NodeEvent) {
        match event {
            // Handle read requests: return the current UTC timestamp.
            NodeEvent::Read(addr, _, callback) => {
                if addr == self.address() {
                    let utc_now: DateTime<Utc> = Utc::now();
                    let value = Value::new(utc_now);
                    callback.callback(ctrlx_datalayer::Result::Ok, &value);
                } else {
                    // Address does not match — respond with an invalid address error.
                    callback.callback(
                        ctrlx_datalayer::Result::InvalidAddress,
                        &Value::new_default(),
                    );
                }
            }
            // Handle metadata requests: return node class, allowed operations, and type reference.
            NodeEvent::Metadata(addr, callback) => {
                if addr == self.address() {
                    let metadata = MetadataBuilder::new("timestamp".to_string(), "timestamp_url".to_string())
                        .with_node_class(NodeClass::Variable)
                        .with_operations_mask(OPERATION_READ)
                        .add_reference(ReferenceType::Read, crate::common::TYPE_ADDRESS_TIMESTAMP.to_string())
                        .build();
                    callback.callback(ctrlx_datalayer::Result::Ok, &metadata);
                } else {
                    // Address does not match — respond with an invalid address error.
                    callback.callback(
                        ctrlx_datalayer::Result::InvalidAddress,
                        &Value::new_default(),
                    );
                }
            }
            // Ignore all other event variants.
            _ => {}
        }
    }
}
