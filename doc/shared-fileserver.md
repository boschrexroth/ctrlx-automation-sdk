# Shared Webserver

## Introduction

The Shared Webserver feature provides a centralized hosting solution for web applications within the ctrlX OS ecosystem. This service enables seamless deployment of static web content for applications that don't require dedicated backend services, promoting efficient resource utilization and simplified app development.

## Overview

In the ctrlX OS environment, there are two primary categories of applications that benefit from shared web hosting:

1. **Data Layer-based Applications**: Apps that provide their data exclusively through the data layer (e.g., `rexroth-plc`) and don't require a custom backend implementation
2. **Static Web Applications**: Apps that consist solely of web content without backend (e.g., `documentation`)

The Automation Core addresses these use cases by providing a shared webserver that serves static files via HTTP(S), eliminating the need for individual web servers per application.

> **Important Note**: This pattern intentionally deviates from the "self-contained" application principle and should only be used for the specific use cases mentioned above.

## Architecture

The Automation Core implements a dedicated web hosting service with the following characteristics:

- **Independent Process**: Runs as a standalone service within the Automation Core context
- **Shared Directory Structure**: Hosts content from the `$SNAP_DATA/package-www` directory
- **Unix Socket Communication**: Utilizes snap-internal Unix ports accessible through the reverse proxy

### Content Sharing Mechanism

The shared webserver uses the content interface to manage web content. The Automation Core provides a content slot that enables other snaps to connect and share their web assets.

## Implementation Guide

### Third-Party Application Integration

To integrate a web application with the shared webserver, third-party snaps must configure their web content to be accessible through the shared hosting mechanism.

#### Prerequisites

- Web content should be organized in the snap path: `$SNAP/package-www/${SNAPCRAFT_PROJECT_NAME}/`
- The web application must be self-contained and not require server-side processing

#### Snapcraft Configuration

Configure your `snapcraft.yaml` file as follows:

```yaml
# [...]
parts:  
  webapp:
    source: ./webapp # The folder containing you static web application
    plugin: dump
    organize:
      'webapp/*': package-www/${SNAPCRAFT_PROJECT_NAME}/ # Ensuring the content is organized correctly
# [...]
slots:
  package-www:
    interface: content
    content: package-www
    source:
      read:
        - $SNAP/package-www/${SNAPCRAFT_PROJECT_NAME}
# [...]
```

#### Package Manifest Configuration

Define the service routing and menu integration in your package manifest file (`rexroth-helloworld.package-manifest.json`):

```json
{
  "services": {
    "proxyMapping": [
      {
        "name": "rexroth-helloworld",
        "url": "/rexroth-helloworld",
        "binding": "unix://{$PACKAGE_WWW_SOCKET}"
      }
    ]
  },
  "menus": {
    "sidebar": [
      {
        "id": "rexroth-helloworld",
        "title": "Hello World",
        "link": "/rexroth-helloworld"
      }
    ]
  }
}
```

## Best Practices

- **Namespace Isolation**: Use your snap's project name as the directory namespace to avoid conflicts
- **Static Content Only**: Ensure your web application consists only of static assets (HTML, CSS, JavaScript, images)
- **Resource Optimization**: Minimize file sizes and optimize assets for web delivery
- **Security Considerations**: Validate that your web content doesn't expose sensitive information