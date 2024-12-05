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

    void AllocateBoard(int dotsInRows, int dotsInColumns, int &blankLineCount);
    void AllocateBoard(int dotsInRows, int dotsInColumns)
    {
        int blankLineCount;
        AllocateBoard(dotsInRows, dotsInColumns, blankLineCount);
    }

    void FreeBoard();

    // **MAKE SURE THESE ARE PUBLIC**
    int GetRows() const { return rows; }
    int GetCols() const { return columns; }

    char &operator()(int row, int column) { return board[row][column]; }
    char operator()(int row, int column) const { return board[row][column]; }
    char &operator()(const Loc &location) { return board[location.row][location.col]; }
    char operator()(const Loc &location) const { return board[location.row][location.col]; }

    vector<Loc> CollectLegalMoves() const;

    int CountAdjacentNextSurroundingLines(int row, int column) const;

    int CountAdjacentNextSurroundingLines(const Loc &location) const { return CountAdjacentNextSurroundingLines(location.row, location.col); };

    int CountSurroundingLines(int row, int column) const;

    int CountSurroundingLines(const Loc &location) const { return CountSurroundingLines(location.row, location.col); };

    friend ostream &operator<<(ostream &outputStream, const Board &board);
};

#endif
