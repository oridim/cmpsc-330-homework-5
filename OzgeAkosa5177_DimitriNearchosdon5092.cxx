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
bool OzgeAkosa5177_DimitriNearchosdon5092_Player::CreatesDoubleCross(const Loc &loc)
{
    // Simulate adding the line.
    board(loc) = player_line;

    int doubleCrossCount = 0;

    // Check all boxes near the current line.
    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if (board.CountSurroundingLines(row, col) == 3) // Box with 3 sides filled.
            {
                doubleCrossCount++;
            }
        }
    }

    // Undo the move.
    board(loc) = ' ';

    // A double-cross happens when two boxes have 3 sides filled.
    return doubleCrossCount >= 2;
}


Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::SelectLineLocation()
{
    ListEmptyLines(); // Populate empty lines.
    CategorizeMoves(); // Categorize moves into high-priority, low-risk, neutral, and delayed.

    bool isEndgame = emptylines_cnt < 10; // Identify the endgame.
    bool isMidgame = (emptylines_cnt >= 10 && emptylines_cnt <= 20); // Identify midgame.

    // High-priority moves to complete a box.
    if (!highPriorityLines.empty())
        return highPriorityLines[0];

    // Strategic delaying moves in midgame (chains control).
    if (isMidgame && CanControlChains())
    {
        for (const Loc &loc : neutralLines)
        {
            if (!CreatesChainForOpp(loc))
                return loc;
        }
    }

    // Avoid creating chains with low-risk moves.
    if (!lowRiskLines.empty())
    {
        for (const Loc &loc : lowRiskLines)
        {
            if (!CreatesChainForOpp(loc))
                return loc;
        }
    }

    // Apply double-cross strategy during the endgame.
    if (isEndgame)
    {
        for (const Loc &loc : highPriorityLines)
        {
            if (CreatesDoubleCross(loc))
                return loc; // Prioritize double-cross moves.
        }
    }

    // Use Minimax to evaluate fallback moves.
    int bestValue = -100000;
    Loc bestMove;
    int depth = (isEndgame) ? 5 : (isMidgame ? 4 : 2); // Adjust depth dynamically.

    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc loc = emptylines[i];
        if (board(loc) == ' ') // Ensure move is valid.
        {
            board(loc) = player_line; // Simulate AI's move.

            int moveValue = Minimax(depth, false);
            if (moveValue > bestValue)
            {
                bestValue = moveValue;
                bestMove = loc;
            }

            board(loc) = ' '; // Undo simulation.
        }
    }

    if (bestValue > -100000)
        return bestMove;

    // Fallback to random move if all else fails.
    if (emptylines_cnt > 0)
    {
        int randloc = rand() % emptylines_cnt;
        return emptylines[randloc];
    }

    return {0, 0}; // Default fallback.
}



int OzgeAkosa5177_DimitriNearchosdon5092_Player::Minimax(int depth, bool isMaximizing)
{
    // Base case: Max depth or no moves left
    if (depth == 0 || emptylines_cnt == 0)
        return EvaluateBoard(); // Use heuristic evaluation function.

    int bestValue = isMaximizing ? -100000 : 100000;

    // Sort moves by their heuristic value to prioritize good moves early.
    vector<Loc> sortedMoves(emptylines, emptylines + emptylines_cnt);
    sort(sortedMoves.begin(), sortedMoves.end(), [this](const Loc &a, const Loc &b) {
        return EvaluateMove(a) > EvaluateMove(b); // Sort descending by EvaluateMove().
    });

    // Loop through sorted moves.
    for (const Loc &loc : sortedMoves)
    {
        if (board(loc) == ' ') // Ensure the move is valid.
        {
            // Simulate the move.
            board(loc) = isMaximizing ? player_line : opponent_line;

            // Recur with adjusted depth.
            int value = Minimax(depth - 1, !isMaximizing);

            // Undo the move.
            board(loc) = ' ';

            // Update the best value based on maximizing or minimizing.
            if (isMaximizing)
                bestValue = max(bestValue, value);
            else
                bestValue = min(bestValue, value);
        }
    }

    return bestValue;
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

void OzgeAkosa5177_DimitriNearchosdon5092_Player::ListEmptyLines()
{
    // Clear all strategic line categories.
    highPriorityLines.clear();
    lowRiskLines.clear();
    neutralLines.clear();
    emptylines_cnt = 0; // Reset count of available lines.

    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            // Skip irrelevant locations (dots and box centers).
            if ((row % 2 == 0 && col % 2 == 0) || (row % 2 == 1 && col % 2 == 1))
            {
                continue;
            }

            // If the location is empty, evaluate its value.
            if (board(row, col) == ' ')
            {
                int surroundingLines = board.CountSurroundingLines(row, col);

                if (surroundingLines == 3)
                {
                    // High-priority move (completes a box).
                    highPriorityLines.push_back({row, col});
                }
                else if (surroundingLines <= 1)
                {
                    // Low-risk move.
                    lowRiskLines.push_back({row, col});
                }
                else
                {
                    // Neutral or other cases.
                    neutralLines.push_back({row, col});
                }

                // Add to the generic list of empty lines.
                emptylines[emptylines_cnt].row = row;
                emptylines[emptylines_cnt].col = col;
                emptylines_cnt++;
            }
        }
    }
}
// Helper function: Evaluate a move's value.
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

int OzgeAkosa5177_DimitriNearchosdon5092_Player::EvaluateBoard()
{
    int aiBoxes = 0, opponentBoxes = 0, chains = 0, safeMoves = 0;

    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if (board.CountSurroundingLines(row, col) == 4)
            {
                if (board(row, col) == player_box)
                    aiBoxes++;
                else if (board(row, col) == opponent_line)
                    opponentBoxes++;
            }
            else if (board.CountSurroundingLines(row, col) == 2)
            {
                chains++;
            }
            else if (board.CountSurroundingLines(row, col) <= 1)
            {
                safeMoves++;
            }
        }
    }

    // Heuristic scoring function.
    return (aiBoxes - opponentBoxes) * 10 + chains * -5 + safeMoves * 2;
}
bool OzgeAkosa5177_DimitriNearchosdon5092_Player::CanControlChains()
{
    int chainsControlled = 0;
    int totalChains = 0;

    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if (board.CountSurroundingLines(row, col) == 2)
            {
                totalChains++;
                if (!CreatesChainForOpp({row, col}))
                {
                    chainsControlled++;
                }
            }
        }
    }

    return chainsControlled >= totalChains; // True if AI controls all chains.
}


void OzgeAkosa5177_DimitriNearchosdon5092_Player::CategorizeMoves()
{
    highPriorityLines.clear();
    lowRiskLines.clear();
    neutralLines.clear();

    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc loc = emptylines[i];
        int surroundingLines = board.CountSurroundingLines(loc.row, loc.col);

        if (surroundingLines == 3 && !CreatesChainForOpp(loc))
        {
            highPriorityLines.push_back(loc); // High-priority: Complete a box.
        }
        else if (surroundingLines == 2 && !CreatesChainForOpp(loc))
        {
            neutralLines.push_back(loc); // Midgame: Strategic moves.
        }
        else if (surroundingLines <= 1)
        {
            lowRiskLines.push_back(loc); // Low-risk: Safe moves.
        }
    }
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::SimulateMove(const Loc &loc)
{
    // Simulate adding the line.
    board(loc) = player_line;

    // Count how many boxes are gained.
    int boxesGained = 0;
    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if (board.CountSurroundingLines(row, col) == 4)
            {
                boxesGained++;
            }
        }
    }

    // Undo the simulated move.
    board(loc) = ' ';

    return boxesGained;
}

