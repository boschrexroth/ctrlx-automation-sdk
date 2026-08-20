/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

use ctrlx_datalayer::metadata_builder::{OPERATION_READ, ReferenceType};
use ctrlx_datalayer::provider::node::{NodeEvent, ProviderNodeCallback};
use ctrlx_datalayer::{MetadataBuilder, Value};
use ctrlx_fbs::metadata_generated::comm::datalayer::NodeClass;

use crate::sampleSchema_generated::pkg::sample::schema::InertialValueT;

/// A Data Layer provider node that returns a FlatBuffers-encoded `InertialValue` on every read.
pub(crate) struct ProviderNodeFbs {
    /// The Data Layer address this node is registered under.
    address: String,
}

impl ProviderNodeFbs {
    /// Creates a new `ProviderNodeFbs` with the given address.
    pub(crate) fn new(address: String) -> Self {
        ProviderNodeFbs { address }
    }

    /// Returns the Data Layer address of this node.
    pub(crate) fn address(&self) -> &str {
        &self.address
    }
}

impl ProviderNodeCallback for ProviderNodeFbs {
    /// Handles incoming Data Layer node events (Read, Metadata).
    fn on_response(&mut self, event: NodeEvent) {
        match event {
            // Handle read requests: build and return a FlatBuffers-encoded InertialValue.
            NodeEvent::Read(addr, _, callback) => {
                if addr == self.address() {
                    // Populate InertialValue with fixed sample data (x=1, y=2, z=3).
                    let mut fbs = InertialValueT::default();
                    fbs.x = 1;
                    fbs.y = 2;
                    fbs.z = 3;

                    // Serialize the InertialValue into a FlatBuffers byte buffer.
                    let mut fbb = flatbuffers::FlatBufferBuilder::new();
                    let offset = fbs.pack(&mut fbb);
                    fbb.finish(offset, None);
                    let data = fbb.finished_data().to_vec();

                    // Wrap the raw bytes in a Data Layer Value and respond.
                    let mut value = Value::new_default();
                    value.flatbuffers_mut(data);
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
                    let metadata = MetadataBuilder::new("fbs".to_string(), "fbs_url".to_string())
                        .with_node_class(NodeClass::Variable)
                        .with_operations_mask(OPERATION_READ)
                        .add_reference(ReferenceType::Read, crate::common::TYPE_ADDRESS_FBS.to_string())
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
