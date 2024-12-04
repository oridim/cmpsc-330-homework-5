#ifndef __MOCKERY_PLAYER__
#define __MOCKERY_PLAYER__

#include <iostream>

#include "common.h"
#include "board.h"
#include "player.h"

using namespace std;

class MockeryPlayer : public IPlayer
{
private:
    Board board;
    char player_box;
    char player_line;

    Loc last_move;

public:
    MockeryPlayer();

    ~MockeryPlayer();

    string PlayerInfo() { return "mockery player"; }

    void Init(int dots_in_rows, int dots_in_cols, char player_box, char player_line);

    void Close();

    void EventAddLine(char bar, const Loc &loc);

    void EventAddBox(char box, const Loc &loc);

    Loc SelectLineLocation();
};

#endif
