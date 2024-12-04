#include <assert.h>
#include <cstdlib> // for srand and rand
#include <ctime>   // for time
#include <iostream>
#include <vector>

#include "common.h"
#include "player.h"

#include "mockery_player.h"

using namespace std;

extern "C" IPlayer *PlayerFactory()
{
    return new MockeryPlayer();
}

MockeryPlayer::MockeryPlayer()
{
    srand(time(0));
}

MockeryPlayer::~MockeryPlayer()
{
}

void MockeryPlayer::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);

    player_box = _player_box;
    player_line = _player_line;
    last_move = Loc();
}

void MockeryPlayer::Close()
{
    board.FreeBoard();
}

void MockeryPlayer::EventAddLine(const char bar, const Loc &loc)
{
    assert(loc.IsLineLocation());
    assert(board(loc) == ' ');

    board(loc) = bar;

    last_move.col = loc.col;
    last_move.row = loc.row;
}

void MockeryPlayer::EventAddBox(const char box, const Loc &loc)
{
    assert(loc.IsBoxLocation());
    assert(board(loc) == ' ');

    board(loc) = box;
}

Loc MockeryPlayer::SelectLineLocation()
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    vector<Loc> legalMoves = vector<Loc>();

    if (last_move.IsLineLocation())
    {
        int row = last_move.row;
        int column = last_move.col;

        int opposingRow = rows - last_move.row - 1;
        int opposingColumn = columns - last_move.col - 1;

        switch (rand() % 3)
        {
        case 0:
            row = opposingRow;

            break;

        case 1:
            column = opposingColumn;

            break;

        case 2:
            row = opposingRow;
            column = opposingColumn;

            break;
        }

        if (board(row, column) == ' ')
        {
            return Loc(row, column);
        }
    }

    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            if (board(row, column) == ' ')
            {
                legalMoves.push_back(Loc(row, column));
            }
        }
    }

    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            if (board(row, column) == ' ')
            {
                legalMoves.push_back(Loc(row, column));
            }
        }
    }

    return legalMoves.at(rand() % legalMoves.size());
}