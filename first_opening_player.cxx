#include <iostream>
#include <assert.h>
#include <cstdlib> // for srand and rand
#include <ctime>   // for time

#include "common.h"
#include "player.h"
#include "first_opening_player.h"

using namespace std;

extern "C" IPlayer *PlayerFactory()
{
    return new FirstOpeningPlayer();
}

FirstOpeningPlayer::FirstOpeningPlayer()
{
    srand(time(0));
}

FirstOpeningPlayer::~FirstOpeningPlayer()
{
}

void FirstOpeningPlayer::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);

    player_box = _player_box;
    player_line = _player_line;
}

void FirstOpeningPlayer::Close()
{
    board.FreeBoard();
}

void FirstOpeningPlayer::EventAddLine(const char bar, const Loc &loc)
{
    assert(loc.IsLineLocation());
    assert(board(loc) == ' ');

    board(loc) = bar;
}

void FirstOpeningPlayer::EventAddBox(const char box, const Loc &loc)
{
    assert(loc.IsBoxLocation());
    assert(board(loc) == ' ');

    board(loc) = box;
}

Loc FirstOpeningPlayer::SelectLineLocation()
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            if (board(row, column) == ' ')
            {
                return Loc(row, column);
            }
        }
    }

    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            if (board(row, column) == ' ')
            {
                return Loc(row, column);
            }
        }
    }

    throw "PANIC THIS SHOULD NEVER HAPPEN!";
}