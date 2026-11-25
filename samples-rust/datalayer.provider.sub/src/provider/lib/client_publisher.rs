/*
 * SPDX-FileCopyrightText: Bosch Rexroth AG
 *
 * SPDX-License-Identifier: MIT
 */

use ctrlx_datalayer::{
    provider::subscription::{NodeSubscription, NotifyItemPublish},
    result,
};
use ctrlx_fbs::notify_info_generated::comm::datalayer::NotifyType;
use std::sync::atomic::{AtomicBool, Ordering};
use std::sync::{Arc, Mutex};

/// Inner state accessible by the timer thread.
struct PublisherInner {
    /// The node subscription that provides subscriber info and the publish channel.
    sub: Box<dyn NodeSubscription>,
    /// Monotonically incrementing counter used as demo payload value.
    counter: i32,
}

impl PublisherInner {
    /// Increments the counter and publishes the new value to all subscribed nodes.
    fn tick(&mut self) {
        // Wrap around on overflow instead of panicking.
        self.counter = self.counter.wrapping_add(1);

        // Collect all nodes that are part of this subscription.
        let nodes = self.sub.nodes();
        let mut list = Vec::<NotifyItemPublish>::new();

        // Build one notification item per subscribed node.
        for node in nodes {
            let mut item = NotifyItemPublish::new(node.clone());
            // Write the current counter value as the node's payload.
            item.data_mut().int32_mut(self.counter);
            // Attach a UTC timestamp so subscribers can detect staleness.
            item.timestamp_mut(chrono::Utc::now());
            // Signal that the notification carries fresh data.
            item.notify_type_mut(NotifyType::Data);
            list.push(item);
        }

        // Push the notification list to all subscribers.
        let result = self.sub.publish(result::Result::Ok, &list);
        if let Err(e) = result {
            log::error!(
                "Failed to publish subscription {}: {:?}",
                self.sub.unique_id(),
                e
            );
            return;
        }
        log::info!(
            "Published subscription {} with value {}",
            self.sub.unique_id(),
            self.counter
        );
    }
}

/// Drives periodic data publication for a single client subscription.
///
/// Spawns a background thread that wakes at the subscription's configured
/// publish interval and calls [`PublisherInner::tick`] to push updated values.
pub struct ClientPublisher {
    /// Inner state shared with the timer thread. 
    /// Reason for the mutex: 'fn tick(&mut self)' needs mutable access to the counter, 
    /// but 'ProviderNodeFolderCallback' also needs access to the subscription to read 
    /// properties and nodes.
    inner: Arc<Mutex<PublisherInner>>,
    /// Mutex reference from `ProviderNodeFolderCallback` for synchronized access.
    provider_mutex: Arc<Mutex<()>>,
    /// Flag that signals the background thread to exit.
    running: Arc<AtomicBool>,
    /// Handle to the background publish thread; `None` before `start()` is called.
    thread: Option<std::thread::JoinHandle<()>>,
}

impl ClientPublisher {
    /// Creates a new `ClientPublisher`.
    /// `sub` is the subscription, `provider_mutex` is owned by `ProviderNodeFolderCallback`.
    pub fn new(sub: Box<dyn NodeSubscription>, provider_mutex: Arc<Mutex<()>>) -> Self {
        Self {
            inner: Arc::new(Mutex::new(PublisherInner { sub, counter: 0 })),
            provider_mutex,
            running: Arc::new(AtomicBool::new(false)),
            thread: None,
        }
    }

    /// Starts the background publish thread.
    ///
    /// The thread wakes at the publish interval configured by the subscriber and
    /// calls [`PublisherInner::tick`] on every iteration.  If no interval is set
    /// the default of 1 000 ms is used.
    pub fn start(&mut self) {
        // Read the desired publish interval from the subscription properties.
        let interval_ms = self
            .inner
            .lock()
            .unwrap()
            .sub
            .properties()
            .map(|p| p.publishInterval as u64)
            .unwrap_or(1000);
        let interval = std::time::Duration::from_millis(interval_ms);

        // Mark the publisher as running before spawning the thread.
        self.running.store(true, Ordering::Relaxed);

        // Clone Arcs so they can be moved into the thread closure.
        let running = Arc::clone(&self.running);
        let inner = Arc::clone(&self.inner);
        let provider_mutex = Arc::clone(&self.provider_mutex);

        let handle = std::thread::spawn(move || {
            while running.load(Ordering::Relaxed) {
                // Sleep for the publish interval; `stop()` unparks the thread
                // early so it can exit without waiting the full interval.
                std::thread::park_timeout(interval);
                // Re-check after waking up: if stop() was called, exit immediately
                // *before* trying to acquire provider_mutex. Otherwise stop() would
                // deadlock because it holds provider_mutex while waiting on join().
                if !running.load(Ordering::Relaxed) {
                    break;
                }
                {
                    // Acquire the provider mutex first to synchronize with the
                    // provider callback, then publish the next value.
                    let _guard = provider_mutex.lock().unwrap();
                    inner.lock().unwrap().tick();
                }
            }
        });
        self.thread = Some(handle);
    }

    /// Stops the timer and waits for the thread to finish.
    pub fn stop(&mut self) {
        self.running.store(false, Ordering::Relaxed);
        if let Some(handle) = self.thread.take() {
            handle.thread().unpark();
            handle.join().ok();
        }
    }
}
