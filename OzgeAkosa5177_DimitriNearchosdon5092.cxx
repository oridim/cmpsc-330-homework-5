#include <assert.h>
#include <cstdlib> // for srand and rand
#include <ctime>   // for time
#include <iostream>
#include <vector>
#include <algorithm>

#include "common.h"
#include "board.h"
#include "player.h"

#include "OzgeAkosa5177_DimitriNearchosdon5092.h"

using namespace std;

extern "C" IPlayer *PlayerFactory()
{
    return new OzgeAkosa5177_DimitriNearchosdon5092_Player();
}

OzgeAkosa5177_DimitriNearchosdon5092_Player::OzgeAkosa5177_DimitriNearchosdon5092_Player()
{
    // **TODO**: Put bootstrap code here that is not related directly to the
    // game engine.
    //
    // Put that stuff in `.Init`.

    srand(time(0));
    emptylines = nullptr;
    emptylines_cnt = 0;
    isBoardAllocated = false; // Initialize the flag.
}

OzgeAkosa5177_DimitriNearchosdon5092_Player::~OzgeAkosa5177_DimitriNearchosdon5092_Player()
{
    // **TODO**: Put deallocations of anything you allocated here that is not
    // directly related to the game engine.
    //
    // Put that stuff `.Close()`.
    Close();
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);
    player_box = _player_box;
    player_line = _player_line;
    opponent_line = (player_line == 'A') ? 'B' : 'A';
    emptylines = new Loc[board.GetRows() * board.GetCols()];

    isBoardAllocated = true; // Mark the board as allocated.
}


void OzgeAkosa5177_DimitriNearchosdon5092_Player::Close()
{
    // **TODO**: Deallocate anything you use to track the game / player state here.

    board.FreeBoard();
if (emptylines)
{
    delete[] emptylines;
    emptylines = nullptr;
}
 isBoardAllocated = false; // Mark the board as deallocated.
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::EventAddLine(const char bar, const Loc &loc)
{
    assert(loc.IsLineLocation());
    assert(board(loc) == ' ');

    board(loc) = bar;
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::EventAddBox(const char box, const Loc &loc)
{
    assert(loc.IsBoxLocation());
    assert(board(loc) == ' ');

    board(loc) = box;
}

bool OzgeAkosa5177_DimitriNearchosdon5092_Player::CreatesDoubleCross(const Loc &loc) {
    board(loc) = player_line; // Simulate move
    int doubleCrossCount = 0;

    for (int row = 1; row < board.GetRows(); row += 2) {
        for (int col = 1; col < board.GetCols(); col += 2) {
            if (board.CountSurroundingLines(row, col) == 3) {
                doubleCrossCount++;
            }
        }
    }

    board(loc) = ' '; // Undo move
    return doubleCrossCount >= 2;
}

Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::SelectLineLocation()
{
    ListEmptyLines(); // Populate empty lines.
    CategorizeMoves(); // Categorize moves into high-priority, low-risk, neutral, and delayed.
    bool isEndgame = emptylines_cnt < 10;
    bool isMidgame = (emptylines_cnt >= 10 && emptylines_cnt <= 20);
    int depth = isEndgame ? 5 : (isMidgame ? 4 : 2); // Dynamically adjust depth.


    //High-priority moves: Complete a box.
    if (!highPriorityLines.empty())
        return highPriorityLines[0];

    //Midgame: Avoid creating chains.
    if (!lowRiskLines.empty())
        return lowRiskLines[0];

    //Endgame: Control chains.
    if (emptylines_cnt < 10)
    {
        for (const Loc &loc : highPriorityLines)
        {
            if (CreatesDoubleCross(loc))
                return loc;
        }
    }

    //Minimax fallback: Evaluate best move.
    int bestValue = -100000;
    Loc bestMove;

    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc loc = emptylines[i];
        if (board(loc) == ' ')
        {
            board(loc) = player_line; // Simulate AI's move.
            int moveValue = Minimax(depth, false, isEndgame, isMidgame); // Pass the values
            if (moveValue > bestValue)
            {
                bestValue = moveValue;
                bestMove = loc;
            }
            board(loc) = ' '; // Undo simulation.
        }
    }
    if (emptylines_cnt < 10 && HandleChains()) {
        // Endgame: prioritize moves that maximize chain control.
        for (const Loc &loc : lowRiskLines) 
        {
            if (!CreatesChainForOpp(loc)) 
            {
                return loc;
          }
        }
    }

    if (bestValue > -100000)
        return bestMove;

    //Fallback: Random move.
    if (emptylines_cnt > 0)
    {
        int randloc = rand() % emptylines_cnt;
        return emptylines[randloc];
    }

    return {0, 0}; // Default fallback.
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::Minimax(int depth, bool isMaximizing, int alpha, int beta) {
    if (depth == 0 || emptylines_cnt == 0) {
        return EvaluateBoard(); // Base case: evaluate the current board state.
    }

    if (isMaximizing) {
        int maxEval = -100000;

        for (int i = 0; i < emptylines_cnt; i++) {
            Loc loc = emptylines[i]; // Define loc here.
            if (board(loc) == ' ') {
                board(loc) = player_line; // Simulate move
                int value = Minimax(depth - 1, false, alpha, beta);

                // Add opponent prediction.
                if (PredictOpponentMove(loc) > value) {
                    value -= 10; // Penalize risky moves.
                }

                board(loc) = ' '; // Undo move
                maxEval = max(maxEval, value);
                alpha = max(alpha, value);
                if (beta <= alpha) break; // Prune
            }
        }
        return maxEval;
    } else {
        int minEval = 100000;

        for (int i = 0; i < emptylines_cnt; i++) {
            Loc loc = emptylines[i]; // Define loc here.
            if (board(loc) == ' ') {
                board(loc) = opponent_line; // Simulate opponent's move
                int value = Minimax(depth - 1, true, alpha, beta);

                board(loc) = ' '; // Undo move
                minEval = min(minEval, value);
                beta = min(beta, value);
                if (beta <= alpha) break; // Prune
            }
        }
        return minEval;
    }
}


bool OzgeAkosa5177_DimitriNearchosdon5092_Player::CreatesChainForOpp(const Loc &loc)
{
    // Simulate adding the line.
    board(loc) = player_line;

    // Check if completing this line opens a chain.
    int opponentChains = 0;
    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if (board.CountSurroundingLines(row, col) == 2)
            {
                opponentChains++;
            }
        }
    }

    // Undo the simulated line addition.
    board(loc) = ' ';

    // If multiple chains are opened, this is bad.
    return opponentChains > 1;
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::ListEmptyLines() {
    // Check if the board is allocated before proceeding.
    if (!isBoardAllocated) {
        cerr << "Error: Board is not allocated in ListEmptyLines!" << endl;
        return; // Safely exit if the board is not ready.
    }

    // Clear all strategic line categories.
    highPriorityLines.clear();
    lowRiskLines.clear();
    neutralLines.clear();
    emptylines_cnt = 0; // Reset count of available lines.

    // Iterate through the board rows and columns.
    for (int row = 0; row < board.GetRows(); row++) {
        for (int col = 0; col < board.GetCols(); col++) {
            // Skip invalid locations (dots and box centers).
            if ((row % 2 == 0 && col % 2 == 0) || (row % 2 == 1 && col % 2 == 1)) {
                continue; // Dots and box centers are not valid line locations.
            }

            // Check if the current location is empty.
            if (board(row, col) == ' ') {
                int surroundingLines = board.CountSurroundingLines(row, col);

                // Categorize the move based on the surrounding lines.
                if (surroundingLines == 3) {
                    highPriorityLines.push_back({row, col}); // High-priority move.
                } else if (surroundingLines <= 1) {
                    lowRiskLines.push_back({row, col}); // Low-risk move.
                } else {
                    neutralLines.push_back({row, col}); // Neutral or delayed move.
                }

                // Add the location to the list of empty lines.
                if (emptylines_cnt < board.GetRows() * board.GetCols()) {
                    emptylines[emptylines_cnt].row = row;
                    emptylines[emptylines_cnt].col = col;
                    emptylines_cnt++;
                } else {
                    cerr << "Warning: Too many empty lines detected in ListEmptyLines!" << endl;
                }
            }
        }
    }
}


int OzgeAkosa5177_DimitriNearchosdon5092_Player::EvaluateMove(const Loc &loc)
{
    if (CreatesChainForOpp(loc))
    {
        return -1000; // Penalize moves that create chains for the opponent.
    }

    return SimulateMove(loc); // Reward moves based on the number of boxes gained.
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::EvaluateFutureMoves(Board &currentBoard, int depth)
{
    if (depth == 0)
    {
        return 0; // No further evaluation if depth is 0.
    }

    int maxGain = -1000; // Initialize with a very low value.

    // Loop through all possible moves.
    for (int row = 0; row < currentBoard.GetRows(); row++)
    {
        for (int col = 0; col < currentBoard.GetCols(); col++)
        {
            // If the location is empty and is a valid line location.
            if (currentBoard(row, col) == ' ' && Loc(row, col).IsLineLocation())
            {
                Loc loc(row, col);
                // Simulate the move and evaluate it recursively.
                int gain = SimulateMoveAndEvaluate(currentBoard, loc, depth);

                // Track the best gain.
                if (gain > maxGain)
                {
                    maxGain = gain;
                }
            }
        }
    }

    return maxGain; // Return the best gain found.
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::EvaluateBoardState(Board &currentBoard) const
{
    int myScore = 0, opponentScore = 0;

    for (int row = 0; row < currentBoard.GetRows(); row++)
    {
        for (int col = 0; col < currentBoard.GetCols(); col++)
        {
            if (currentBoard.CountSurroundingLines(row, col) == 4)
            {
                // If it's your box, add to your score.
                if (currentBoard(row, col) == player_box)
                {
                    myScore++;
                }
                else
                {
                    opponentScore++;
                }
            }
        }
    }

    return myScore - opponentScore; // Return the net score difference.
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::EvaluateBoard() {
    int aiBoxes = 0, opponentBoxes = 0, chains = 0, opponentChains = 0, chainScore = 0;

    for (int row = 1; row < board.GetRows(); row += 2) {
        for (int col = 1; col < board.GetCols(); col += 2) {
            int surroundingLines = board.CountSurroundingLines(row, col);
            if (surroundingLines == 4) {
                if (board(row, col) == player_box)
                    aiBoxes++;
                else if (board(row, col) == opponent_line)
                    opponentBoxes++;
            } else if (surroundingLines == 2) {
                chains++;
                chainScore += SimulateChainLength({row, col});
            } else if (surroundingLines == 3) {
                opponentChains++;
            }
        }
    }

    // Heuristic scoring with chain control and risk management.
    return (aiBoxes - opponentBoxes) * 20 - opponentChains * 10 + chains * 5 + chainScore * 2;
}


bool OzgeAkosa5177_DimitriNearchosdon5092_Player::CanControlChains() {
    int chainsControlled = 0;
    int totalChains = 0;

    for (int row = 0; row < board.GetRows(); row++) {
        for (int col = 0; col < board.GetCols(); col++) {
            if (board.CountSurroundingLines(row, col) == 2) {
                totalChains++;
                if (!CreatesChainForOpp({row, col})) {
                    chainsControlled++;
                }
            }
        }
    }

    return chainsControlled >= totalChains / 2; // Control at least half the chains.
}

bool OzgeAkosa5177_DimitriNearchosdon5092_Player::HandleChains() {
    int shortChains = 0;
    int longChains = 0;

    for (int row = 1; row < board.GetRows(); row += 2) {
        for (int col = 1; col < board.GetCols(); col += 2) {
            if (board.CountSurroundingLines(row, col) == 2) {
                // Check chain length
                int chainLength = SimulateChainLength({row, col});
                if (chainLength <= 3) {
                    shortChains++;
                } else {
                    longChains++;
                }
            }
        }
    }

    // Favor long chains in endgame.
    return longChains > shortChains;
}
int OzgeAkosa5177_DimitriNearchosdon5092_Player::SimulateChainLength(const Loc &start) {
    if (!isBoardAllocated) {
        cerr << "Error: Board not allocated in SimulateChainLength!" << endl;
        return 0;
    }

    int chainLength = 0;
    Loc current = start;

    while (board.CountSurroundingLines(current.row, current.col) == 2) {
        Loc next = NextChainLocation(current);
        if (next.row == current.row && next.col == current.col) {
            break;
        }
        chainLength++;
        current = next;

        if (current.row < 0 || current.row >= board.GetRows() || current.col < 0 || current.col >= board.GetCols()) {
            break;
        }
    }

    return chainLength;
}


Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::NextChainLocation(const Loc &current) {
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr != 0 || dc != 0) {
                Loc neighbor(current.row + dr, current.col + dc);

                // Add bounds checking.
                if (neighbor.row >= 0 && neighbor.row < board.GetRows() &&
                    neighbor.col >= 0 && neighbor.col < board.GetCols() &&
                    board.CountSurroundingLines(neighbor.row, neighbor.col) == 2) {
                    return neighbor;
                }
            }
        }
    }
    return current; // Return the current location if no valid neighbor is found.
}


void OzgeAkosa5177_DimitriNearchosdon5092_Player::CategorizeMoves() {
    highPriorityLines.clear();
    lowRiskLines.clear();
    neutralLines.clear();
    delayedMoves.clear();

    emptylines_cnt = 0; // Reset count of available lines.

    for (int row = 0; row < board.GetRows(); row++) {
        for (int col = (row % 2 == 0 ? 1 : 0); col < board.GetCols(); col += 2) { 
            // Check only valid line positions
            if (board(row, col) == ' ') {
                Loc loc(row, col);
                int surroundingLines = board.CountSurroundingLines(row, col);

                if (surroundingLines == 3 && !CreatesChainForOpp(loc)) {
                    highPriorityLines.push_back(loc); // Complete a box
                } else if (surroundingLines <= 1) {
                    lowRiskLines.push_back(loc); // Safe moves
                } else if (surroundingLines == 2 && !CreatesDoubleCross(loc)) {
                    neutralLines.push_back(loc); // Neutral moves
                } else {
                    delayedMoves.push_back(loc); // Risky moves
                }
                if (surroundingLines == 3 && !CreatesDoubleCross(loc)) {
                    highPriorityLines.push_back(loc); // Avoid double cross scenarios.
                }

                emptylines[emptylines_cnt++] = loc; // Add to generic list of empty lines
            }
        }
    }
}

