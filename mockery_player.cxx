#include <algorithm> // for shuffle
#include <assert.h>
#include <ctime> // for time
#include <iostream>
#include <random> // for mersenne_twister_engine
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
    random_engine = mt19937(time(0));
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

int MockeryPlayer::CountLinesAroundSlot(const Loc &loc)
{
    int count = 0;

    int column = loc.col;
    int row = loc.row;

    for (int deltaRowIndex = -1; deltaRowIndex <= 1; deltaRowIndex += 2)
    {
        if (board(row + deltaRowIndex, column) != ' ')
        {
            count += 1;
        }
    }

    for (int deltaColumnIndex = -1; deltaColumnIndex <= 1; deltaColumnIndex += 2)
    {
        if (board(row, column + deltaColumnIndex) != ' ')
        {
            count += 1;
        }
    }

    return count;
}

bool MockeryPlayer::DoesMovePayoff(const Loc &loc)
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    int row = loc.row;
    int column = loc.col;

    Loc previousAdjacentLocation = loc.IsLineVerticalLocation() ? Loc(row, column - 1) : Loc(row - 1, column);
    Loc nextAdjacentLocation = loc.IsLineVerticalLocation() ? Loc(row, column + 1) : Loc(row + 1, column);

    int previousAdjacentLines = (previousAdjacentLocation.col >= 0 && previousAdjacentLocation.row >= 0) ? CountLinesAroundSlot(previousAdjacentLocation) : 0;
    int nextAdjacentLines = (nextAdjacentLocation.col < columns && nextAdjacentLocation.row < rows) ? CountLinesAroundSlot(nextAdjacentLocation) : 0;

    return ((previousAdjacentLines == 3) || (nextAdjacentLines == 3)) && !(previousAdjacentLines == 2 || nextAdjacentLines == 2);
}

bool MockeryPlayer::DoesMoveSell(const Loc &loc)
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    int row = loc.row;
    int column = loc.col;

    Loc previousAdjacentLocation = loc.IsLineVerticalLocation() ? Loc(row, column - 1) : Loc(row - 1, column);
    Loc nextAdjacentLocation = loc.IsLineVerticalLocation() ? Loc(row, column + 1) : Loc(row + 1, column);

    int previousAdjacentLines = (previousAdjacentLocation.col >= 0 && previousAdjacentLocation.row >= 0) ? CountLinesAroundSlot(previousAdjacentLocation) : 0;
    int nextAdjacentLines = (nextAdjacentLocation.col < columns && nextAdjacentLocation.row < rows) ? CountLinesAroundSlot(nextAdjacentLocation) : 0;

    return (previousAdjacentLines == 2) || (nextAdjacentLines == 2);
}

Loc MockeryPlayer::SelectLineLocation()
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    vector<Loc> legalMoves = vector<Loc>();

    if (last_move.IsLineLocation())
    {
        int column = last_move.col;
        int row = last_move.row;

        int opposingColumn = columns - column - 1;
        int opposingRow = rows - row - 1;

        vector<Loc> mirroredMoves = vector<Loc>();

        mirroredMoves.push_back(Loc(row, opposingColumn));
        mirroredMoves.push_back(Loc(opposingRow, column));
        mirroredMoves.push_back(Loc(opposingRow, opposingColumn));

        shuffle(begin(mirroredMoves), end(mirroredMoves), random_engine);

        for (const Loc &location : mirroredMoves)
        {
            if (board(location) == ' ' && DoesMovePayoff(location))
            {
                return location;
            }
        }

        for (const Loc &location : mirroredMoves)
        {
            if (board(location) == ' ' && !DoesMoveSell(location))
            {
                return location;
            }
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

    return legalMoves.at(random_engine() % legalMoves.size());
}