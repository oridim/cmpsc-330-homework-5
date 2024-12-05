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
    Board board;         // Board instance to manage game state
    Loc *emptylines;     // Array to store empty lines
    int emptylines_cnt;  // Count of empty lines
    char player_box;     // Player's box character
    char player_line;    // Player's line character
    char opponent_line;  // Opponent's line character

    Loc FindScoringMove();       // Finds a move to complete a box
    Loc FindSafeMove();          // Finds a move that avoids risky setups
    Loc FindDisruptiveMove();    // Finds a move to disrupt the opponent's plans
    void ListEmptyLines();       // Updates the list of all available empty lines


public:
    // Constructor and Destructor
    OzgeAkosa5177_DimitriNearchosdon5092_Player();
    ~OzgeAkosa5177_DimitriNearchosdon5092_Player();

    // Player metadata
    string PlayerInfo() { return "Ozge Selin Ak (osa5177@psu.edu) + Dimitri Orion Nearchos (don5092@psu.edu)"; }

    // Game lifecycle functions
    void Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line);
    void Close();

    // Event handling functions
    void EventAddLine(const char bar, const Loc &loc);
    void EventAddBox(const char box, const Loc &loc);

    // Move selection function
    Loc SelectLineLocation();
};

#endif