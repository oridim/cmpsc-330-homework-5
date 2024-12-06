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
    char player_box;     // Player's box character
    char player_line;    // Player's line character
    int board_rows;      // Number of rows in the board
    int board_cols;      // Number of columns in the board
    queue<Loc> priorityMoves;


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

    int EvaluateMove(const Loc &move);

};

#endif
