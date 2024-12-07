#ifndef __BOARD__
#define __BOARD__

#include <vector>

#include "./common.h"

class IBoard
{
public:
    virtual int GetRows() const = 0;
    virtual int GetCols() const = 0;
    virtual char operator()(int row, int col) const = 0;
    virtual char operator()(const Loc &loc) const = 0;
    virtual char &operator()(int row, int col) = 0;
    virtual char &operator()(const Loc &loc) = 0;
};

class Board : public IBoard
{
private:
    char **board;

    int columns;

    int rows;

public:
    Board()
    {
        rows = columns = -1;
        board = nullptr;
    }

    ~Board()
    {
        FreeBoard();
    }

    // Allocate memory for the board and initialize it.
    void AllocateBoard(int dotsInRows, int dotsInColumns, int &blankLineCount);
    void AllocateBoard(int dotsInRows, int dotsInColumns)
    {
        int blankLineCount;
        AllocateBoard(dotsInRows, dotsInColumns, blankLineCount);
    }

    void FreeBoard();

    // Getters for rows and columns.
    int GetRows() const { return rows; }
    int GetCols() const { return columns; }

    // Overloaded operators for accessing and modifying board cells.
    char &operator()(int row, int column) { return board[row][column]; }
    char operator()(int row, int column) const { return board[row][column]; }
    char &operator()(const Loc &location) { return board[location.row][location.col]; }
    char operator()(const Loc &location) const { return board[location.row][location.col]; }

    // Count the number of lines around a given cell.
    int CountSurroundingLines(int row, int column) const;

    int CountSurroundingLines(const Loc &location) const { return CountSurroundingLines(location.row, location.col); };

    // Output the board state to a stream
    friend ostream &operator<<(ostream &outputStream, const Board &board);
};

#endif
