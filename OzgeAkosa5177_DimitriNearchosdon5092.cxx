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
#include <assert.h>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include "common.h"
#include "board.h"
#include "player.h"

#include "logic_helper.h"

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

Loc OzgeAkosa5177_DimitriNearchosdon5092_Player::SelectLineLocation()
{
    vector<Loc> legalMoves = LogicHelper::CollectLegalMoves(board);

    // Step 1: Look for moves that immediately complete a box
    for (const Loc &move : legalMoves)
    {
        if (LogicHelper::DoesMoveYieldCapture(board, move))
        {
            return move; // Prioritize completing a box
        }
    }

    // Step 2: Avoid moves that set up the opponent for a score
    for (const Loc &move : legalMoves)
    {
        if (!LogicHelper::DoesMoveYieldChain(board, move))
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
            if (LogicHelper::DoesBoxYieldPrevention(board, neighbor))
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

    if (LogicHelper::DoesMoveYieldCapture(board, move))
    {
        score += 10; // Favor moves completing boxes.
    }

    if (LogicHelper::DoesMoveYieldChain(board, move))
    {
        score -= 5; // Penalize moves leaving chains.
    }

    return score;
}
