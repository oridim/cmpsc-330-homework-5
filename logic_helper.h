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
#ifndef __LOGIC_HELPER__
#define __LOGIC_HELPER__

#include <vector>

#include "common.h"
#include "board.h"

using namespace std;

class LogicHelper
{
public:
    // Collect all legal moves available on the board.
    static vector<Loc> CollectMoves(const Board &board, char displayCharacter);

    // Collect scorable boxes based on display character and yield type.
    static vector<Loc> CollectLegalMoves(const Board &board) { return CollectMoves(board, ' '); };

    // Count surrounding lines for the "next" adjacent cell in the given direction.
    static int CountAdjacentNextSurroundingLines(const Board &board, int row, int column) { return CountAdjacentNextSurroundingLines(board, Loc(row, column)); };

    static int CountAdjacentNextSurroundingLines(const Board &board, const Loc &location);

    // Count surrounding lines for the "previous" adjacent cell in the given direction.
    static int CountAdjacentPreviousSurroundingLines(const Board &board, int row, int column) { return CountAdjacentPreviousSurroundingLines(board, Loc(row, column)); };

    static int CountAdjacentPreviousSurroundingLines(const Board &board, const Loc &location);

    // Count the number of lines around a given cell.
    static int CountSurroundingLines(const Board &board, int row, int column);

    static int CountSurroundingLines(const Board &board, const Loc &location) { return CountSurroundingLines(board, location.row, location.col); };

    // Check if a box is a "prevention" move (1 surrounding line).
    static bool DoesBoxYieldPrevention(const Board &board, const Loc &location) { return DoesBoxYieldPrevention(board, location.row, location.col); };

    static bool DoesBoxYieldPrevention(const Board &board, int row, int column);

    // Check if a move (line) will capture a box.
    static bool DoesMoveYieldCapture(const Board &board, const Loc &location);

    static bool DoesMoveYieldCapture(const Board &board, int row, int column) { return DoesMoveYieldCapture(board, Loc(row, column)); };

    // Check if a move creates or continues a chain.
    static bool DoesMoveYieldChain(const Board &board, const Loc &location);

    static bool DoesMoveYieldChain(const Board &board, int row, int column) { return DoesMoveYieldChain(board, Loc(row, column)); };
};

#endif
