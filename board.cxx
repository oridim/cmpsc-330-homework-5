#include <assert.h>
#include <iostream>
#include <vector>

#include "common.h"
#include "board.h"

using namespace std;

void Board::AllocateBoard(int dots_in_rows, int dots_in_cols, int &blanklinecount)
{
    assert(board == nullptr);

    rows = dots_in_rows * 2 - 1;
    columns = dots_in_cols * 2 - 1;

    board = new char *[rows];

    for (int row = 0; row < rows; row++)
    {
        board[row] = new char[columns];
    }

    blanklinecount = 0;

    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            board[row][column] = ' ';

            if (Loc(row, column).IsLineLocation())
            {
                blanklinecount++;
            }
        }
    }

    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            board[row][column] = '.';
        }
    }
}

vector<Loc> Board::CollectLegalMoves() const
{
    vector<Loc> legalMoves = vector<Loc>();

    for (int row = 0; row < rows; row += 2)
    {
        for (int column = 1; column < columns; column += 2)
        {
            if (board[row][column] == ' ')
            {
                legalMoves.push_back(Loc(row, column));
            }
        }
    }

    for (int row = 1; row < rows; row += 2)
    {
        for (int column = 0; column < columns; column += 2)
        {
            if (board[row][column] == ' ')
            {
                legalMoves.push_back(Loc(row, column));
            }
        }
    }

    return legalMoves;
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

void Board::FreeBoard()
{
    if (board != nullptr)
    {
        for (int row = 0; row < rows; row++)
        {
            delete[] board[row];
        }

        delete[] board;
        board = nullptr;
    }
}

ostream &operator<<(ostream &outputStream, const Board &board)
{
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
