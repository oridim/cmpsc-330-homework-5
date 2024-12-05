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
    int rows, cols;

public:
    Board()
    {
        rows = cols = -1;
        board = nullptr;
    }

    ~Board()
    {
        FreeBoard();
    }

    void AllocateBoard(int dots_in_rows, int dots_in_cols, int &blanklinecount);
    void FreeBoard();
    void AllocateBoard(int dots_in_rows, int dots_in_cols)
    {
        int blanklinecount;
        AllocateBoard(dots_in_rows, dots_in_cols, blanklinecount);
    }

    // **MAKE SURE THESE ARE PUBLIC**
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }

    char &operator()(int row, int col) { return board[row][col]; }
    char operator()(int row, int col) const { return board[row][col]; }
    char &operator()(const Loc &loc) { return board[loc.row][loc.col]; }
    char operator()(const Loc &loc) const { return board[loc.row][loc.col]; }

    vector<Loc> CollectLegalMoves() const;
    int CountSurroundingLines(int row, int col) const;
    int CountSurroundingLines(const Loc &loc) const;

    friend ostream &operator<<(ostream &os, const Board &board);
};

#endif
