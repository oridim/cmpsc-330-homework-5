#ifndef __RANDOM_PLAYER__
#define __RANDOM_PLAYER__

#include "board.h"
#include "common.h"

#include <vector>
#include "player.h"
#include "board.h"
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

    void Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line);
    void Close();

    void EventAddLine(const char bar, const Loc &loc);
    void EventAddBox(const char box, const Loc &loc);
    Loc SelectLineLocation();
    void ListEmptyLines();
    virtual const char *PlayerInfo() const = 0;
    const char *PlayerInfo() const override; 
};

#endif