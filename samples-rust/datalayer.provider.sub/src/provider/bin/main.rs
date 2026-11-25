/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::Arc;
use log::{error, info};
use ctrlx_datalayer::{factory::Connection, System, ProviderNode};

use provider_lib::{ADDRESS_FOLDER_WILDCARD, ProviderNodeFolderCallback};

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
        // Create a provider connected via the virtual IP interface.
        let provider = system.factory().create_provider(Connection::IpVirtual);

        let folder_callback = ProviderNodeFolderCallback::new();
        let node = ProviderNode::new(Box::new(folder_callback), true);

        let result = provider.register_node(ADDRESS_FOLDER_WILDCARD, &node);
        if result.is_err() {
            error!("Failed to register node: {:?}", result.err());
            return;
        }

        let result = provider.start();
        if let Err(e) = result {
            error!("Failed to start provider: {:?}", e);
            return;
        }   
        
        while !shutdown.load(Ordering::Relaxed) && provider.is_connected() {
            // park_timeout returns immediately on EINTR (unlike thread::sleep which
            // retries with the original absolute deadline). When SIGINT or SIGTERM
            // arrives, the signal handler sets `shutdown` to true and EINTR wakes
            // this thread, so the loop exits without waiting the full tick.
            std::thread::park_timeout(std::time::Duration::from_secs(1));
        }

        info!("Shutting down provider...");
    }
}
