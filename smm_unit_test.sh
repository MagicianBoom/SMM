#!/bin/bash

set -e

GTEST_ROOT_DIR="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
BUILD_DIR=$GTEST_ROOT_DIR/build

function build_run_test()
{
    local target_file=$GTEST_ROOT_DIR/*

    if [ ! -d "$BUILD_DIR" ]; then
        mkdir $BUILD_DIR
    fi

    cd ${GTEST_ROOT_DIR}
    g++ ${target_file}/*.c -lgtest -lpthread -o $BUILD_DIR/gtest

    sleep 1s

    cd $BUILD_DIR
    set +e
    ./gtest
    set -e
}

function clean()
{
    cd ${BUILD_DIR}
    if [ -f "${BUILD_DIR}/gtest" ]; then
        rm ${BUILD_DIR}/gtest
    fi
}

OPS_TARGET=$1

case "$OPS_TARGET" in
test)
    build_run_test
    ;;
clean)
    clean
    ;;
*)
    echo "USAGE:"
    echo "Clean test project:   smm_unit_test.sh test"
    echo "Test smm:             smm_unit_test.sh clean"
    ;;
esac

