# SocketCAN FD Project

## Overview

This project is a C++ implementation designed to interact with CAN FD (Controller Area Network Flexible Data-Rate) interfaces on Linux systems using the SocketCAN API. It provides utilities for reading from and writing to CAN FD interfaces, with support for custom configurations loaded from a YAML file.

## Features
* Configurable CAN Interface: The CAN interface name and filter IDs can be specified in a YAML configuration file.
* CAN FD Socket Communication: The project handles the setup of CAN FD sockets, including enabling CAN FD frames, binding to the specified CAN interface, and applying filters.
* Interactive Mode: Provides interactive utilities to send custom CAN FD frames.
* Real-time Reading: Continuously reads CAN FD frames and prints their details to the console.

## Build Instructions

1. <strong>Install Dependencies: </strong>Ensure you have the required libraries installed:

* <strong> YAML-CPP: </strong> For parsing the YAML configuration file.
* <strong> libsocketcan: </strong> If needed for additional SocketCAN utilities.
* <strong> CMake: </strong> For building the project.

On Ubuntu, you can install these dependencies with:

``` 
sudo apt-get install libyaml-cpp-dev libsocketcan-dev cmake g++ 
```

2. <strong> Clone the Repository: </strong>
``` 
git clone https://github.com/CelilSincanli/socketcanfd.git
cd socketcanfd
``` 
3. <strong> Build the Project: </strong>  Create a build directory and run CMake to configure the build, then compile:
``` 
mkdir build
cd build
cmake ..
make
``` 
This will generate the executables socketcanfd_read and socketcanfd_write in the build/src directory.

## Usage
### Setup CAN FD Interface
Before running the CAN FD reader or writer, you need to set up the CAN interface with the appropriate bitrate, enable CAN FD, and configure the MTU size. The following instructions show how to set up the interface for different vendors:

<strong> Using PEAK USB Adapter </strong>

1. Load the PEAK USB driver:
```
sudo modprobe peak_usb
```
2. Set the MTU size and bring up the CAN interface:
```
sudo ip link set can0 mtu 72
sudo ip link set can0 up type can bitrate 500000 dbitrate 2000000 fd on

```

<strong> Using Kvaser USB Adapter </strong>
1. Load the Kvaser USB driver (if not already loaded):
```
sudo modprobe kvaser_usb

```
2. Set the MTU size and bring up the CAN interface:
```
sudo ip link set can0 mtu 72
sudo ip link set can0 up type can bitrate 500000 dbitrate 2000000 fd on

```

### Running the Reader

The reader application continuously reads CAN FD frames from the configured CAN interface and prints the frame data.

To start the CAN FD reader, run:

``` 
./build/src/socketcanfd_read
``` 

This will read and print CAN FD messages from the configured CAN interface (can0 by default).

### Running the Writer

The writer application allows interactive sending of CAN FD frames. It prompts for CAN ID, flags, and data to be sent.

To start the CAN FD writer, run:

``` 
./build/src/socketcanfd_write
``` 

This will allow you to interactively enter CAN ID, flags, and data to send as a CAN FD message.

### Configuration

The CAN FD configuration is loaded from the config/canfd_config.yaml file. This file allows you to specify:

* default_can_interface_name: The name of the CAN interface to use (e.g., can0).
* default_can_filter_ids: A list of CAN IDs to filter incoming messages.

``` 
default_can_interface_name: can0
default_can_filter_ids:
  - 1536
  - 1537
  # Add more filter IDs as needed

``` 