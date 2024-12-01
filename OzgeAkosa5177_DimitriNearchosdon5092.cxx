#include <assert.h>
#include <cstdlib> // for srand and rand
#include <ctime>   // for time
#include <iostream>
#include <vector>

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
    ListEmptyLines(); // Find empty lines and categorize them.

    bool isEndgame = emptylines_cnt < 10; // Consider it an endgame if there are fewer than 10 empty lines.

    //Look for high-priority moves to complete a box.
    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc loc = emptylines[i];
        if (board.CountSurroundingLines(loc.row, loc.col) == 3 && !CreatesChainForOpp(loc))
        {
            return loc; // Complete a box if it won't create chains.
        }
    }

    // Apply the double-cross strategy during the endgame.
    if (isEndgame)
    {
        for (int i = 0; i < emptylines_cnt; i++)
        {
            Loc loc = emptylines[i];
            if (CreatesDoubleCross(loc)) // Check if the move sets up a double-cross.
            {
                return loc; // Prioritize double-cross moves in the endgame.
            }
        }
    }

    //Avoid creating chains for the opponent by playing safe moves.
    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc loc = emptylines[i];
        if (board.CountSurroundingLines(loc.row, loc.col) <= 1) // Moves with 1 or fewer surrounding lines.
        {
            return loc; // Make a safe move.
        }
    }
// Use Minimax to evaluate the best fallback move.
    int bestValue = -100000; // Initialize to a very low value.
    Loc bestMove;

    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc loc = emptylines[i];
        board(loc) = player_line; // Simulate AI's move.

        int moveValue = Minimax(3, false); // Look 3 moves ahead.
        if (moveValue > bestValue)
        {
            bestValue = moveValue;
            bestMove = loc;
        }

        board(loc) = ' '; // Undo the move.
    }

   
}



int OzgeAkosa5177_DimitriNearchosdon5092_Player::Minimax(int depth, bool isMaximizing)
{
    if (depth == 0 || emptylines_cnt == 0) // Base case: max depth or no moves left.
        return EvaluateBoard();           // Evaluate the current board state.

    if (isMaximizing) // Maximize AI's score.
    {
        int bestValue = -100000;

        for (int i = 0; i < emptylines_cnt; i++)
        {
            Loc loc = emptylines[i];
            board(loc) = player_line; // Simulate AI's move.

            int value = Minimax(depth - 1, false); // Recur for opponent's turn.
            bestValue = max(bestValue, value);

            board(loc) = ' '; // Undo the move.
        }

        return bestValue;
    }
    else // Minimize opponent's score.
    {
        int bestValue = 100000;

        for (int i = 0; i < emptylines_cnt; i++)
        {
            Loc loc = emptylines[i];
            board(loc) = opponent_line; // Simulate opponent's move.

            int value = Minimax(depth - 1, true); // Recur for AI's turn.
            bestValue = min(bestValue, value);

            board(loc) = ' '; // Undo the move.
        }

        return bestValue;
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

