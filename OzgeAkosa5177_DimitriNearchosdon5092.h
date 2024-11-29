#ifndef __OZGEAKOSA5177_DIMITRINEARCHOSDON5092_PLAYER__
#define __OZGEAKOSA5177_DIMITRINEARCHOSDON5092_PLAYER__

#include <vector>

#include "common.h"
#include "board.h"

#include "player.h"

using namespace std;

class OzgeAkosa5177_DimitriNearchosdon5092_Player : public IPlayer
{
private:
    Board board;         // This is your Board instance.
    Loc *emptylines;     // Array to store empty lines.
    int emptylines_cnt;  // Count of empty lines.
    char player_box;         // The player's box character.
    char player_line;  
    vector<Loc> highPriorityLines; // Strategic vector
    vector<Loc> lowRiskLines;
    vector<Loc> neutralLines;

public:
    OzgeAkosa5177_DimitriNearchosdon5092_Player();
    ~OzgeAkosa5177_DimitriNearchosdon5092_Player();

    string PlayerInfo() { return "Ozge Selin Ak (osa5177@psu.edu) + Dimitri Orion Nearchos (don5092@psu.edu)"; }

    void Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line);
    void Close();

    void EventAddLine(const char bar, const Loc &loc);
    void EventAddBox(const char box, const Loc &loc);
    Loc SelectLineLocation();
    void ListEmptyLines();
};

#endif