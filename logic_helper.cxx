//---------------------------------------------------------------------
//
// Name: Ozge Selin Ak + Dimitri Orion Nearchos
// Email: osa5177@psu.edu + don5092@psu.edu
// Class: CMPSC 330
// Section: 001
//
// Assignment: Homework 5
// Due Date: December 6th, 2024
//
// Description:
//
// 
//The code implements a strategic player for the game "Dots and Boxes" 
//that evaluates the game board and selects moves based on a defined strategy. 
//It prioritizes completing boxes to score points, avoids moves that might 
//give the opponent an advantage, and uses a priority queue to handle critical 
//opportunities, such as completing boxes with three lines. Additionally, it
//dynamically updates its internal board state based on game events and uses 
//a fallback random selection when no strategic moves are available, ensuring 
//the player remains competitive throughout the game.
//--------------------------------------------------------------------- 
#include <vector>

#include "common.h"
#include "board.h"

#include "logic_helper.h"

vector<Loc> LogicHelper::CollectMoves(const Board &board, char displayCharacter)
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

int LogicHelper::CountAdjacentNextSurroundingLines(const Board &board, const Loc &location)
{
    if (location.IsLineVerticalLocation())
    {
        int columns = board.GetCols();
        int nextColumn = location.col + 1;

        // Return the surrounding line count for the adjacent cell if valid.
        return (nextColumn < columns) ? CountSurroundingLines(board, location.row, nextColumn) : 0;
    }

    int rows = board.GetRows();
    int nextRow = location.row + 1;

    // Return the surrounding line count for the adjacent cell if valid.
    return (nextRow < rows) ? CountSurroundingLines(board, nextRow, location.col) : 0;
}

int LogicHelper::CountAdjacentPreviousSurroundingLines(const Board &board, const Loc &location)
{
    if (location.IsLineVerticalLocation())
    {
        int previousColumn = location.col - 1;

        return (previousColumn >= 0) ? CountSurroundingLines(board, location.row, previousColumn) : 0;
    }

    int previousRow = location.row - 1;

    return (previousRow >= 0) ? CountSurroundingLines(board, previousRow, location.col) : 0;
}

int LogicHelper::CountSurroundingLines(const Board &board, int row, int column)
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

bool LogicHelper::DoesBoxYieldPrevention(const Board &board, int row, int col)
{
    return CountSurroundingLines(board, row, col) == 1;
}

bool LogicHelper::DoesMoveYieldCapture(const Board &board, const Loc &location)
{
    // Determines if a box at a given location is ready to be captured.
    int previousAdjacentLines = CountAdjacentPreviousSurroundingLines(board, location);
    int nextAdjacentLines = CountAdjacentNextSurroundingLines(board, location);

    return (previousAdjacentLines == 3) || (nextAdjacentLines == 3);
}

bool LogicHelper::DoesMoveYieldChain(const Board &board, const Loc &location)
{
    // Determines if a box at a given location is part of a chain.
    int previousAdjacentLines = CountAdjacentPreviousSurroundingLines(board, location);
    int nextAdjacentLines = CountAdjacentNextSurroundingLines(board, location);

    return (previousAdjacentLines == 2) || (nextAdjacentLines == 2);
}