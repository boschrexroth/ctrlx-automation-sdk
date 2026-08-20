/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

//! Library crate for the ctrlX Data Layer provider subscription sample.
//!
//! Exposes the folder-node callback, the client publisher, and the shared
//! Data Layer address constants used by both the provider and the client binary.

/// Folder-node callback that handles all Data Layer provider node events.
pub mod provider_node_folder_callback;
/// Periodic publisher that pushes updated values to active client subscriptions.
pub mod client_publisher;

pub use provider_node_folder_callback::ProviderNodeFolderCallback;
pub use client_publisher::ClientPublisher;

/// Base address of the sample provider folder in the Data Layer tree.
pub const ADDRESS_FOLDER: &str = "sdk/rust/provider/sub/";
/// Wildcard address matching all nodes beneath the sample folder.
pub const ADDRESS_FOLDER_WILDCARD: &str = "sdk/rust/provider/sub/**";
/// Address of the first integer node published by the provider.
pub const ADDRESS_INT1: &str = "sdk/rust/provider/sub/int1";
/// Address of the second integer node published by the provider.
pub const ADDRESS_INT2: &str = "sdk/rust/provider/sub/int2";