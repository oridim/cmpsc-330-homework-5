#include <assert.h>
#include <iostream>
#include <vector>

#include "common.h"
#include "board.h"

using namespace std;

void Board::AllocateBoard(int dotsInRows, int dotsInColumns, int &blankLineCount)
{
    assert(board == nullptr);

    rows = dotsInRows * 2 - 1;
    columns = dotsInColumns * 2 - 1;

    board = new char *[rows];

    // Allocate memory for the board.
    for (int row = 0; row < rows; row++)
    {
        board[row] = new char[columns];
    }

    blankLineCount = 0;

    // Initialize all board cells to blank spaces (' ') or dots ('.').
    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            board[row][column] = ' ';

            if (Loc(row, column).IsLineLocation())
            {
                blankLineCount++;
            }
        }
    }

     // Place dots ('.') at even row and column indices.
    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            board[row][column] = '.';
        }
    }
}

void Board::FreeBoard()
{
    if (board != nullptr)
    {
        // Free each row.
        for (int row = 0; row < rows; row++)
        {
            delete[] board[row];
        }

        delete[] board;
        board = nullptr;
    }
}

vector<Loc> Board::CollectBoxes(char displayCharacter, YIELD_KINDS yieldKind) const
{
    vector<Loc> availableBoxes = vector<Loc>();

    // Iterate over all box locations (odd row and column indices).
    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            if (board[row][column] == displayCharacter)
            {
                // Skip boxes that don't match the yield kind.
                if (
                    (yieldKind == YIELD_KINDS::capture && !DoesBoxYieldCapture(row, column)) ||
                    (yieldKind == YIELD_KINDS::chain && !DoesBoxYieldChain(row, column)))
                {
                    continue;
                }

                availableBoxes.push_back(Loc(row, column));
            }
        }
    }

    return availableBoxes;
}

vector<Loc> Board::CollectMoves(char displayCharacter, YIELD_KINDS yieldKind) const
{
    vector<Loc> availableBoxes = vector<Loc>();

    // Iterate over all horizontal line locations (even rows, odd columns).   
    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            if (board[row][column] == displayCharacter)
            {
                if (
                    (yieldKind == YIELD_KINDS::capture && !DoesMoveYieldCapture(row, column)) ||
                    (yieldKind == YIELD_KINDS::chain && !DoesMoveYieldChain(row, column)))
                {
                    continue;
                }

                availableBoxes.push_back(Loc(row, column));
            }
        }
    }
    // Iterate over all vertical line locations (odd rows, even columns).
    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            if (board[row][column] == displayCharacter)
            {
                if (
                    (yieldKind == YIELD_KINDS::capture && !DoesMoveYieldCapture(row, column)) ||
                    (yieldKind == YIELD_KINDS::chain && !DoesMoveYieldChain(row, column)))
                {
                    continue;
                }

                availableBoxes.push_back(Loc(row, column));
            }
        }
    }

    return availableBoxes;
}

int Board::CountAdjacentNextSurroundingLines(const Loc &location) const
{
    if (location.IsLineVerticalLocation())
    {
        int nextColumn = location.col + 1;

        // Return the surrounding line count for the adjacent cell if valid.
        return (nextColumn < columns) ? CountSurroundingLines(location.row, nextColumn) : 0;
    }

    int nextRow = location.row + 1;

    // Return the surrounding line count for the adjacent cell if valid.
    return (nextRow < rows) ? CountSurroundingLines(nextRow, location.col) : 0;
}

int Board::CountAdjacentPreviousSurroundingLines(const Loc &location) const
{
    if (location.IsLineVerticalLocation())
    {
        int previousColumn = location.col - 1;

        return (previousColumn >= 0) ? CountSurroundingLines(location.row, previousColumn) : 0;
    }

    int previousRow = location.row - 1;

    return (previousRow >= 0) ? CountSurroundingLines(previousRow, location.col) : 0;
}

int Board::CountSurroundingLines(int row, int column) const
{
    int lineCount = 0;

    // Check the four possible surrounding positions of the cell (top, bottom, left, right).
    if (row > 0 && this->operator()(row - 1, column) != ' ')
    {
        lineCount++; // Above
    }

    if (row < GetRows() - 1 && this->operator()(row + 1, column) != ' ')
    {
        lineCount++; // Below
    }

    if (column > 0 && this->operator()(row, column - 1) != ' ')
    {
        lineCount++; // Left
    }

    if (column < GetCols() - 1 && this->operator()(row, column + 1) != ' ')
    {
        lineCount++; // Right
    }

    return lineCount;
}

bool Board::DoesBoxYieldCapture(int row, int col) const
{
    return CountSurroundingLines(row, col) == 3;
}

bool Board::DoesBoxYieldChain(int row, int col) const
{
    return CountSurroundingLines(row, col) == 2;
}

bool Board::DoesBoxYieldFreebie(int row, int col) const
{
    return CountSurroundingLines(row, col) == 0;
}

bool Board::DoesBoxYieldPrevention(int row, int col) const
{
    return CountSurroundingLines(row, col) == 1;
}

bool Board::DoesMoveYieldCapture(const Loc &location) const
{
    // Determines if a box at a given location is ready to be captured.
    int previousAdjacentLines = CountAdjacentPreviousSurroundingLines(location);
    int nextAdjacentLines = CountAdjacentNextSurroundingLines(location);

    return (previousAdjacentLines == 3) || (nextAdjacentLines == 3);
}

bool Board::DoesMoveYieldChain(const Loc &location) const
{
    // Determines if a box at a given location is part of a chain.
    int previousAdjacentLines = CountAdjacentPreviousSurroundingLines(location);
    int nextAdjacentLines = CountAdjacentNextSurroundingLines(location);

    return (previousAdjacentLines == 2) || (nextAdjacentLines == 2);
}

bool Board::DoesMoveYieldFreebie(const Loc &location) const
{
    //  Determines if a box is completely free of lines
    int previousAdjacentLines = CountAdjacentPreviousSurroundingLines(location);
    int nextAdjacentLines = CountAdjacentNextSurroundingLines(location);

    return (previousAdjacentLines == 0 && nextAdjacentLines != 2) &&
           (previousAdjacentLines != 2 && nextAdjacentLines == 0);
}

bool Board::DoesMoveYieldPrevention(const Loc &location) const
{
    //Determines if a box is a "prevention" move.
    int previousAdjacentLines = CountAdjacentPreviousSurroundingLines(location);
    int nextAdjacentLines = CountAdjacentNextSurroundingLines(location);

    return (previousAdjacentLines == 1 && nextAdjacentLines != 2) &&
           (previousAdjacentLines != 2 && nextAdjacentLines == 1);
}

ostream &operator<<(ostream &outputStream, const Board &board)
{
    // Outputs the current state of the board to the console.
    cout << "   ";

    for (int column = 0; column < board.columns; column++)
    {
        if (column % 10 == 0)
        {
            cout << (column / 10);
        }
        else
        {
            cout << ' ';
        }
    }

    cout << endl;
    cout << "   ";

    for (int column = 0; column < board.columns; column++)
    {
        cout << (column % 10);
    }

    cout << endl;

    for (int row = 0; row < board.GetRows(); row++)
    {
        if (row % 10 == 0)
        {
            cout << (row / 10) << (row % 10) << ' ';
        }
        else
        {
            cout << ' ' << (row % 10) << ' ';
        }

        for (int column = 0; column < board.GetCols(); column++)
        {
            Loc location = Loc(row, column);
            char displayCharacter = board(row, column);

            if (displayCharacter == ' ')
            {
                cout << displayCharacter;
            }
            else if (location.IsLineVerticalLocation() && displayCharacter != ' ')
            {
                cout << "|";
            }
            else if (location.IsLineHorizontalLocation() && displayCharacter != ' ')
            {
                cout << "-";
            }
            else
            {
                cout << displayCharacter;
            }
        }

        cout << endl;
    }

    return outputStream;
}
