# Self-Health Monitoring Example

In this example, you'll learn how to implement proactive health monitoring for your application on ctrlX AUTOMATION. Instead of relying on external monitoring services, your app will report its own health status directly to the system, making it visible in ctrlX Diagnostics and the Datalayer.

This approach uses snapd's built-in health-state feature to reflect your application's operational status throughout its lifecycle - from startup to shutdown.

## Lesson Plan

By completing this tutorial, you will:

- Understand how snap health states work and their lifecycle.
- Learn to set health status during different phases (startup, operation, shutdown).
- Implement self-health reporting in an application.
- Integrate health status with ctrlX Datalayer and Diagnostics.
- Handle health states during normal shutdown vs. crashes.

## What We'll Work On

You'll build a sample application (`./app/dummy-app`) that manages its own health status by:

- Setting status to _waiting_ during startup.
- Transitioning to _okay_ when ready for operation.
- Reporting _error_ status when problems are detected.
- Properly handling health states during shutdown scenarios.

## What You'll Need

- ctrlX AUTOMATION development environment.
- Basic understanding of snap services and daemons.
- Familiarity with snapcraft.yaml configuration.
- Access to the ctrlX Datalayer.

**Reference Documentation:**
- [Snap Health-State](https://snapcraft.io/docs/using-snapctl#heading--health-state)
- [Snap Services and Commands](https://snapcraft.io/docs/services-and-daemons/)

## Project Layout

The self-health example has the following structure:

```
self-health/
├── bin/
│   ├── dummy-app          # Main application that reports its own health
│   └── wrapper.sh         # Wrapper script that detects crashes
├── snap/
│   └── snapcraft.yaml     # Snap configuration with daemon settings
├── build-snap-amd64.sh    # Build script for amd64 architecture
└── build-snap-arm64.sh    # Build script for arm64 architecture
```

**Key Files:**
- __bin/dummy-app__: The main application that sets health status during startup, operation, and shutdown.
- __bin/wrapper.sh__: Wrapper script that executes the app and detects crashes based on exit codes.
- __snap/snapcraft.yaml__: Defines the snap service configuration including daemon settings.

## Step-by-Step: Building the Example

### Step 1: Understanding Health State Behavior

Before implementing, understand these key behaviors:

- **Persistence**: The health status persists even when the app is _stopped_ or _disabled_.
- **Visibility**: Health status appears in:
  - `snap info {SNAP_NAME_HERE} --verbose` (always shows health).
  - `snap list {SNAP_NAME_HERE}` (shows health if not okay).
  - ctrlX Datalayer at `system/health/apps`.
  - ctrlX Diagnostics message when health transitions to error.

### Step 2: Configure Your Service

In your __snapcraft.yaml__, configure the service as a daemon:

- Set `daemon: simple` for a long-running service.
- Configure `restart-condition` and `restart-delay` as needed.
- Your application's command wrapper will handle health status reporting when the app crashes.

```yaml
apps:
  app:
    command: wrapper.sh
    daemon: simple
    restart-condition: always
    restart-delay: 10s
```

### Step 3: Implement Startup Health Reporting

During your application startup:

1. Set health status to _waiting_ immediately when the service starts.
2. Initialize your application resources.
3. Set health status to _okay_ once fully operational.
4. Set health status to _error_ if initialization fails.

From __bin/dummy-app__:

```bash
echo "Dummy App [waiting]: starting ... "
snapctl set-health waiting "starting"
sleep 5s

echo "Dummy App [okay]: ready"
snapctl set-health okay
```

### Step 4: Implement Runtime Health Monitoring

During operation, your application should:

1. Monitor its own operational conditions.
2. Proactively set status to _error_ when problems are detected.
3. Include meaningful error messages with the health status.

From __bin/dummy-app__:

```bash
echo "Dummy App [error]: uuups, something went wrong: error"
# when status is not "okay", message is required
# code must start with lowercase ASCII letters, and contain only ASCII letters and numbers, optionally separated by single dashes
snapctl set-health error --code="code-17" "error message"

sleep 5s    
echo "Dummy App [okay]: ready again"
snapctl set-health okay
```

### Step 5: Handle Shutdown Scenarios

Implement proper health status management during shutdown:

- Set status to _error_ before exiting abnormally, if possible.
- Note that health status persists after shutdown - consider resetting to _okay_ or _waiting_ on next startup.

The __wrapper script__ (bin/wrapper.sh) detects crashes and sets the health accordingly:

```bash
$SNAP/dummy-app "$@"
result=$?
if [ $result -eq 0 ]; then
    echo "Wrapper dummy-app exited successfully. Setting to waiting"
    snapctl set-health waiting "waiting for restart"
else
    echo "dummy-app failed with exit code: $result"
    snapctl set-health error --code="crash-01" "application crashed with exit code $result"
fi

exit $result
```

### Step 6: Test and Verify

Verify your implementation by:

1. Starting the service and checking initial _waiting_ status.
2. Confirming transition to _okay_ when ready.
3. Triggering error conditions and verifying _error_ status.
4. Checking health visibility in ctrlX Diagnostics.
5. Querying the Datalayer node `system/health/apps`.

### Benefits of This Approach

- **No external monitoring needed**: Your application reports its own health without requiring a separate monitoring service.
- **Proactive reporting**: Service reports its own state immediately.
- **System integration**: Health status integrates with ctrlX system diagnostics.

### Limitations to Consider

- Does not automatically detect crashes without service-side reporting.
- Health status persistence requires manual reset after stopping/disabling services.

## Support

### Developer Community

Please join the [Developer Community](https://developer.community.boschrexroth.com/)

### SDK Forum

Please visit the [SDK Forum](https://developer.community.boschrexroth.com/t5/ctrlX-AUTOMATION/ct-p/dcdev_community-bunit-dcae/)

### Issues

If you've found an error in these sample, please [file an issue](https://github.com/boschrexroth)

## License

SPDX-FileCopyrightText: Bosch Rexroth AG

SPDX-License-Identifier: MIT
