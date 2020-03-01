#!/bin/bash

#cd src/external/PiScan-protobuf

protoc -I=src/external/PiScan-protobuf --cpp_out=src/external/PiScan-protobuf *.proto

#cd ../../..
