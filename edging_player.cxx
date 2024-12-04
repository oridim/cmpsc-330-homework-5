#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>

#include "common.h"
#include "player.h"

#include "edging_player.h"

using namespace std;

extern "C" IPlayer *PlayerFactory()
{
    return new EdgingPlayer();
}

EdgingPlayer::EdgingPlayer()
{
}

EdgingPlayer::~EdgingPlayer()
{
}

void EdgingPlayer::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);

    player_box = _player_box;
    player_line = _player_line;
}

void EdgingPlayer::Close()
{
    board.FreeBoard();
}

void EdgingPlayer::EventAddLine(const char bar, const Loc &loc)
{
    assert(loc.IsLineLocation());
    assert(board(loc) == ' ');

    board(loc) = bar;
}

void EdgingPlayer::EventAddBox(const char box, const Loc &loc)
{
    assert(loc.IsBoxLocation());
    assert(board(loc) == ' ');

    board(loc) = box;
}

Loc EdgingPlayer::SelectLineLocation()
{
    cout << board;

    int columns = board.GetCols();
    int rows = board.GetRows();

    int maximumEdgeLayers = min(rows / 2, columns / 2);

    for (int layer = 0; layer < maximumEdgeLayers; layer++)
    {
        for (int row = layer + 1; row < (rows - layer); row += 2)
        {
            int opposingColumn = columns - layer - 1;

            if (board(row, layer) == ' ')
            {
                return Loc(row, layer);
            }
            else if (board(row, opposingColumn) == ' ')
            {
                return Loc(row, opposingColumn);
            }
        }

        for (int column = layer + 1; column < (columns - layer); column += 2)
        {
            int opposingRow = rows - layer - 1;

            if (board(layer, column) == ' ')
            {
                return Loc(layer, column);
            }
            else if (board(opposingRow, column) == ' ')
            {
                return Loc(opposingRow, column);
            }
        }
    }

    throw "PANIC THIS SHOULD NEVER HAPPEN!";
}