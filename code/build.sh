#!/bin/bash
if [ ! -d ../build ]; then
	mkdir ../build
fi

pushd ../build

gcc ../code/main.c -g -O0 -lc

popd
