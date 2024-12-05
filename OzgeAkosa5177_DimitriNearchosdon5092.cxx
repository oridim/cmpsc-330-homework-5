#include <assert.h>
#include <ctime> // for time
#include <iostream>
#include <random> // rand / srand
#include <vector>

#include "common.h"
#include "board.h"
#include "player.h"

#include "OzgeAkosa5177_DimitriNearchosdon5092.h"

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
    opponent_line = (player_line == 'A') ? 'B' : 'A';
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
    // Step 1: Prioritize completing a box
    Loc scoringMove = FindScoringMove();
    if (scoringMove.row != -1 && scoringMove.col != -1)
    {
        return scoringMove;
    }

    // Step 2: Avoid giving the opponent easy opportunities
    Loc safeMove = FindSafeMove();
    if (safeMove.row != -1 && safeMove.col != -1)
    {
        return safeMove;
    }

    // Step 3: Look for disruptive opportunities
    Loc disruptiveMove = FindDisruptiveMove();
    if (disruptiveMove.row != -1 && disruptiveMove.col != -1)
    {
        return disruptiveMove;
    }

    // Step 4: Fallback to any available move
    ListEmptyLines();
    if (emptylines_cnt > 0)
    {
        int randloc = rand() % emptylines_cnt;
        return emptylines[randloc];
    }

    return {-1, -1}; // No valid moves left
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

                // Check all sides of the box
                if (board({row - 1, col}) == ' ') emptyLine = {row - 1, col};
                else lineCount++;

                if (board({row + 1, col}) == ' ') emptyLine = {row + 1, col};
                else lineCount++;

                if (board({row, col - 1}) == ' ') emptyLine = {row, col - 1};
                else lineCount++;

                if (board({row, col + 1}) == ' ') emptyLine = {row, col + 1};
                else lineCount++;

                // If the box has three lines, return the empty line
                if (lineCount == 3)
                {
                    return emptyLine;
                }
            }
        }
    }
    return {-1, -1}; // No scoring move found
}

Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::FindSafeMove()
{
    ListEmptyLines();

    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc candidate = emptylines[i];
        board(candidate) = player_line; // Simulate move

        // Check if this move leaves the opponent an easy opportunity
        bool createsOpportunity = false;
        for (int row = 0; row < board.GetRows(); row++)
        {
            for (int col = 0; col < board.GetCols(); col++)
            {
                if (row % 2 == 1 && col % 2 == 1) // Check box locations
                {
                    if (board.CountSurroundingLines(row, col) == 3)
                    {
                        createsOpportunity = true;
                        break;
                    }
                }
            }
            if (createsOpportunity) break;
        }

        board(candidate) = ' '; // Undo simulated move

        if (!createsOpportunity)
        {
            return candidate; // Return the first safe move
        }
    }

    return {-1, -1}; // No safe move found
}

Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::FindDisruptiveMove()
{
    ListEmptyLines();

    for (int i = 0; i < emptylines_cnt; i++)
    {
        Loc candidate = emptylines[i];
        board(candidate) = player_line; // Simulate move

        // Check if this disrupts opponent setups
        bool disruptsOpponent = false;
        for (int row = 1; row < board.GetRows(); row += 2)
        {
            for (int col = 1; col < board.GetCols(); col += 2)
            {
                if (board.CountSurroundingLines(row, col) == 2) // Opponent can form a chain
                {
                    disruptsOpponent = true;
                    break;
                }
            }
            if (disruptsOpponent) break;
        }

        board(candidate) = ' '; // Undo simulation

        if (disruptsOpponent)
        {
            return candidate; // Prioritize moves that block opponents
        }
    }

    return {-1, -1}; // No disruptive move found
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
