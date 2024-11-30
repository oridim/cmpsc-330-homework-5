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

Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::SelectLineLocation()
{
   ListEmptyLines(); // Find empty lines and categorize them.

    // Prioritize moves that complete a box (high-priority).
    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc loc = emptylines[i];
        if (board.CountSurroundingLines(loc.row, loc.col) == 3) // Fix the syntax here
        {
            if (!CreatesChainForOpp (loc))
            {

            return loc;
            }
        }
    }

    // Avoid moves that leave a box with 3 sides (low-risk).
    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc loc = emptylines[i];
        if (board.CountSurroundingLines(loc.row, loc.col) <= 1) // Fix the syntax here
        {
            return loc;
        }
    }
    
    // If no strategic moves exist, fall back to random.
    int randloc = rand() % emptylines_cnt;
    return emptylines[randloc];

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


bool OzgeAkosa5177_DimitriNearchosdon5092_Player::CreatesChainForOpp(const Loc &loc)
{
    // Simulate the move.
    board(loc) = player_line;

    // Check how many boxes now have 2 sides filled.
    int chainsCreated = 0;
    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if (board.CountSurroundingLines(row, col) == 2)
            {
                chainsCreated++;
            }
        }
    }

    // Undo the move.
    board(loc) = ' ';

    return chainsCreated > 1; // If multiple chains are opened, this is bad.
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

