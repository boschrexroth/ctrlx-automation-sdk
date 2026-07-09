/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
use log::{error, info};
use std::sync::Arc;
use std::sync::mpsc;

use ctrlx_datalayer::{
    BulkReadArg, BulkResponse, System, client::bulk::ResponseCallback, factory::Connection,
};
use ctrlx_fbs::metadata_generated::comm::datalayer::MetadataT;
use std::sync::atomic::{AtomicBool, Ordering};

/// Entry point of the application.
/// Initializes the logger, sets up signal handlers, creates the ctrlX Data Layer system,
/// and runs the bulk operation loop until a shutdown signal is received.
fn main() {
    // Initialize the environment logger (respects the RUST_LOG environment variable).
    env_logger::init();

    // Shared flag that is set to true when a termination signal is received.
    let shutdown = Arc::new(AtomicBool::new(false));
    // Register OS signals so the main loop exits cleanly on SIGINT / SIGTERM / SIGABRT.
    signal_hook::flag::register(signal_hook::consts::SIGINT, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGTERM, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGABRT, Arc::clone(&shutdown)).unwrap();

    // Create and start the ctrlX Data Layer system (empty string uses the default address).
    let system = System::new("");
    system.start(false);
    {
        // Create a client that connects via the virtual IP address.
        let client = system.factory().create_client(Connection::IpVirtual);

        // Keep running bulk operations as long as the client is connected and no shutdown was requested.
        while client.is_connected() && !shutdown.load(Ordering::Relaxed) {
            info!("Connected to ctrlX Data Layer");
            run(&client);
            // Wait 10 seconds before the next iteration to avoid busy-looping.
            std::thread::park_timeout(std::time::Duration::from_secs(10));
        }
    }
    info!("Shutdown!");
}

/// Executes all bulk operations (read, metadata, browse) using a single synchronous bulk handle.
///
/// # Arguments
/// * `client` - Reference to the connected ctrlX Data Layer client.
fn run(client: &ctrlx_datalayer::Client) {
    // Create a synchronous bulk handle for batching multiple Data Layer requests.
    run_sync(client);
    run_async(client);
}

/// Executes all asynchronous bulk operations (read, metadata, browse).
/// Each operation uses a channel-based callback to receive the response.
///
/// # Arguments
/// * `client` - Reference to the connected ctrlX Data Layer client.
fn run_async(client: &ctrlx_datalayer::Client) {
    let mut bulk = client.create_async_bulk();
    async_read(&mut bulk);
    async_metadata(&mut bulk);
    async_browse(&mut bulk);
}

/// Executes all synchronous bulk operations (read, metadata, browse).
/// Results are returned directly from the call without a callback.
///
/// # Arguments
/// * `client` - Reference to the connected ctrlX Data Layer client.
fn run_sync(client: &ctrlx_datalayer::Client) {
    let bulk = client.create_sync_bulk();
    bulk_read(&bulk);
    bulk_metadata(&bulk);
    bulk_browse(&bulk);
}

/// Performs a bulk browse operation on multiple Data Layer nodes.
/// Retrieves the list of child nodes for each specified address and logs the results.
///
/// # Arguments
/// * `bulk` - Reference to the synchronous bulk handle.
fn bulk_browse(bulk: &ctrlx_datalayer::SyncBulk) {
    // Addresses of the Data Layer nodes whose children should be listed.
    let args = vec!["framework/metrics/system", "framework/metrics/process"];
    let result = bulk.browse(&args);
    if result.is_err() {
        error!("Bulk browse failed: {}", result.err().unwrap());
        return;
    }
    let responses = result.unwrap();
    // Iterate over each response and log the child node names.
    for response in responses {
        if response.result != ctrlx_datalayer::Result::Ok {
            error!(
                "Failed to browse address {}: {}",
                response.address, response.result
            );
            return;
        }
        info!(
            "Address: {}, nodes: {:?}",
            response.address,
            response.value.as_ref::<Vec<String>>().unwrap()
        );
    }
}

/// Performs a bulk metadata retrieval for multiple Data Layer nodes.
/// Fetches and logs the description field from the metadata of each specified address.
///
/// # Arguments
/// * `bulk` - Reference to the synchronous bulk handle.
fn bulk_metadata(bulk: &ctrlx_datalayer::SyncBulk) {
    // Addresses whose metadata (e.g., description, unit) should be retrieved.
    let args = vec![
        "framework/metrics/system/cpu-utilisation-percent",
        "framework/metrics/system/memavailable-mb",
    ];
    let result = bulk.metadata(&args);
    if result.is_err() {
        error!("Bulk metadata retrieval failed: {}", result.err().unwrap());
        return;
    }
    let responses = result.unwrap();
    // Iterate over each response, deserialize the FlatBuffers metadata, and log the description.
    for response in responses {
        if response.result != ctrlx_datalayer::Result::Ok {
            error!(
                "Failed to retrieve metadata for address {}: {}",
                response.address, response.result
            );
            return;
        }
        let metadata = MetadataT::try_from(&response.value);
        info!(
            "Address: {}, Metadata: {:?}",
            response.address,
            metadata.unwrap().description
        );
    }
}

/// Performs a bulk read operation on multiple Data Layer nodes.
/// Reads the current value of each specified address and logs it as an f64.
///
/// # Arguments
/// * `bulk` - Reference to the synchronous bulk handle.
fn bulk_read(bulk: &ctrlx_datalayer::SyncBulk) {
    // Build the list of read arguments; `argument` is None because no additional input is required.
    let args = vec![
        BulkReadArg {
            address: "framework/metrics/system/cpu-utilisation-percent".to_string(),
            argument: None,
        },
        BulkReadArg {
            address: "framework/metrics/system/memavailable-mb".to_string(),
            argument: None,
        },
    ];
    let result = bulk.read(&args);
    if result.is_err() {
        error!("Bulk read failed: {}", result.err().unwrap());
        return;
    }
    let responses = result.unwrap();
    // Iterate over each response and log the node's current value.
    for response in responses {
        if response.result != ctrlx_datalayer::Result::Ok {
            error!(
                "Failed to read address {}: {}",
                response.address, response.result
            );
            return;
        }
        info!(
            "Address: {}, Value: {}",
            response.address,
            response.value.as_ref::<f64>().unwrap()
        );
    }
}

/// A [`ResponseCallback`] implementation that forwards bulk responses through a synchronous channel.
/// Allows async bulk results to be received in a blocking fashion via [`mpsc::Receiver`].
struct ChannelCallback {
    sender: mpsc::SyncSender<Vec<BulkResponse>>,
}

impl ResponseCallback for ChannelCallback {
    fn on_response(&mut self, response: Vec<BulkResponse>) {
        self.sender.send(response).unwrap();
    }
}

/// Creates a [`ChannelCallback`] together with the corresponding [`mpsc::Receiver`].
/// The caller passes the boxed callback to an async bulk operation and waits on the receiver
/// for the result.
///
/// # Returns
/// A tuple of `(callback, receiver)` where the callback is forwarded to the bulk API
/// and the receiver is used to collect the responses.
fn make_callback() -> (Box<ChannelCallback>, mpsc::Receiver<Vec<BulkResponse>>) {
    let (tx, rx) = mpsc::sync_channel(1);
    (Box::new(ChannelCallback { sender: tx }), rx)
}

/// Performs an asynchronous bulk read on multiple Data Layer nodes.
/// The response is received via a channel callback and each value is logged as an f64.
///
/// # Arguments
/// * `bulk` - Mutable reference to the asynchronous bulk handle.
fn async_read(bulk: &mut ctrlx_datalayer::AsyncBulk) {
    // Build the list of read arguments; `argument` is None because no additional input is required.
    let args = vec![
        BulkReadArg {
            address: "framework/metrics/system/cpu-utilisation-percent".to_string(),
            argument: None,
        },
        BulkReadArg {
            address: "framework/metrics/system/memavailable-mb".to_string(),
            argument: None,
        },
    ];

    let (callback, rx) = make_callback();
    let result = bulk.read(&args, callback);

    if result.is_err() {
        error!("Bulk read failed: {}", result.err().unwrap());
        return;
    }

    let responses = rx
        .recv_timeout(std::time::Duration::from_secs(5))
        .expect("Timeout: No callback received within 5 seconds");
    // Iterate over each response and log the node's current value.
    for response in responses {
        if response.result != ctrlx_datalayer::Result::Ok {
            error!(
                "Failed to read address {}: {}",
                response.address, response.result
            );
            return;
        }
        info!(
            "Address: {}, Value: {}",
            response.address,
            response.value.as_ref::<f64>().unwrap()
        );
    }
}

/// Performs an asynchronous bulk browse on multiple Data Layer nodes.
/// The response is received via a channel callback and the child node names are logged.
///
/// # Arguments
/// * `bulk` - Mutable reference to the asynchronous bulk handle.
fn async_browse(bulk: &mut ctrlx_datalayer::AsyncBulk) {
    // Addresses of the Data Layer nodes whose children should be listed.
    let args = vec!["framework/metrics/system", "framework/metrics/process"];

    let (callback, rx) = make_callback();
    let result = bulk.browse(&args, callback);

    if result.is_err() {
        error!("Bulk browse failed: {}", result.err().unwrap());
        return;
    }

    let responses = rx
        .recv_timeout(std::time::Duration::from_secs(5))
        .expect("Timeout: No callback received within 5 seconds");
    // Iterate over each response and log the child node names.
    for response in responses {
        if response.result != ctrlx_datalayer::Result::Ok {
            error!(
                "Failed to browse address {}: {}",
                response.address, response.result
            );
            return;
        }
        info!(
            "Address: {}, nodes: {:?}",
            response.address,
            response.value.as_ref::<Vec<String>>().unwrap()
        );
    }
}

/// Performs an asynchronous bulk metadata retrieval for multiple Data Layer nodes.
/// The response is received via a channel callback and the description field is logged.
///
/// # Arguments
/// * `bulk` - Mutable reference to the asynchronous bulk handle.
fn async_metadata(bulk: &mut ctrlx_datalayer::AsyncBulk) {
    // Addresses whose metadata (e.g., description, unit) should be retrieved.
    let args = vec![
        "framework/metrics/system/cpu-utilisation-percent",
        "framework/metrics/system/memavailable-mb",
    ];

    let (callback, rx) = make_callback();
    let result = bulk.metadata(&args, callback);

    if result.is_err() {
        error!("Bulk metadata retrieval failed: {}", result.err().unwrap());
        return;
    }

    let responses = rx
        .recv_timeout(std::time::Duration::from_secs(5))
        .expect("Timeout: No callback received within 5 seconds");
    // Iterate over each response, deserialize the FlatBuffers metadata, and log the description.
    for response in responses {
        if response.result != ctrlx_datalayer::Result::Ok {
            error!(
                "Failed to retrieve metadata for address {}: {}",
                response.address, response.result
            );
            return;
        }
        let metadata = MetadataT::try_from(&response.value);
        info!(
            "Address: {}, Metadata: {:?}",
            response.address,
            metadata.unwrap().description
        );
    }
}
