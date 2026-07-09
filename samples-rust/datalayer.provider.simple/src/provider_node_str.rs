/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

use ctrlx_datalayer::metadata_builder::{OPERATION_READ, ReferenceType};
use ctrlx_datalayer::provider::node::{NodeEvent, ProviderNodeCallback};
use ctrlx_datalayer::{MetadataBuilder, Value};
use ctrlx_fbs::metadata_generated::comm::datalayer::NodeClass;

/// A Data Layer provider node that cycles through a list of static strings on every read.
pub(crate) struct ProviderNodeStr {
    /// The Data Layer address this node is registered under.
    address: String,
    /// The pool of string values returned in round-robin order.
    values: Vec<&'static str>,
    /// Index into `values` pointing to the next value to return.
    index: usize,
}

impl ProviderNodeStr {
    /// Creates a new `ProviderNodeStr` with the given address.
    /// The node cycles through ["ctrlX", "Software", "Development", "Kit"] on successive reads.
    pub(crate) fn new(address: String) -> Self {
        ProviderNodeStr {
            address,
            values: vec!["ctrlX", "Software", "Development", "Kit"],
            index: 0,
        }
    }

    /// Returns the Data Layer address of this node.
    pub(crate) fn address(&self) -> &str {
        &self.address
    }
}

impl ProviderNodeCallback for ProviderNodeStr {
    /// Handles incoming Data Layer node events (Read, Metadata).
    fn on_response(&mut self, event: NodeEvent) {
        match event {
            // Handle read requests: return the next string in the cycle.
            NodeEvent::Read(addr, _, callback) => {
                if addr == self.address() {
                    let current = self.values[self.index];
                    self.index = (self.index + 1) % self.values.len();
                    let value = Value::new(current.to_string());
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
                    let metadata = MetadataBuilder::new("str".to_string(), "str_url".to_string())
                        .with_node_class(NodeClass::Variable)
                        .with_operations_mask(OPERATION_READ)
                        .add_reference(ReferenceType::Read, crate::common::TYPE_ADDRESS_STRING.to_string())
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
