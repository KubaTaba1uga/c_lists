#!/bin/bash

builddir="$1"
enable_tests="$2"

if [ -z "$1" ]
then
    builddir="build"
fi

if [ -z "$2" ]
then
    enable_tests="true"
fi

meson setup $builddir -Denable_tests=$enable_tests
