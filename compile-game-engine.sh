#!/usr/bin/env bash

# **NOTE**: The below flag causes the Bash script to exit whenever a command fails.

set -e

# Compile each individual player to a shared object.

g++ -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./dotsboxesgm.cxx" \
    "./main.cxx" -o "./dotsboxes"
