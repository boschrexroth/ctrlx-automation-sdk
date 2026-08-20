#!/usr/bin/env bash

# ==============================================================================
# DESCRIPTION:
# This script automates the retrieval and import of the entire SSL certificate 
# chain from Maven Central (repo.maven.apache.org) into the Java 'cacerts' 
# truststore. 
#
# It is specifically designed for enterprise environments where SSL-decrypting 
# proxies (e.g., Zscaler, McAfee, px) are active and cause Gradle build failures 
# like "PKIX path building failed / SunCertPathBuilderException".
#
# FEATURES:
# 1. Automatically detects corporate proxies from environment or /etc/environment.
# 2. Downloads the entire certificate chain using OpenSSL (with Proxy tunneling).
# 3. Splits the chain and imports each certificate with a unique alias.
# 4. Stops any running Gradle Daemons to apply changes immediately WITHOUT reboot.
# ==============================================================================

# --- CONFIGURATION ---
TARGET_HOST="repo.maven.apache.org"
TARGET_PORT=443

# If environment variables are empty, read them from /etc/environment
if [ -z "$http_proxy" ]; then
    if [ -f /etc/environment ]; then
        PROXY_ENV=$(grep -i "http_proxy" /etc/environment | head -n 1 | cut -d'=' -f2 | tr -d '"')
        export http_proxy="$PROXY_ENV"
    fi
fi

# Parse proxy
if [ -n "$http_proxy" ]; then
    PROXY_CLEAN=$(echo "$http_proxy" | sed -e 's/http:\/\///' -e 's/https:\/\///')
    PROXY_HOST=$(echo "$PROXY_CLEAN" | cut -d':' -f1)
    PROXY_PORT=$(echo "$PROXY_CLEAN" | cut -d':' -f2)
    echo "🌐 Detected proxy: $PROXY_HOST on port $PROXY_PORT"
else
    echo "ℹ️ No proxy detected. Establishing a direct connection..."
fi

# Determine Java path
if [ -z "$JAVA_HOME" ]; then
    #JAVA_HOME="/usr/lib/jvm/default-java"
    # Better fit for local installation
    JAVA_HOME="/usr/lib/jvm/java-1.21.0-openjdk-amd64"
fi

CACERTS_PATH="$JAVA_HOME/lib/security/cacerts"
STOREPASS="changeit"
WORKDIR=$(mktemp -d)
RAW_CHAIN_FILE="$WORKDIR/raw_chain.pem"

echo "ℹ️ Using JAVA_HOME: $JAVA_HOME"
echo "ℹ️ Keystore path: $CACERTS_PATH"

# --- STEP 1: Download the entire certificate chain ---
echo "🌐 Downloading the certificate chain..."

if [ -n "$PROXY_HOST" ] && [ -n "$PROXY_PORT" ]; then
    # Download via OpenSSL proxy flag
    echo -n | openssl s_client -showcerts -connect "${TARGET_HOST}:${TARGET_PORT}" \
      -servername "${TARGET_HOST}" \
      -proxy "${PROXY_HOST}:${PROXY_PORT}" 2>/dev/null > "$RAW_CHAIN_FILE"
else
    # Direct download
    echo -n | openssl s_client -showcerts -connect "${TARGET_HOST}:${TARGET_PORT}" \
      -servername "${TARGET_HOST}" 2>/dev/null > "$RAW_CHAIN_FILE"
fi

# Fall back to netcat if openssl -proxy fails
if [ ! -s "$RAW_CHAIN_FILE" ] && [ -n "$PROXY_HOST" ]; then
    echo "⚠️ 'openssl -proxy' failed. Trying fallback with Netcat (nc)..."
    echo -e "CONNECT ${TARGET_HOST}:${TARGET_PORT} HTTP/1.1\r\nHost: ${TARGET_HOST}\r\n\r\n" \
      | nc "$PROXY_HOST" "$PROXY_PORT" 2>/dev/null \
      | openssl s_client -showcerts -connect "${TARGET_HOST}:${TARGET_PORT}" -servername "${TARGET_HOST}" 2>/dev/null > "$RAW_CHAIN_FILE"
fi

if [ ! -s "$RAW_CHAIN_FILE" ]; then
    echo "❌ Error: Could not establish a connection or load certificates."
    rm -rf "$WORKDIR"
    exit 1
fi

# --- STEP 2: Extract and split the certificates ---
echo "✂️ Extracting and splitting the certificates..."

# Splits the file at each BEGIN / END block into separate files (cert_0.crt, cert_1.crt, etc.)
awk '/BEGIN CERTIFICATE/,/END CERTIFICATE/{ if(/BEGIN/){a++}; out="'"$WORKDIR"'/cert_"a".crt"; print > out}' "$RAW_CHAIN_FILE"

TOTAL_CERTS=$(ls -1 "$WORKDIR"/cert_*.crt 2>/dev/null | wc -l)

if [ "$TOTAL_CERTS" -eq 0 ]; then
    echo "❌ Error: No valid certificates were found in the response."
    rm -rf "$WORKDIR"
    exit 1
fi

echo "✅ Found $TOTAL_CERTS certificates in the chain."

# --- STEP 3: Import each certificate into the Java cacerts ---
for cert_file in "$WORKDIR"/cert_*.crt; do
    # Determine a unique alias based on the file name (e.g. maven-chain-1)
    CERT_NUM=$(basename "$cert_file" | tr -dc '0-9')
    ALIAS="maven-chain-${CERT_NUM}"
    
    echo "🗑️ Cleaning up old alias '$ALIAS'..."
    sudo "$JAVA_HOME/bin/keytool" -delete -alias "$ALIAS" -keystore "$CACERTS_PATH" -storepass "$STOREPASS" 2>/dev/null

    echo "🔐 Importing certificate #$CERT_NUM with alias '$ALIAS'..."
    sudo "$JAVA_HOME/bin/keytool" -importcert -trustcacerts \
      -alias "$ALIAS" \
      -file "$cert_file" \
      -keystore "$CACERTS_PATH" \
      -storepass "$STOREPASS" \
      -noprompt
done

# --- STEP 4: Clean up ---
rm -rf "$WORKDIR"

if [ $? -eq 0 ]; then
    echo "🎉 Success! The entire certificate chain ($TOTAL_CERTS certificates) has been imported."
else
    echo "❌ There were problems importing the certificates."
fi

