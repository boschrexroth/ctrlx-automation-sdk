/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
use log::{error, info};
use std::sync::Arc;
use std::sync::mpsc;
use std::sync::atomic::{AtomicBool, Ordering};
use ctrlx_datalayer::{system::System, factory::Connection, client::Client, client::r#async::ResponseCallback, variant::Value};
/// Performs synchronous ping, browse, and read operations on the Data Layer.
fn run_sync(client: &Client) {
    let result = client.sync().ping();
    if !result.is_ok() {
        error!("Ping failed: {}", result.err().unwrap());
        return;
    }

    let result = client.sync().browse("");
    if !result.is_ok() {
        error!("Browse failed: {}", result.err().unwrap());
        return;
    }

    let result = client.sync().read("scheduler/admin/info/counter");
    if !result.is_ok() {
        error!("Read failed: {}", result.err().unwrap());
        return;
    }
    let value = result.unwrap();
    info!("Read value: {}", value.as_ref::<u64>().unwrap());
}

struct ChannelCallback {
    sender: mpsc::SyncSender<(ctrlx_datalayer::Result, Value)>,
}

impl ResponseCallback for ChannelCallback {
    fn on_response(&mut self, res: ctrlx_datalayer::Result, data: Value) {
        let _ = self.sender.send((res, data));
    }
}

// Helper function: Creates callback + receiver end
fn make_callback() -> (Box<ChannelCallback>, mpsc::Receiver<(ctrlx_datalayer::Result, Value)>) {
    let (tx, rx) = mpsc::sync_channel(1);
    (Box::new(ChannelCallback { sender: tx }), rx)
}

/// Sends an asynchronous ping to the Data Layer and waits for the response.
fn ping_async(client: &Client) {
    let (callback, rx) = make_callback();
    let result = client.r#async().ping(callback);
    if result != ctrlx_datalayer::Result::Ok {
        error!("Async ping failed: {}", result);
        return;
    }

    // Wait for the response
    let (_res, _value) = rx.recv_timeout(std::time::Duration::from_secs(5))
        .expect("Timeout: No ping callback received within 5 seconds");
    info!("Async ping: {}", result);
}

/// Asynchronously browses the root of the Data Layer and prints the available nodes.
fn browse_async(client: &Client) {
    let (callback, rx) = make_callback();
    let result = client.r#async().browse("", callback);
    if result != ctrlx_datalayer::Result::Ok {
        error!("Async browse failed: {}", result);
        return;
    }
    let (_res, value) = rx.recv_timeout(std::time::Duration::from_secs(5))
        .expect("Timeout: No browse callback received within 5 seconds");

    let nodes = value.as_ref::<Vec<String>>().cloned().unwrap_or_default();
    info!("Async Browse Values: {:?}", nodes);
}

/// Asynchronously reads the scheduler counter from the Data Layer and prints its value.
fn read_async(client: &Client) {
    let (callback, rx) = make_callback();
    let result = client.r#async().read("scheduler/admin/info/counter", None, callback);
    if result != ctrlx_datalayer::Result::Ok {
        error!("Async read failed: {}", result);
        return;
    }
    
    let (_res, value) = rx.recv_timeout(std::time::Duration::from_secs(5))
        .expect("Timeout: No callback received within 5 seconds");  
    info!("Read value: {}", value.as_ref::<u64>().unwrap());  
}

/// Runs all asynchronous Data Layer operations (ping, browse, read).
fn run_async(client: &Client) {
    ping_async(client);
    browse_async(client);
    read_async(client);
}

/// Runs both synchronous and asynchronous Data Layer operations.
fn run(client: &Client) {
    run_sync(client);
    run_async(client);
}

/// Entry point: Initializes the Data Layer system, connects a client, and runs operations in a loop.
fn main() {

    env_logger::init();

    // Shared flag that is set to true when a termination signal is received.
    let shutdown = Arc::new(AtomicBool::new(false));
    // Register OS signals so the main loop exits cleanly on SIGINT / SIGTERM / SIGABRT.
    signal_hook::flag::register(signal_hook::consts::SIGINT, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGTERM, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGABRT, Arc::clone(&shutdown)).unwrap();

    let system = System::new("");
    system.start(false);
    {
        let client = system.factory().create_client(Connection::IpVirtual);

        while client.is_connected() && !shutdown.load(Ordering::Relaxed) {
            info!("Connected to ctrlX Data Layer");
            run(&client);
            std::thread::park_timeout(std::time::Duration::from_secs(1));
        }
    }
}
