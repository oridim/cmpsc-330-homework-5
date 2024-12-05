#include <assert.h>
#include <cstdlib> // for srand and rand
#include <ctime>   // for time
#include <iostream>

#include "common.h"
#include "player.h"

#include "snake_oil_player.h"

using namespace std;

extern "C" IPlayer *PlayerFactory()
{
    return new SnakeOilPlayer();
}

SnakeOilPlayer::SnakeOilPlayer()
{
    srand(time(0));
}

SnakeOilPlayer::~SnakeOilPlayer()
{
}

void SnakeOilPlayer::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);

    player_box = _player_box;
    player_line = _player_line;
}

void SnakeOilPlayer::Close()
{
    board.FreeBoard();
}

void SnakeOilPlayer::EventAddLine(const char bar, const Loc &loc)
{
    assert(loc.IsLineLocation());
    assert(board(loc) == ' ');

    board(loc) = bar;
}

void SnakeOilPlayer::EventAddBox(const char box, const Loc &loc)
{
    assert(loc.IsBoxLocation());
    assert(board(loc) == ' ');

    board(loc) = box;
}

vector<Loc> SnakeOilPlayer::ComputeLegalMoves()
{
    vector<Loc> legalMoves = vector<Loc>();

    int columns = board.GetCols();
    int rows = board.GetRows();

    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            Loc location = Loc(row, column);

            if (board(location) == ' ')
            {
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
                legalMoves.push_back(location);
            }
        }
    }

    return legalMoves;
}

vector<Loc> SnakeOilPlayer::ComputePayoffMoves()
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    vector<Loc> payoffMoves = vector<Loc>();

    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            Loc location = Loc(row, column);

            if (board(location) == ' ' && DoesMovePayoff(location))
            {
                payoffMoves.push_back(location);
            }
        }
    }

    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            Loc location = Loc(row, column);

            if (board(location) == ' ' && DoesMovePayoff(location))
            {
                payoffMoves.push_back(location);
            }
        }
    }

    return payoffMoves;
}

vector<Loc> SnakeOilPlayer::ComputeSnakeMoves()
{
    vector<Loc> legalSnakeMoves = vector<Loc>();

    int columns = board.GetCols();
    int rows = board.GetRows();

    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            Loc location = Loc(row, column);

            if (board(location) == ' ' && !DoesMoveSell(location) && DoesMoveChainLine(location))
            {
                legalSnakeMoves.push_back(location);
            }
        }
    }

    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            Loc location = Loc(row, column);

            if (board(location) == ' ' && !DoesMoveSell(location) && DoesMoveChainLine(location))
            {
                legalSnakeMoves.push_back(location);
            }
        }
    }

    return legalSnakeMoves;
}

bool SnakeOilPlayer::DoesMoveChainLine(const Loc &loc)
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    int row = loc.row;
    int column = loc.col;

    Loc downAdjacentLocation = Loc(row + 1, column);
    Loc leftAdjacentLocation = Loc(row, column - 1);
    Loc rightAdjacentLocation = Loc(row, column + 1);
    Loc upAdjacentLocation = Loc(row - 1, column);

    int downAdjacentLines = (downAdjacentLocation.row < rows) ? board.CountSurroundingLines(downAdjacentLocation) : 0;
    int leftAdjacentLines = (leftAdjacentLocation.col > 0) ? board.CountSurroundingLines(leftAdjacentLocation) : 0;
    int rightAdjacentLines = (rightAdjacentLocation.col < columns) ? board.CountSurroundingLines(rightAdjacentLocation) : 0;
    int upAdjacentLines = (upAdjacentLocation.row > 0) ? board.CountSurroundingLines(upAdjacentLocation) : 0;

    // We want to make some big lines, so let's skip the one-liners.

    return (downAdjacentLines > 1) || (leftAdjacentLines > 1) || (rightAdjacentLines > 1) || (upAdjacentLines > 1);
}

bool SnakeOilPlayer::DoesMovePayoff(const Loc &loc)
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    int row = loc.row;
    int column = loc.col;

    Loc previousAdjacentLocation = loc.IsLineVerticalLocation() ? Loc(row, column - 1) : Loc(row - 1, column);
    Loc nextAdjacentLocation = loc.IsLineVerticalLocation() ? Loc(row, column + 1) : Loc(row + 1, column);

    int previousAdjacentLines = (previousAdjacentLocation.col >= 0 && previousAdjacentLocation.row >= 0) ? board.CountSurroundingLines(previousAdjacentLocation) : 0;
    int nextAdjacentLines = (nextAdjacentLocation.col < columns && nextAdjacentLocation.row < rows) ? board.CountSurroundingLines(nextAdjacentLocation) : 0;

    return (previousAdjacentLines == 3) || (nextAdjacentLines == 3);
}

bool SnakeOilPlayer::DoesMoveSell(const Loc &loc)
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    int row = loc.row;
    int column = loc.col;

    Loc previousAdjacentLocation = loc.IsLineVerticalLocation() ? Loc(row, column - 1) : Loc(row - 1, column);
    Loc nextAdjacentLocation = loc.IsLineVerticalLocation() ? Loc(row, column + 1) : Loc(row + 1, column);

    int previousAdjacentLines = (previousAdjacentLocation.col >= 0 && previousAdjacentLocation.row >= 0) ? board.CountSurroundingLines(previousAdjacentLocation) : 0;
    int nextAdjacentLines = (nextAdjacentLocation.col < columns && nextAdjacentLocation.row < rows) ? board.CountSurroundingLines(nextAdjacentLocation) : 0;

    return (previousAdjacentLines == 2) || (nextAdjacentLines == 2);
}

Loc SnakeOilPlayer::SelectLineLocation()
{
    vector<Loc> payoffMoves = ComputePayoffMoves();

    if (payoffMoves.size() > 0)
    {
        return payoffMoves.at(rand() % payoffMoves.size());
    }

    vector<Loc> snakeMoves = ComputeSnakeMoves();

    if (snakeMoves.size() > 0)
    {
        return snakeMoves.at(rand() % snakeMoves.size());
    }

    vector<Loc> legalMoves = ComputeLegalMoves();

    vector<Loc> sellingMoves = vector<Loc>();
    vector<Loc> nonSellingMoves = vector<Loc>();

    for (Loc &location : legalMoves)
    {
        if (DoesMoveSell(location))
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