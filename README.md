
# IMS - Icecast Metadata Sender

  

Simple service for sending metadata to icecast server. IMS opens UDP socket, similarly to Opticodec-PC Streaming Encoder ([see more here](ftp://ftp.orban.com/1511/Opticodec_1010_Encoder/Documentation/PAD_Metadata/1010.3.7_PAD.pdf)) and sends *artist* and *title* of song to Icecast.

  

[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg)](https://github.com/dgalus/ims/graphs/commit-activity) [![GitHub license](https://img.shields.io/github/license/Naereen/StrapDown.js.svg)](https://github.com/dgalus/ims/blob/main/LICENSE) [![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/dgalus/ims)

## Dependencies

- gcc
- make
- libconfig-devel
- libcurl-devel

In Fedora you can install dependencies with the following command:

```sudo dnf install gcc libconfig-devel libcurl-devel```

## Compilation

1. Install dependencies.
2. Go to *ims* directory

```cd ims```

3. Build application by typing

```make```
## Configuration file

Example config file is in: *src/config.ini.sample*. Comments are in the file.

## Usage

```./bin/ims --config config.ini```

## Run as docker container

Build the image:

```sudo docker build -t ims .```

Create or copy and edit configuration file from *src/config.ini.sample*.

Run docker container with the following command.

```sudo docker run -d [-p <PORT>:<PORT>] --name <CONTAINER_NAME> [--network=host] --privileged --restart=always -v <YOUR_CONFIG_FILE_PATH>:/data/config.ini ims:latest```

Replace *PORT* with port number specified in configuration file, *CONTAINER_NAME* with preffered container name, and *YOUR_CONFIG_FILE_PATH* with your config file absolute path.

Arguents in *[ ]* are optional. When *--network=host* is used, option *-p PORT:PORT* is omitted.