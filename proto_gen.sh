#!/bin/bash

cd ./proto

protoc -cpp_out=. *.proto
