/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

/// Macro that expands to the common root path prefix for all Data Layer nodes in this sample.
macro_rules! address_root {
    () => {
        "sdk/rust/provider/simple/"
    };
}

// pub(crate) const ADDRESS_ROOT: &str = address_root!();

/// Data Layer address of the int32 provider node.
pub(crate) const ADDRESS_INT32: &str = concat!(address_root!(), "int32");
/// Data Layer address of the string provider node.
pub(crate) const ADDRESS_STRING: &str = concat!(address_root!(), "string");
/// Data Layer address of the timestamp provider node.
pub(crate) const ADDRESS_TIMESTAMP: &str = concat!(address_root!(), "timestamp");
/// Data Layer address of the FlatBuffers inertial-value provider node.
pub(crate) const ADDRESS_FBS: &str = concat!(address_root!(), "inertial-value");

/// Type reference address for the built-in int32 Data Layer type.
pub(crate) const TYPE_ADDRESS_INT32: &str = "types/datalayer/int32";
/// Type reference address for the built-in string Data Layer type.
pub(crate) const TYPE_ADDRESS_STRING: &str = "types/datalayer/string";
/// Type reference address for the built-in timestamp Data Layer type.
pub(crate) const TYPE_ADDRESS_TIMESTAMP: &str = "types/datalayer/timestamp";
/// Type reference address for the custom FlatBuffers inertial-value Data Layer type.
pub(crate) const TYPE_ADDRESS_FBS: &str = concat!("types/", address_root!(), "inertial-value");