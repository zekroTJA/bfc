#!/usr/bin/env bash

afl-fuzz -i inputs -o /output "$@" -- ./dist/bfc
