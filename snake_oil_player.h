#ifndef __SNAKE_OIL_PLAYER__
#define __SNAKE_OIL_PLAYER__

#include <iostream>
#include <vector>

#include "common.h"
#include "board.h"
#include "player.h"

using namespace std;

class SnakeOilPlayer : public IPlayer
{
private:
    Board board;
    char player_box;
    char player_line;

public:
    SnakeOilPlayer();

    ~SnakeOilPlayer();

    string PlayerInfo() { return "snake oil player"; }

    void Init(int dots_in_rows, int dots_in_cols, char player_box, char player_line);

    void Close();

    void EventAddLine(char bar, const Loc &loc);

    void EventAddBox(char box, const Loc &loc);

    vector<Loc> ComputeLegalMoves();

    vector<Loc> ComputePayoffMoves();

    vector<Loc> ComputeSnakeMoves();

    bool DoesMoveChainLine(const Loc &loc);

    bool DoesMovePayoff(const Loc &loc);

    bool DoesMoveSell(const Loc &loc);

    Loc SelectLineLocation();
};

#endif
