#!/bin/bash

#url="http://10.0.2.2:3128"

# Host extrahieren
# Zuerst "http://" entfernen, dann nach : trennen und das erste Feld nehmen
host=$(echo "$HTTP_PROXY" | sed 's/http:\/\///' | cut -d':' -f1)

# Port extrahieren
# Zuerst "http://" entfernen, dann nach : trennen und das zweite Feld nehmen
port=$(echo "$HTTP_PROXY" | sed 's/http:\/\///' | cut -d':' -f2)

echo "Host: $host"
echo "Port: $port"

./gradlew -Dhttp.proxyHost="${host}" -Dhttp.proxyPort="${port}" -Dhttps.proxyHost="${host}" -Dhttps.proxyPort="${port}" "$@"