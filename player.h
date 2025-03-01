#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include "board.h"
#include "Iblock.h"
#include <vector>


class player
{
public:
    player(board* board);
    ~player();

    void RegenerateBlocks();
    void AddToScore(int score);
    std::vector<Iblock*> GetBlocks();

private:
    int _score;
    std::vector<Iblock*> _blocks;
    board* _board;

};

#endif