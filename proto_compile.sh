#! /bin/bash
SRC_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DST_DIR=$SRC_DIR/gen
DST_PY_DIR=$SRC_DIR/py/gen

protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/query.proto
protoc -I=$SRC_DIR --grpc_out=$DST_DIR --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $SRC_DIR/query.proto

protoc -I=$SRC_DIR --python_out=$DST_PY_DIR --grpc_out=$DST_PY_DIR --plugin=protoc-gen-grpc=`which grpc_python_plugin` $SRC_DIR/query.proto