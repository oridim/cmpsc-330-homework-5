#include <assert.h>
#include <ctime> // for time
#include <iostream>
#include <random> // rand / srand
#include <vector>

#include "common.h"
#include "player.h"

#include "naive_eager_player.h"

using namespace std;

extern "C" IPlayer *PlayerFactory()
{
    return new NaiveEagerPlayer();
}

NaiveEagerPlayer::NaiveEagerPlayer()
{
    srand(time(0));
}

NaiveEagerPlayer::~NaiveEagerPlayer()
{
}

void NaiveEagerPlayer::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);

    player_box = _player_box;
    player_line = _player_line;
}

void NaiveEagerPlayer::Close()
{
    board.FreeBoard();
}

void NaiveEagerPlayer::EventAddLine(const char bar, const Loc &loc)
{
    assert(loc.IsLineLocation());
    assert(board(loc) == ' ');

    board(loc) = bar;
}

void NaiveEagerPlayer::EventAddBox(const char box, const Loc &loc)
{
    assert(loc.IsBoxLocation());
    assert(board(loc) == ' ');

    board(loc) = box;
}

Loc NaiveEagerPlayer::SelectLineLocation()
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    vector<Loc> legalMoves = vector<Loc>();

    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            Loc location = Loc(row, column);

            if (board(location) == ' ')
            {
                if (board.DoesMoveYieldCapture(location))
                {
                    return location;
                }

                legalMoves.push_back(location);
            }
        }
    }

    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            Loc location = Loc(row, column);

            if (board(location) == ' ')
            {
                if (board.DoesMoveYieldCapture(location))
                {
                    return location;
                }

                legalMoves.push_back(location);
            }
        }
    }

    vector<Loc> sellingMoves = vector<Loc>();
    vector<Loc> nonSellingMoves = vector<Loc>();

    for (Loc &location : legalMoves)
    {
        if (board.DoesMoveYieldChain(location))
        {
            sellingMoves.push_back(location);
        }
        else
        {
            nonSellingMoves.push_back(location);
        }
    }

    if (nonSellingMoves.size() > 0)
    {
        return nonSellingMoves.at(rand() % nonSellingMoves.size());
    }

    return sellingMoves.at(rand() % sellingMoves.size());
}