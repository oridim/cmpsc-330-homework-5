#ifndef __FIRST_OPENING_PLAYER__
#define __FIRST_OPENING_PLAYER__

#include <iostream>
#include "common.h"
#include "player.h"
#include "board.h"
using namespace std;

class FirstOpeningPlayer : public IPlayer
{
private:
    Board board;
    char player_box;
    char player_line;

public:
    FirstOpeningPlayer();

    ~FirstOpeningPlayer();

    string PlayerInfo() { return "first opening player"; }

    void Init(int dots_in_rows, int dots_in_cols, char player_box, char player_line);

    void Close();

    void EventAddLine(char bar, const Loc &loc);

    void EventAddBox(char box, const Loc &loc);

    Loc SelectLineLocation();
};

#endif
