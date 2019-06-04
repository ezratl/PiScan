#!/bin/sh

cd src/external/PiScan-protobuf

protoc -I=. --cpp_out=. *.proto

cd ../../..
