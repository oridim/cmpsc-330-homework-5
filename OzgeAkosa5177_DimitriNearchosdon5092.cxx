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

vector<Loc> OzgeAkosa5177_DimitriNearchosdon5092_Player::CollectMoves(char displayCharacter) const
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    vector<Loc> availableBoxes = vector<Loc>();

    // Iterate over all horizontal line locations (even rows, odd columns).
    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            if (board(row, column) == displayCharacter)
            {
                availableBoxes.push_back(Loc(row, column));
            }
        }
    }
    // Iterate over all vertical line locations (odd rows, even columns).
    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            if (board(row, column) == displayCharacter)
            {
                availableBoxes.push_back(Loc(row, column));
            }
        }
    }

    return availableBoxes;
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::CountAdjacentNextSurroundingLines(const Loc &location) const
{
    if (location.IsLineVerticalLocation())
    {
        int columns = board.GetCols();
        int nextColumn = location.col + 1;

        // Return the surrounding line count for the adjacent cell if valid.
        return (nextColumn < columns) ? CountSurroundingLines(location.row, nextColumn) : 0;
    }

    int rows = board.GetRows();
    int nextRow = location.row + 1;

    // Return the surrounding line count for the adjacent cell if valid.
    return (nextRow < rows) ? CountSurroundingLines(nextRow, location.col) : 0;
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::CountAdjacentPreviousSurroundingLines(const Loc &location) const
{
    if (location.IsLineVerticalLocation())
    {
        int previousColumn = location.col - 1;

        return (previousColumn >= 0) ? CountSurroundingLines(location.row, previousColumn) : 0;
    }

    int previousRow = location.row - 1;

    return (previousRow >= 0) ? CountSurroundingLines(previousRow, location.col) : 0;
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::CountSurroundingLines(int row, int column) const
{
    int columns = board.GetCols();
    int rows = board.GetRows();

    int lineCount = 0;

    // Check the four possible surrounding positions of the cell (top, bottom, left, right).
    if (row > 0 && board(row - 1, column) != ' ')
    {
        lineCount++; // Above
    }

    if (row < rows - 1 && board(row + 1, column) != ' ')
    {
        lineCount++; // Below
    }

    if (column > 0 && board(row, column - 1) != ' ')
    {
        lineCount++; // Left
    }

    if (column < columns - 1 && board(row, column + 1) != ' ')
    {
        lineCount++; // Right
    }

    return lineCount;
}

bool OzgeAkosa5177_DimitriNearchosdon5092_Player::DoesBoxYieldPrevention(int row, int col) const
{
    return CountSurroundingLines(row, col) == 1;
}

bool OzgeAkosa5177_DimitriNearchosdon5092_Player::DoesMoveYieldCapture(const Loc &location) const
{
    // Determines if a box at a given location is ready to be captured.
    int previousAdjacentLines = CountAdjacentPreviousSurroundingLines(location);
    int nextAdjacentLines = CountAdjacentNextSurroundingLines(location);

    return (previousAdjacentLines == 3) || (nextAdjacentLines == 3);
}

bool OzgeAkosa5177_DimitriNearchosdon5092_Player::DoesMoveYieldChain(const Loc &location) const
{
    // Determines if a box at a given location is part of a chain.
    int previousAdjacentLines = CountAdjacentPreviousSurroundingLines(location);
    int nextAdjacentLines = CountAdjacentNextSurroundingLines(location);

    return (previousAdjacentLines == 2) || (nextAdjacentLines == 2);
}

Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::SelectLineLocation()
{
    vector<Loc> legalMoves = CollectLegalMoves();

    // Step 1: Look for moves that immediately complete a box
    for (const Loc &move : legalMoves)
    {
        if (DoesMoveYieldCapture(move))
        {
            return move; // Prioritize completing a box
        }
    }

    // Step 2: Avoid moves that set up the opponent for a score
    for (const Loc &move : legalMoves)
    {
        if (!DoesMoveYieldChain(move))
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
            {
                neighbors.emplace_back(move.row - 1, move.col);
            }

            if (move.row < board_rows - 1)
            {
                neighbors.emplace_back(move.row, move.col);
            }
        }
        else if (move.IsLineVerticalLocation())
        {
            // Check left and right of the vertical line
            if (move.col > 0)
            {
                neighbors.emplace_back(move.row, move.col - 1);
            }

            if (move.col < board_cols - 1)
            {
                neighbors.emplace_back(move.row, move.col);
            }
        }

        // Evaluate neighbors for safe moves
        for (const Loc &neighbor : neighbors)
        {
            if (DoesBoxYieldPrevention(neighbor))
            {
                return move; // Use neighbor logic for strategic play
            }
        }
    }

    // Step 4: Fallback to any available move
    return legalMoves.at(rand() % legalMoves.size());
}

int OzgeAkosa5177_DimitriNearchosdon5092_Player::EvaluateMove(const Loc &move)
{
    int score = 0;

    if (DoesMoveYieldCapture(move))
    {
        score += 10; // Favor moves completing boxes.
    }

    if (DoesMoveYieldChain(move))
    {
        score -= 5; // Penalize moves leaving chains.
    }

    return score;
}
