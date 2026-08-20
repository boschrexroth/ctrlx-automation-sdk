/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

use ctrlx_datalayer::metadata_builder::{OPERATION_READ, OPERATION_WRITE, ReferenceType};
use ctrlx_datalayer::provider::node::{NodeEvent, ProviderNodeCallback};
use ctrlx_datalayer::{MetadataBuilder, Value};
use ctrlx_fbs::metadata_generated::comm::datalayer::NodeClass;

use crate::common::TYPE_ADDRESS_INT32;

/// A Data Layer provider node that holds a single `i32` value.
pub(crate) struct ProviderNodeInt32 {
    /// The Data Layer address this node is registered under.
    address: String,
    /// The current integer value stored in this node.
    value: i32,
}

impl ProviderNodeInt32 {
    /// Creates a new `ProviderNodeInt32` with the given address and initial value.
    pub(crate) fn new(address: String, initial_value: i32) -> Self {
        ProviderNodeInt32 { address, value: initial_value }
    }

    /// Returns the Data Layer address of this node.
    pub(crate) fn address(&self) -> &str {
        &self.address
    }
}

impl ProviderNodeCallback for ProviderNodeInt32 {
    /// Handles incoming Data Layer node events (Read, Write, Metadata).
    fn on_response(&mut self, event: NodeEvent) {
        match event {
            // Handle read requests: return the current i32 value if the address matches.
            NodeEvent::Read(addr, _, callback) => {
                if addr == self.address() {
                    let value = Value::new(self.value);
                    callback.callback(ctrlx_datalayer::Result::Ok, &value);
                } else {
                    // Address does not match — respond with an invalid address error.
                    callback.callback(
                        ctrlx_datalayer::Result::InvalidAddress,
                        &Value::new_default(),
                    );
                }
            }
            // Handle write requests: update the stored value if the address and type match.
            NodeEvent::Write(addr, input, callback) => {
                if addr == self.address() {
                    match input.as_ref::<i32>().copied() {
                        Some(v) => {
                            // Successfully parsed the new value — store it and confirm.
                            self.value = v;
                            callback.callback(ctrlx_datalayer::Result::Ok, &Value::new_default());
                        }
                        None => {
                            // Input could not be interpreted as i32 — respond with a type mismatch error.
                            callback.callback(
                                ctrlx_datalayer::Result::TypeMismatch,
                                &Value::new_default(),
                            );
                        }
                    }
                } else {
                    // Address does not match — respond with an invalid address error.
                    callback.callback(
                        ctrlx_datalayer::Result::InvalidAddress,
                        &Value::new_default(),
                    );
                }
            }
            // Handle metadata requests: return node class, allowed operations, and type references.
            NodeEvent::Metadata(addr, callback) => {
                if addr == self.address() {
                    let metadata = MetadataBuilder::new("int32".to_string(), "int32_url".to_string())
                        .with_node_class(NodeClass::Variable)
                        .with_operations_mask(OPERATION_READ | OPERATION_WRITE)
                        .add_reference(ReferenceType::Read, TYPE_ADDRESS_INT32.to_string())
                        .add_reference(ReferenceType::Write, TYPE_ADDRESS_INT32.to_string())
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
