#include <assert.h>
#include <ctime> // for time
#include <iostream>
#include <random> // rand / srand
#include <vector>

#include "common.h"
#include "player.h"

#include "naive_eager_player.h"

using namespace std;

extern "C" IPlayer *PlayerFactory()
{
    return new NaiveEagerPlayer();
}

NaiveEagerPlayer::NaiveEagerPlayer()
{
    srand(time(0));
}

NaiveEagerPlayer::~NaiveEagerPlayer()
{
}

void NaiveEagerPlayer::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);

    player_box = _player_box;
    player_line = _player_line;
}

void NaiveEagerPlayer::Close()
{
    board.FreeBoard();
}

void NaiveEagerPlayer::EventAddLine(const char bar, const Loc &loc)
{
    assert(loc.IsLineLocation());
    assert(board(loc) == ' ');

    board(loc) = bar;
}

void NaiveEagerPlayer::EventAddBox(const char box, const Loc &loc)
{
    assert(loc.IsBoxLocation());
    assert(board(loc) == ' ');

    board(loc) = box;
}

Loc NaiveEagerPlayer::SelectLineLocation()
{
    vector<Loc> legalMoves = board.CollectLegalMoves();

    vector<Loc> chainingCaptures = vector<Loc>();
    vector<Loc> chainingMoves = vector<Loc>();
    vector<Loc> nonChainingCaptures = vector<Loc>();
    vector<Loc> nonChainingMoves = vector<Loc>();

    for (Loc &location : legalMoves)
    {
        if (board.DoesMoveYieldCapture(location))
        {
            if (board.DoesMoveYieldChain(location))
            {
                chainingCaptures.push_back(location);
            }
            else
            {
                nonChainingCaptures.push_back(location);
            }
        }
        else if (board.DoesMoveYieldChain(location))
        {
            chainingMoves.push_back(location);
        }
        else
        {
            nonChainingMoves.push_back(location);
        }
    }

    if (chainingCaptures.size() > 0)
    {
        return chainingCaptures.at(rand() % chainingCaptures.size());
    }
    else if (nonChainingCaptures.size() > 0)
    {
        return nonChainingCaptures.at(rand() % nonChainingCaptures.size());
    }
    else if (nonChainingMoves.size() > 0)
    {
        return nonChainingMoves.at(rand() % nonChainingMoves.size());
    }

    return chainingMoves.at(rand() % chainingMoves.size());
}