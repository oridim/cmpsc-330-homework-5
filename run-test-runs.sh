#!/usr/bin/env bash

# **NOTE**: The below flag causes the Bash script to exit whenever a command fails.

set -e

# Ours vs. First Opening Player

# **NOTE**: We are sending 3 rows and 5 columns to the stdin of the game engine.

echo "**************************************************"
echo "Ours vs. First Opening Player"
echo "**************************************************"

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./first_opening_player.so <<< "3 5"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./first_opening_player.so <<< "4 6"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./first_opening_player.so <<< "5 7"

# Ours vs. Edging Player

echo "**************************************************"
echo "Ours vs. Edging Player"
echo "**************************************************"

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./edging_player.so <<< "3 5"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./edging_player.so <<< "4 6"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./edging_player.so <<< "5 7"

# Ours vs. Mockery Player

echo "**************************************************"
echo "Ours vs. Mockery Player"
echo "**************************************************"

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./mockery_player.so <<< "3 5"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./mockery_player.so <<< "4 6"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./mockery_player.so <<< "5 7"

# Ours vs. Naive Eager Player

echo "**************************************************"
echo "Ours vs. Naive Eager Player"
echo "**************************************************"

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./naive_eager_player.so <<< "3 5"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./naive_eager_player.so <<< "4 6"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./naive_eager_player.so <<< "5 7"

# Ours vs. Random Player

echo "**************************************************"
echo "Ours vs. Random Player"
echo "**************************************************"

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./random_player.so <<< "3 5"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./random_player.so <<< "4 6"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./random_player.so <<< "5 7"

# Ours vs. Strategic Player

echo "**************************************************"
echo "Ours vs. Strategic Player"
echo "**************************************************"

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "3 5"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "4 6"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./strategic_player.so <<< "5 7"

# Ours vs. Random Player vs. Strategic Player

echo "**************************************************"
echo "Ours vs. Random Player vs. Strategic Player"
echo "**************************************************"

./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./random_player.so ./strategic_player.so <<< "3 5"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./random_player.so ./strategic_player.so <<< "4 6"
./dotsboxes ./OzgeAkosa5177_DimitriNearchosdon5092.so ./random_player.so ./strategic_player.so <<< "5 7"

# Strategic Player vs. Naive Eager Player

echo "**************************************************"
echo "Strategic Player vs. Naive Eager Player"
echo "**************************************************"

./dotsboxes ./strategic_player.so ./naive_eager_player.so <<< "3 5"
./dotsboxes ./strategic_player.so ./naive_eager_player.so <<< "4 6"
./dotsboxes ./strategic_player.so ./naive_eager_player.so <<< "5 7"