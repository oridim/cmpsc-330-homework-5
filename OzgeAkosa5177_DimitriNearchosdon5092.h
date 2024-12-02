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
    char opponent_line; // Line character for the opponent.
    vector<Loc> highPriorityLines; // Strategic vector
    vector<Loc> lowRiskLines;
    vector<Loc> neutralLines;
    vector<Loc> delayedMoves; // Moves to delay to avoid triggering chains too early.

    void CategorizeMoves(); // Categorize moves into high-priority, low-risk, and neutral.
    int EvaluateBoardState(Board &currentBoard) const;
    int EvaluateMove(const Loc &loc);                  // Evaluates a move's strategic value.
    int SimulateMove(const Loc &loc);
    int EvaluateFutureMoves(Board &currentBoard, int depth); // Recursively evaluate move outcomes.
    int SimulateMoveAndEvaluate(Board &currentBoard, const Loc &loc, int depth); // Evaluate a specific move.
    bool CreatesChainForOpp(const Loc &loc);
    bool CreatesDoubleCross(const Loc &loc); // Checks if a move creates a double-cross.
    int Minimax(int depth, bool isMaximizing, int alpha, int beta);
    int EvaluateBoard(); 
    int PredictOpponentMove(const Loc &loc);   
    bool CanControlChains();  
    bool HandleChains();             // Handle chain management in the endgame. 
    int SimulateChainLength(const Loc &start); // Calculate the length of a chain starting from a location.
    Loc NextChainLocation(const Loc &current); // Find the next location in a chain.             

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