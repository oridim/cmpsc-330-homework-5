#include <assert.h>
#include <ctime>
#include <iostream>
#include <random>
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
    board_rows = _dots_in_rows;
    board_cols = _dots_in_cols;
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

    // Step 1: Look for moves that immediately complete a box
    for (const Loc &move : legalMoves)
    {
        if (board.DoesMoveYieldCapture(move))
        {
            return move; // Prioritize completing a box
        }
    }

    // Step 2: Avoid moves that set up the opponent for a score
    for (const Loc &move : legalMoves)
    {
        if (!board.DoesMoveYieldChain(move))
        {
            return move; // Prefer safe moves
        }
    }

    // Step 3: Check neighboring boxes for disruption opportunities
    for (const Loc &move : legalMoves)
    {
        vector<Loc> neighbors;

        if (move.IsLineHorizontalLocation())
        {
            // Check above and below the horizontal line
            if (move.row > 0)
                neighbors.emplace_back(move.row - 1, move.col);
            if (move.row < board_rows - 1)
                neighbors.emplace_back(move.row, move.col);
        }
        else if (move.IsLineVerticalLocation())
        {
            // Check left and right of the vertical line
            if (move.col > 0)
                neighbors.emplace_back(move.row, move.col - 1);
            if (move.col < board_cols - 1)
                neighbors.emplace_back(move.row, move.col);
        }

        // Evaluate neighbors for safe moves
        for (const Loc &neighbor : neighbors)
        {
            if (!board.DoesMoveYieldChain(neighbor))
            {
                return move; // Use neighbor logic for strategic play
            }
        }
    }

    // Step 4: Fallback to any available move
    return legalMoves.at(rand() % legalMoves.size());
}
