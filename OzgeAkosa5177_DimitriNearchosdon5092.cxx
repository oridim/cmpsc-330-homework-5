#include <iostream>
#include <assert.h>
#include <cstdlib> // for srand and rand
#include <ctime>   // for time

#include "common.h"
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
}

OzgeAkosa5177_DimitriNearchosdon5092_Player::~OzgeAkosa5177_DimitriNearchosdon5092_Player()
{
    // **TODO**: Put deallocations of anything you allocated here that is not
    // directly related to the game engine.
    //
    // Put that stuff `.Close()`.
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    // **TODO**: Allocate anything you use to track the game / player state here.

    board.AllocateBoard(_dots_in_rows, _dots_in_cols);

    player_box = _player_box;
    player_line = _player_line;
    emptylines = new Loc[board.GetRows() * board.GetCols()];
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::Close()
{
    // **TODO**: Deallocate anything you use to track the game / player state here.

    board.FreeBoard();

    int emptylines_cnt = board.GetRows() * board.GetCols();
    delete[] emptylines;
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
    ListEmptyLines();

    int randloc = rand() % emptylines_cnt;
    return emptylines[randloc];
}

void OzgeAkosa5177_DimitriNearchosdon5092_Player::ListEmptyLines()
{
    emptylines_cnt = 0;
    for (int row = 0; row < board.GetRows(); row++)
    {
        for (int col = 0; col < board.GetCols(); col++)
        {
            if ((row % 2 == 0) && (col % 2 == 0))
            {
                continue; // dot
            }
            if ((row % 2 == 1) && (col % 2 == 1))
            {
                continue; // box
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
