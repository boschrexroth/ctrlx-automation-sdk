/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
use ctrlx_datalayer::{
    client::subscription::NotifyItem, client::subscription::SubscriptionCallback,
    factory::Connection, subscription_properties_builder::SubscriptionPropertiesBuilder,
    system::System,
};
use log::{error, info};
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};

/// Subscription callback implementation that handles incoming data layer notifications.
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
            if item.data.variant_type() == ctrlx_datalayer::variant::VariantType::Float64 {
                // Print floating-point values with full precision.
                info!(
                    "address: {}, value: {}",
                    item.address(),
                    item.data.as_ref::<f64>().unwrap()
                );
                continue;
            }
            // Fall back to printing the value as a u32.
            info!(
                "address: {}, value: {:?}",
                item.address(),
                item.data.as_ref::<u32>().unwrap()
            );
        }
    }
}

fn main() {
    // Initialize the logger (reads RUST_LOG environment variable).
    env_logger::init();

    // Shared flag that is set to true when a termination signal is received.
    let shutdown = Arc::new(AtomicBool::new(false));
    // Register OS signals so the main loop exits cleanly on SIGINT / SIGTERM / SIGABRT.
    signal_hook::flag::register(signal_hook::consts::SIGINT, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGTERM, Arc::clone(&shutdown)).unwrap();
    signal_hook::flag::register(signal_hook::consts::SIGABRT, Arc::clone(&shutdown)).unwrap();

    // Create and start the ctrlX Data Layer system.
    let system = System::new("");
    system.start(false);
    {
        // Create a client that connects via the virtual IP address (snap-internal communication).
        let client = system.factory().create_client(Connection::IpVirtual);

        // Build subscription properties with a unique subscription ID.
        let props = SubscriptionPropertiesBuilder::new("test_rust_id".to_string()).build();
        let callback = Box::new(SubscriptionCallbackImpl);

        // Create the subscription and attach the callback.
        let result = client.create_subscription(props, callback);

        if result.is_err() {
            error!("Failed to create subscription: {:?}", result.err());
            return;
        }
        let mut subscription = result.unwrap();

        // List of data layer node addresses to subscribe to.
        let addresses = vec![
            "framework/metrics/system/cpu-utilisation-percent",
            "framework/metrics/system/memused-percent",
        ];

        // Subscribe to the node addresses.
        let result = subscription.subscribe(&addresses);
        if result.is_err() {
            error!("Failed to subscribe to addresses: {:?}", result.err());
            return;
        }

        // Keep the application running while the client is connected and no signal was received.
        while client.is_connected() && !shutdown.load(Ordering::Relaxed) {
            info!("Connected to ctrlX Data Layer");
            // Sleep in short intervals so signal flags (SIGINT/SIGTERM/SIGABRT) are
            // checked promptly without blocking the loop for a full long sleep period.
            std::thread::park_timeout(std::time::Duration::from_secs(1));
        }

        info!("Shutting down subscription and client");
        // Unsubscribe from all addresses and stop the subscription.
        subscription.unsubscribe_all().unwrap();
    }
}
