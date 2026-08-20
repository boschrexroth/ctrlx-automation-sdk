#!/bin/bash

#url="http://10.0.2.2:3128"

# Extract host
# First remove "http://", then split by : and take the first field
host=$(echo "$HTTP_PROXY" | sed 's/http:\/\///' | cut -d':' -f1)

# Extract port
# First remove "http://", then split by : and take the second field
port=$(echo "$HTTP_PROXY" | sed 's/http:\/\///' | cut -d':' -f2)

echo "Host: $host"
echo "Port: $port"

ARG_GRADLE=(
  "-Dhttp.proxyHost=${host}"
  "-Dhttp.proxyPort=${port}"
  "-Dhttps.proxyHost=${host}"
  "-Dhttps.proxyPort=${port}"
)

if [ -f "/etc/ssl/certs/java/cacerts" ]; then
  ARG_GRADLE+=("-Djavax.net.ssl.trustStore=/etc/ssl/certs/java/cacerts")
  echo "cacerts found in: /etc/ssl/certs/java/cacerts"
elif [ -f "${JAVA_HOME}/lib/security/cacerts" ]; then
  ARG_GRADLE+=("-Djavax.net.ssl.trustStore=${JAVA_HOME}/lib/security/cacerts")
  echo "cacerts found in: ${JAVA_HOME}/lib/security/cacerts"
fi

./gradlew "${ARG_GRADLE[@]}" "$@"