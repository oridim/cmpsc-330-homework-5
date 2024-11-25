#!/usr/bin/env bash

# **NOTE**: The below flag causes the Bash script to exit whenever a command fails.

set -e

# Ours vs. Random Player

# **NOTE**: We are sending 3 rows and 5 columns to the stdin of the game engine.

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./random_player.so <<< "3 5"

# Ours vs. Strategic Player

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "3 5"

# Ours vs. Random Player vs. Strategic Player

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./random_player.so ./strategic_player.so <<< "3 5"