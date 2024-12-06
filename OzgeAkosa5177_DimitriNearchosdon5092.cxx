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
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::Close()
{
    board.FreeBoard();
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

    for (const Loc &move : legalMoves)
    {
        // Step 1: Prioritize completing a box
        if (board.DoesMoveYieldCapture(move))
        {
            return move;
        }

        // Step 2: Avoid giving the opponent easy opportunities
        if (!board.DoesMoveYieldChain(move))
        {
            return move;
        }

        // Step 3: Look for disruptive opportunities
        if (board.DoesMoveYieldPrevention(move))
        {
            return move;
        }
    }

    // Step 4: Fallback to any available move randomly.
    return legalMoves.at(rand() % legalMoves.size());
}