#ifndef __RANDOM_PLAYER__
#define __RANDOM_PLAYER__

#include <iostream>
#include "common.h"
#include "player.h"
#include "board.h"
#include <vector>
using namespace std;

class OzgeAkosa5177_DimitriNearchosdon5092_Player : public IPlayer
{
private:
    Board board;
    char player_box;
    char player_line;
    Loc *emptylines;
    int emptylines_cnt;

    vector<Loc> highPriorityLines; // Moves that complete a box.
    vector<Loc> lowRiskLines;     // Moves that don't create risky situations.
    vector<Loc> neutralLines;     // Other valid moves.

public:
    OzgeAkosa5177_DimitriNearchosdon5092_Player();

    ~OzgeAkosa5177_DimitriNearchosdon5092_Player();

    string PlayerInfo() { return "Ozge Selin Ak (osa5177@psu.edu) + Dimitri Orion Nearchos (don5092@psu.edu)"; }

    void Init(int dots_in_rows, int dots_in_cols, char player_box, char player_line);

    // Close() will be called after finishing playing the game
    // You can remove all dynamically allocated memories
    void Close();

    void EventAddLine(char bar, const Loc &loc);

    void EventAddBox(char box, const Loc &loc);

    Loc SelectLineLocation();

    void ListEmptyLines();
};

#endif
