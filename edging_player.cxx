#include <algorithm> // for min
#include <assert.h>
#include <cmath>   // for max
#include <cstdlib> // for srand and rand
#include <ctime>   // for time
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
    srand(time(0));
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

vector<Loc> EdgingPlayer::ComputeLegalEdgeMoves()
{
    vector<Loc> legalEdgeMoves = vector<Loc>();

    int columns = board.GetCols();
    int rows = board.GetRows();

    bool shouldScanOutwards = rand() % 2 == 0;
    int maximumEdgeLayers = min(rows / 2, columns / 2);

    for (
        int layer = shouldScanOutwards ? maximumEdgeLayers : 0;
        shouldScanOutwards ? (layer >= 0) : (layer < maximumEdgeLayers);
        shouldScanOutwards ? layer-- : layer++)
    {

        for (int column = layer + 1; column < (columns - layer); column += 2)
        {
            Loc location = Loc(layer, column);
            Loc opposingLocation = Loc(rows - layer - 1, column);

            if (board(location) == ' ')
            {
                legalEdgeMoves.push_back(location);
            }

            if (board(opposingLocation) == ' ')
            {
                legalEdgeMoves.push_back(opposingLocation);
            }
        }

        for (int row = layer + 1; row < (rows - layer); row += 2)
        {
            Loc location = Loc(row, layer);
            Loc opposingLocation = Loc(row, columns - layer - 1);

            if (board(location) == ' ')
            {
                legalEdgeMoves.push_back(location);
            }

            if (board(opposingLocation) == ' ')
            {
                legalEdgeMoves.push_back(opposingLocation);
            }
        }

        if (legalEdgeMoves.size() > 0)
        {
            break;
        }
    }

    return legalEdgeMoves;
}

Loc EdgingPlayer::SelectLineLocation()
{
    vector<Loc> legalEdgeMoves = ComputeLegalEdgeMoves();

    return legalEdgeMoves.at(rand() % legalEdgeMoves.size());
}