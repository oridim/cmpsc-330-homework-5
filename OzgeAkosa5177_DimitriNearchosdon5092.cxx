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
    vector<Loc> legalMoves = board.CollectLegalMoves();

    // Step 1: Prioritize completing a box
    Loc *scoringMove = FindScoringMove(legalMoves);
    if (scoringMove != nullptr)
    {
        return *scoringMove;
    }

    // Step 2: Avoid giving the opponent easy opportunities
    Loc *safeMove = FindSafeMove(legalMoves);
    if (safeMove != nullptr)
    {
        return *safeMove;
    }

    // Step 3: Look for disruptive opportunities
    Loc *disruptiveMove = FindDisruptiveMove(legalMoves);
    if (disruptiveMove != nullptr)
    {
        return *disruptiveMove;
    }

    // Step 4: Fallback to any available move randomly.
    return legalMoves.at(rand() % legalMoves.size());
}

Loc *OzgeAkosa5177_DimitriNearchosdon5092_Player::FindScoringMove(const vector<Loc> &locations) const
{
    for (const Loc &location : locations)
    {
        if (board.DoesMoveYieldCapture(location))
        {
            return (Loc *)&location;
        }
    }

    return nullptr;
}

Loc *OzgeAkosa5177_DimitriNearchosdon5092_Player::FindSafeMove(const vector<Loc> &locations) const
{
    for (const Loc &location : locations)
    {
        if (!board.DoesMoveYieldChain(location))
        {
            return (Loc *)&location;
        }
    }

    return nullptr;
}

Loc *OzgeAkosa5177_DimitriNearchosdon5092_Player::FindDisruptiveMove(const vector<Loc> &locations) const
{
    for (const Loc &location : locations)
    {
        if (board.DoesMoveYieldPrevention(location))
        {
            return (Loc *)&location;
        }
    }

    return nullptr;
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
