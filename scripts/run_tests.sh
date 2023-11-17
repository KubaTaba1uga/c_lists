#!/bin/bash

builddir="$1"
test_name="$2"

if [ -z "$1" ]
then
    builddir="build"
fi

meson test $test_name -C $builddir

success=$(!?)

cat $builddir/meson-logs/testlog.txt

exit $success
