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
public:
    enum YIELD_KINDS
    {
        undefined,

        capture,

        chain
    };

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

    vector<Loc> CollectBoxes(char displayCharacter, YIELD_KINDS yieldKind = YIELD_KINDS::undefined) const;

    vector<Loc> CollectMoves(char displayCharacter, YIELD_KINDS yieldKind = YIELD_KINDS::undefined) const;

    vector<Loc> CollectLegalMoves(YIELD_KINDS yieldKind = YIELD_KINDS::undefined) const { return CollectMoves(' ', yieldKind); };

    vector<Loc> CollectScorableBoxes(YIELD_KINDS yieldKind = YIELD_KINDS::undefined) const { return CollectBoxes(' ', yieldKind); }

    int CountAdjacentNextSurroundingLines(int row, int column) const { return CountAdjacentNextSurroundingLines(Loc(row, column)); };

    int CountAdjacentNextSurroundingLines(const Loc &location) const;

    int CountAdjacentPreviousSurroundingLines(int row, int column) const { return CountAdjacentPreviousSurroundingLines(Loc(row, column)); };

    int CountAdjacentPreviousSurroundingLines(const Loc &location) const;

    int CountSurroundingLines(int row, int column) const;

    int CountSurroundingLines(const Loc &location) const { return CountSurroundingLines(location.row, location.col); };

    bool DoesBoxYieldCapture(const Loc &location) const { return DoesBoxYieldCapture(location.row, location.col); };

    bool DoesBoxYieldCapture(int row, int column) const;

    bool DoesBoxYieldChain(const Loc &location) const { return DoesBoxYieldChain(location.row, location.col); };

    bool DoesBoxYieldChain(int row, int column) const;

    bool DoesBoxYieldFreebie(const Loc &location) const { return DoesBoxYieldFreebie(location.row, location.col); };

    bool DoesBoxYieldFreebie(int row, int column) const;

    bool DoesBoxYieldPrevention(const Loc &location) const { return DoesBoxYieldPrevention(location.row, location.col); };

    bool DoesBoxYieldPrevention(int row, int column) const;

    bool DoesMoveYieldCapture(const Loc &location) const;

    bool DoesMoveYieldCapture(int row, int column) const { return DoesMoveYieldCapture(Loc(row, column)); };

    bool DoesMoveYieldChain(const Loc &location) const;

    bool DoesMoveYieldChain(int row, int column) const { return DoesMoveYieldChain(Loc(row, column)); };

    bool DoesMoveYieldFreebie(const Loc &location) const;

    bool DoesMoveYieldFreebie(int row, int column) const { return DoesMoveYieldFreebie(Loc(row, column)); };

    bool DoesMoveYieldPrevention(const Loc &location) const;

    bool DoesMoveYieldPrevention(int row, int column) const { return DoesMoveYieldPrevention(Loc(row, column)); };

    friend ostream &operator<<(ostream &outputStream, const Board &board);
};

#endif
