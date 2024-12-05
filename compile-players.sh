#!/usr/bin/env bash

# **NOTE**: The below flag causes the Bash script to exit whenever a command fails.

set -e

# Compile each individual player to a shared object.

g++ -shared -fPIC -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./random_player.cxx" -o "./random_player.so"

g++ -shared -fPIC -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./first_opening_player.cxx" -o "./first_opening_player.so"

g++ -shared -fPIC -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./edging_player.cxx" -o "./edging_player.so"

g++ -shared -fPIC -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./mockery_player.cxx" -o "./mockery_player.so"

g++ -shared -fPIC -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./naive_eager_player.cxx" -o "./naive_eager_player.so"

g++ -shared -fPIC -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./OzgeAkosa5177_DimitriNearchosdon5092.cxx" -o "./OzgeAkosa5177_DimitriNearchosdon5092.so"

g++ -shared -fPIC -ansi -pedantic -std=c++14 -Wall -g \
    "./board.cxx" \
    "./snake_oil_player.cxx" -o "./snake_oil_player.so"
