This document is part of the Bosch Rexroth ctrlX OS App Development Guideline, and describes specific topics for Apps which are based on the Docker container technology

Please refer to the App Development Guide for further information about general requirements and checks

The content of this document may be subject of change in future.

# 1. Artifacts / Files

In addition to a native Ubuntu Core App, a Docker App shall provide the files in this list

| Files | Description | Value(s) | Optional / Mandatory |
| --- | --- | --- |--- |
| Docker-compose.yml | The docker-compose.yml file contains the configuration and options to run the services required by the snap | docker-compose.yml	| Mandatory |
| Docker-compose.env | The docker-compose.env file contains the environment variable(s) set to run the snaps | docker-compose.env |Optional |

# 2. Base Checks

## 2.1 snapcraft yaml / snap.yaml

In addition to the checks for snap.yaml which are described [here](appdevguide_basechecks.md), a Docker App has additional elements:

| Item | Content | Optional / Mandatory |
| --- | --- | --- |
| docker-compose | configuration file | Mandatory |
| docker-volumes | manage app data | Mandatory |

The mandatory entries must be provided like the following:

```yaml
parts:
  docker-compose:
    plugin: dump
    source: ./docker-compose
    organize:
      '*': docker-compose/${SNAPCRAFT_PROJECT_NAME}/
slots:
  docker-compose:
    interface: content
    content: docker-compose
    source:
      read:
        - $SNAP/docker-compose/${SNAPCRAFT_PROJECT_NAME}
  docker-volumes:
    interface: content
    content: docker-volumes
    source:
      write:
        - $SNAP_DATA/docker-volumes/${SNAPCRAFT_PROJECT_NAME}

```

## 2.2 docker-compose.yml

| Item | Content | Optional / Mandatory |
| --- | --- | --- |
| version | version of the docker-compose | Mandatory |
| services | information about the docker images, like image name, container name, used ports, required volumes etc. | Mandatory |
| volumes | mounted volumes | Optional |
| ports | described ports are not conflicting with standard/blocked ports. and app uses only described ports | Optional |

Note: For example, here you can find information about syntax of a compose file: <https://github.com/compose-spec/compose-spec/blob/master/spec.md>

## 2.3 docker-compose.env

This file is optional and provides all environment variables which are used in the docker-compose.yml

| Item | Content | Optional / Mandatory |
| --- | --- | --- |
| variables | All variables in docker-compose.yml are provided here | Optional |
