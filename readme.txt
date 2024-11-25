Build mazerunner program and player_randommove.so library

g++ -ansi -pedantic -std=c++14  board.cxx  dotsboxesgm.cxx  main.cxx  -o dotsboxes
g++ -shared -fPIC -ansi -pedantic -std=c++14  random_player.cxx  board.cxx     -o random_player.so


playing the dots and boxes games with multiple players

./dotsboxes  ./strategic_player.so  ./random_player.so  ./strategic_player.so  ./random_player.so

