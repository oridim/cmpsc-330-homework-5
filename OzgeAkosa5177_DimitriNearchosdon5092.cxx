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
    srand(time(0));
    emptylines = nullptr;
}

OzgeAkosa5177_DimitriNearchosdon5092_Player::~OzgeAkosa5177_DimitriNearchosdon5092_Player()
{
    Close();
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);
    player_box = _player_box;
    player_line = _player_line;
    opponent_line = (player_line == 'A') ? 'B' : 'A'; // Assuming 2 players: A and B
    emptylines = new Loc[board.GetRows() * board.GetCols()];
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::Close()
{
    board.FreeBoard();
    delete[] emptylines;
    emptylines = nullptr;
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
    // Step 1: Complete a box if possible
    Loc scoringMove = FindScoringMove();
    if (scoringMove.row != -1 && scoringMove.col != -1)
    {
        return scoringMove;
    }

    // Step 2: Control chain-like situations
    Loc chainControlMove = FindChainControlMove();
    if (chainControlMove.row != -1 && chainControlMove.col != -1)
    {
        return chainControlMove; // Control chains to maintain an advantage
    }

    // Step 3: Block or disrupt opponents' scoring opportunities
    Loc disruptiveMove = FindDisruptiveMove();
    if (disruptiveMove.row != -1 && disruptiveMove.col != -1)
    {
        return disruptiveMove;
    }

    // Step 4: Avoid creating three-line boxes for the opponent
    Loc safeMove = FindSafeMove();
    if (safeMove.row != -1 && safeMove.col != -1)
    {
        return safeMove;
    }

    // Step 5: Fallback to any available move
    ListEmptyLines();
    if (emptylines_cnt > 0)
    {
        int randloc = rand() % emptylines_cnt;
        return emptylines[randloc];
    }

    return {-1, -1}; // No valid moves left
}


Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::FindChainControlMove()
{
    for (int row = 1; row < board.GetRows(); row += 2)
    {
        for (int col = 1; col < board.GetCols(); col += 2)
        {
            if (board.CountSurroundingLines(row, col) == 2) // Part of a chain
            {
                // Look for a line to maintain the chain without completing it
                if (board({row - 1, col}) == ' ') return {row - 1, col};
                if (board({row + 1, col}) == ' ') return {row + 1, col};
                if (board({row, col - 1}) == ' ') return {row, col - 1};
                if (board({row, col + 1}) == ' ') return {row, col + 1};
            }
        }
    }
    return {-1, -1}; // No chain to control
}

Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::FindScoringMove()
{
    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if (row % 2 == 1 && col % 2 == 1) // Only check box locations
            {
                int lineCount = 0;
                Loc emptyLine = {-1, -1};

                Loc top = {row - 1, col};
                if (board(top) != ' ')
                {
                    lineCount++;
                }
                else
                {
                    emptyLine = top;
                }

                Loc bottom = {row + 1, col};
                if (board(bottom) != ' ')
                {
                    lineCount++;
                }
                else
                {
                    emptyLine = bottom;
                }

                Loc left = {row, col - 1};
                if (board(left) != ' ')
                {
                    lineCount++;
                }
                else
                {
                    emptyLine = left;
                }

                Loc right = {row, col + 1};
                if (board(right) != ' ')
                {
                    lineCount++;
                }
                else
                {
                    emptyLine = right;
                }

                if (lineCount == 3) // Return the empty line that completes the box
                {
                    return emptyLine;
                }
            }
        }
    }

    return {-1, -1}; // No scoring move found
}

Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::FindDisruptiveMove()
{
    for (int row = 1; row < board.GetRows(); row += 2)
    {
        for (int col = 1; col < board.GetCols(); col += 2)
        {
            if (board.CountSurroundingLines(row, col) == 2)
            {
                // Check adjacent lines to disrupt
                if (board({row - 1, col}) == ' ') return {row - 1, col};
                if (board({row + 1, col}) == ' ') return {row + 1, col};
                if (board({row, col - 1}) == ' ') return {row, col - 1};
                if (board({row, col + 1}) == ' ') return {row, col + 1};
            }
        }
    }
    return {-1, -1};
}


Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::FindDisruptiveMove()
{
    for (int row = 1; row < board.GetRows(); row += 2)
    {
        for (int col = 1; col < board.GetCols(); col += 2)
        {
            if (board.CountSurroundingLines(row, col) == 2)
            {
                // Check adjacent lines to disrupt
                if (board({row - 1, col}) == ' ') return {row - 1, col};
                if (board({row + 1, col}) == ' ') return {row + 1, col};
                if (board({row, col - 1}) == ' ') return {row, col - 1};
                if (board({row, col + 1}) == ' ') return {row, col + 1};
            }
        }
    }
    return {-1, -1};
}


void OzgeAkosa5177_DimitriNearchosdon5092_Player::ListEmptyLines()
{
    emptylines_cnt = 0;

    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if ((row % 2 == 0 && col % 2 == 0) || (row % 2 == 1 && col % 2 == 1))
            {
                continue; // Skip dots and box centers
            }

            if (board(row, col) == ' ')
            {
                emptylines[emptylines_cnt].row = row;
                emptylines[emptylines_cnt].col = col;
                emptylines_cnt++;
            }
        }
    }
}