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
    int columns = board.GetCols();
    int rows = board.GetRows();

    bool shouldScanOutwards = rand() % 2 == 0;
    int maximumEdgeLayers = min(rows / 2, columns / 2);

    for (
        int layer = shouldScanOutwards ? maximumEdgeLayers : 0;
        shouldScanOutwards ? (layer >= 0) : (layer < maximumEdgeLayers);
        shouldScanOutwards ? layer-- : layer++)
    {
        // **HACK:** Does the code reuse suck? Yes?
        // Do I care? No.
        // Should I care? Also no.

        bool shouldInvertDirection = rand() % 2 == 0;

        if (shouldInvertDirection)
        {
            for (int column = layer + 1; column < (columns - layer); column += 2)
            {
                bool shouldInvertScan = rand() % 2 != 0;
                int opposingRow = rows - layer - 1;

                if (board(shouldInvertScan ? opposingRow : layer, column) == ' ')
                {
                    return Loc(shouldInvertScan ? opposingRow : layer, column);
                }
                else if (board(shouldInvertScan ? layer : opposingRow, column) == ' ')
                {
                    return Loc(shouldInvertScan ? layer : opposingRow, column);
                }
            }

            for (int row = layer + 1; row < (rows - layer); row += 2)
            {
                bool shouldInvertScan = rand() % 2 != 0;
                int opposingColumn = columns - layer - 1;

                if (board(row, shouldInvertScan ? opposingColumn : layer) == ' ')
                {
                    return Loc(row, shouldInvertScan ? opposingColumn : layer);
                }
                else if (board(row, shouldInvertScan ? layer : opposingColumn) == ' ')
                {
                    return Loc(row, shouldInvertScan ? layer : opposingColumn);
                }
            }
        }
        else
        {
            for (int row = layer + 1; row < (rows - layer); row += 2)
            {
                bool shouldInvertScan = rand() % 2 != 0;
                int opposingColumn = columns - layer - 1;

                if (board(row, shouldInvertScan ? opposingColumn : layer) == ' ')
                {
                    return Loc(row, shouldInvertScan ? opposingColumn : layer);
                }
                else if (board(row, shouldInvertScan ? layer : opposingColumn) == ' ')
                {
                    return Loc(row, shouldInvertScan ? layer : opposingColumn);
                }
            }

            for (int column = layer + 1; column < (columns - layer); column += 2)
            {
                bool shouldInvertScan = rand() % 2 != 0;
                int opposingRow = rows - layer - 1;

                if (board(shouldInvertScan ? opposingRow : layer, column) == ' ')
                {
                    return Loc(shouldInvertScan ? opposingRow : layer, column);
                }
                else if (board(shouldInvertScan ? layer : opposingRow, column) == ' ')
                {
                    return Loc(shouldInvertScan ? layer : opposingRow, column);
                }
            }
        }
    }

    throw "PANIC THIS SHOULD NEVER HAPPEN!";
}