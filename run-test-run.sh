#!/usr/bin/env bash

# **NOTE**: The below flag causes the Bash script to exit whenever a command fails.

set -e

# Ours vs. Strategic Player

# **NOTE**: We are sending 3 rows and 5 columns to the stdin of the game engine.

echo "**************************************************"
echo "Ours vs. Strategic Player"
echo "**************************************************"

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "3 5"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "4 6"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "5 7"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "6 8"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "7 9"