#!/bin/bash

cd ./proto

protoc --cpp_out=. *.proto
protoc --python_out=. *.proto
