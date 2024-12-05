#ifndef __NAIVE_EAGER_PLAYER__
#define __NAIVE_EAGER_PLAYER__

#include <iostream>

#include "common.h"
#include "board.h"
#include "player.h"

using namespace std;

class NaiveEagerPlayer : public IPlayer
{
private:
    Board board;
    char player_box;
    char player_line;

public:
    NaiveEagerPlayer();

    ~NaiveEagerPlayer();

    string PlayerInfo() { return "naive eager player"; }

    void Init(int dots_in_rows, int dots_in_cols, char player_box, char player_line);

    void Close();

    void EventAddLine(char bar, const Loc &loc);

    void EventAddBox(char box, const Loc &loc);

    int CountLinesAroundSlot(const Loc &loc);

    bool DoesMovePayoff(const Loc &loc);

    bool DoesMoveSell(const Loc &loc);

    Loc SelectLineLocation();
};

#endif
