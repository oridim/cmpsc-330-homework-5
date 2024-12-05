#ifndef __EDGING_PLAYER__
#define __EDGING_PLAYER__

#include <iostream>
#include <vector>

#include "common.h"
#include "board.h"
#include "player.h"

using namespace std;

class EdgingPlayer : public IPlayer
{
private:
    Board board;
    char player_box;
    char player_line;

public:
    EdgingPlayer();

    ~EdgingPlayer();

    string PlayerInfo() { return "edging player"; }

    void Init(int dots_in_rows, int dots_in_cols, char player_box, char player_line);

    void Close();

    void EventAddLine(char bar, const Loc &loc);

    void EventAddBox(char box, const Loc &loc);

    vector<Loc> ComputeLegalEdgeMoves();

    vector<Loc> ComputePayoffMoves();

    Loc SelectLineLocation();
};

#endif
