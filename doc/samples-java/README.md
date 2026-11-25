# Samples Java

## Basic Samples

* [ctrlX Data Layer Client](./datalayer.client.simple/README.md)
* [ctrlX Data Layer Client Subscription](./datalayer.client.sub/README.md)
* [ctrlX Data Layer Client Bulk](./datalayer.client.bulk/README.md)
* [ctrlX Data Layer Provider](./datalayer.provider.simple/README.md)
* [ctrlX Data Layer Provider Subscription](./datalayer.provider.sub/README.md)

## Prerequisites

### Java & Gradle

Java JDK LTS is used for development. [Gradle Build Tool](https://gradle.org/) is used as a build tool for the Java projects. The Gradle settings are based on Kotlin DSL.

Java JDK and Gradle are installed using the following script (see script for versions).

```bash
    cd ctrlx-automation-sdk/scripts
    ./install-java-gradle.sh
```

Gradle Builds

```bash
    ./gradlew clean build
```

### Visual Studio Code

#### Java Extensions

Java Extensions for Visual Studio Code can be loaded using this script or see [Java in Visual Studio Code](https://code.visualstudio.com/docs/languages/java).

This script must be executed in a Visual Stuido Code terminal/console.

```bash
    ./install-vcode-extensions-java.sh
```

Settings:

"\<ctrl>\<shift>+p" "Java: Clean Java Language Server Workspace"

### Install __ctrlX Data Layer__ build and runtime environment

```bash
    ctrlx-automation-sdk/scripts/install-ctrlx-datalayer.sh
```

## Java/Gradle and Snap Build within a Corporate Proxy

### Gradle Build

`'gradle_proxy.sh'` can be used for a simple application build.

### Snapcraft Build

The Snapcraft gradle plugin requires uncommenting the following lines.
See: `gradle-parameters:` and `build-environment`

```yaml
parts:
  client:
    plugin: gradle
    source: .
    #gradle-parameters:
    #  - -Djavax.net.ssl.trustStore="/etc/ssl/certs/java/cacerts"    
    build-environment:
      - SNAP_BUILD_JAVA_VERSION: "21"
    #  - http_proxy: "http://10.0.2.2:3128"
    #  - https_proxy: "http://10.0.2.2:3128"      
    build-packages:
      - openjdk-21-jdk-headless
    stage-packages:
      - to amd64:
        - openjdk-21-jre-headless
      - to arm64:
        - openjdk-21-jre-headless:arm64
```

### Problems

When using a 'Corporate Proxy', the following errors can occur when using Java/Gradle or Snap Build.

__Problem:__

```text
Could not GET 'https://repo.maven.apache.org/maven2/com/google/guava/guava/33.4.5-jre/guava-33.4.5-jre.pom'.
    Got SSL handshake exception during request. It might be caused by SSL misconfiguration
        > (certificate_unknown) PKIX path building failed: sun.security.provider.certpath.SunCertPathBuilderException: unable to find valid certification path to requested target
```

__Reason:__
The error message `PKIX path building failed: unable to find valid certification path to requested target` means that your Java environment does not trust the SSL certificate of the Maven repository (`repo.maven.apache.org`). In corporate networks, it is common for a security firewall or proxy to intercept all internet traffic and encrypt it with its own corporate certificate. By default, Java does not recognize this corporate certificate as trustworthy, which leads to this SSL handshake error.

__Solution: (Manual)__
Of course, here is the translation:

* __Step 1:__ Export the SSL Certificate

    First, you need to obtain the required certificate (usually your company's root certificate). The easiest way is to export it from your browser:
    1. Open a web browser (e.g., Chrome or Firefox) and go to any HTTPS page, for example, `https://repo.maven.apache.org`.
    2. Click on the lock icon in the address bar to view the certificate information.
    3. Navigate through the certificate chain and find the top-level certificate (the root certificate).
    4. Export this certificate as a file to your computer. Choose the format "Base-64 encoded X.509 (.CER)". Save it with an easily recognizable name, e.g., `my-root.cer`.

* __Step 2:__ Find the correct Java Truststore (`cacerts`)

    It is crucial to import the certificate into the `cacerts` file of the Java installation that is used by Gradle.
    __If you run Gradle from the command line__, it likely uses the JDK defined in your `JAVA_HOME` environment variable. The path to the truststore file would then typically be:
        __Linux__: `/usr/lib/jvm/java-xx-openjdk/lib/security/cacerts` or `$JAVA_HOME/lib/security/cacerts` or `/etc/ssl/certs/java/cacerts`

* __Step 3:__ Import the Certificate with `keytool`

    Once you have the path to the `cacerts` file, you can import the certificate using the Java `keytool` utility.

    1. Open a command line or a terminal __with administrator privileges__. (On Linux, prefix the command with `sudo`).
    2. Execute the following command. Adjust the file paths for your certificate and your `cacerts` file accordingly:

    ```bash
        # Simplification of the assumption: '/etc/ssl/certs/java/cacerts'
        sudo keytool -import -trustcacerts -alias my-corporate-root -file "./my-root.cer" -keystore "/etc/ssl/certs/java/cacerts"
    ```

    3. You will be prompted for the keystore password. If it has never been changed, the default password is `changeit`.
    4. Confirm with `yes` (or `ja`) that you want to trust this certificate.

__Solution: (Helper Script)__
These individual steps are summarized in the script `ctrlx-automation-sdk/scripts/import_java_entire_chain.sh`. For this script to function properly, it is recommended to set the __JAVA_HOME__ environment variable.
Thereafter, the 'gradle' daemon must be stopped with ./gradlew --stop or the system must be rebooted.
