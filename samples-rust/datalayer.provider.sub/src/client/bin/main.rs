/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use log::{error, info};
use ctrlx_datalayer::{factory::Connection, System, SubscriptionPropertiesBuilder, SubscriptionCallback, client::subscription::NotifyItem, VariantType};
use provider_lib::{ADDRESS_INT1, ADDRESS_INT2};

/// Subscription callback that logs every incoming notification to stdout.
///
/// The ctrlX Data Layer calls [`SubscriptionCallback::on_notify`] on this
/// implementation whenever one of the subscribed nodes publishes a new value.
struct SubscriptionCallbackImpl;

impl SubscriptionCallback for SubscriptionCallbackImpl {
    /// Called by the data layer whenever subscribed nodes publish new values.
    fn on_notify(&mut self, status: ctrlx_datalayer::Result, items: Vec<NotifyItem>) {
        // Check whether the notification arrived without errors.
        if status != ctrlx_datalayer::Result::Ok {
            error!("Subscription notification error: {:?}", status);
            return;
        }
        // Iterate over all changed items in this notification batch.
        for item in &items {
            // Fall back to printing the value as a i32.
            if item.data.variant_type() == VariantType::Int32 {
            info!(
                "address: {}, value: {:?}",
                item.address(),
                item.data.as_ref::<i32>().unwrap_or(&-1)
            );
            continue;
            } 
            info!(
                "address: {}, value: {:?}",
                item.address(),
                item.data.variant_type()
            );  
        }
    }
}

/// Entry point of the client binary.
///
/// Creates a ctrlX Data Layer client, subscribes to two integer nodes provided
/// by the companion provider binary, and logs all incoming notifications until a
/// termination signal is received.
fn main() {
    env_logger::init();
    // Shared flag that is set to true when a termination signal is received.
    let shutdown = Arc::new(AtomicBool::new(false));
    // Register OS signals so the main loop exits cleanly on SIGINT / SIGTERM / SIGABRT.
    signal_hook::flag::register(signal_hook::consts::SIGINT, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGTERM, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGABRT, Arc::clone(&shutdown)).unwrap();    
    
    // Initialise the ctrlX system runtime; passing an empty string selects the default snap environment.
    let system = System::new("");
    system.start(false);
    {
        // Create a client connected via the virtual IP interface.
        let client = system.factory().create_client(Connection::IpVirtual);
        // Build subscription properties with a unique ID; default publish interval applies.
        let props = SubscriptionPropertiesBuilder::new("test_rust_sub_id".to_string()).build();
        let callback = Box::new(SubscriptionCallbackImpl);
        let result = client.create_subscription(props, callback);
        if let Err(e) = result {
            error!("Failed to create subscription: {:?}", e);
            return;
        }
        let mut subscription = result.unwrap();

        // List of data layer node addresses to subscribe to.
        let addresses = vec![
            ADDRESS_INT1,
            ADDRESS_INT2,
        ];
        // Subscribe to the node addresses.
        let result = subscription.subscribe(&addresses);
        if result.is_err() {
            error!("Failed to subscribe to addresses: {:?}", result.err());
            return;
        }        
        // Keep the client alive as long as the connection is healthy and no signal was received.
        // park_timeout wakes early on EINTR so signal delivery exits the loop promptly.
        while client.is_connected() && !shutdown.load(Ordering::Relaxed) {
            std::thread::park_timeout(std::time::Duration::from_secs(1));
        }
        info!("Shutting down client...");
        subscription.unsubscribe_all().unwrap();
    }
}
