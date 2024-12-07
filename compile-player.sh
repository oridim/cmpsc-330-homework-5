#!/usr/bin/env bash

# **NOTE**: The below flag causes the Bash script to exit whenever a command fails.

set -e

# Compile only the osadon player to a shared object.

g++ -shared -fPIC -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./logic_helper.cxx" \
    "./OzgeAkosa5177_DimitriNearchosdon5092.cxx" -o "./OzgeAkosa5177_DimitriNearchosdon5092.so"