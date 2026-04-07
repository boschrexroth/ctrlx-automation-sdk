#!/bin/bash

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
