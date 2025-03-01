#ifndef BOARD_H
#define BOARD_H

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Iblock.h"

#define SIZE 8
#define STARTING_PEICES 6

//🟥🟪🟦🟩🟨🟧
enum ColorMap
{
    EMPTY,
    RED,
    PURPLE,
    BLUE,
    GREEN,
    YELLOW,
    ORANGE
};

typedef struct BlockPlacement
{
    bool placed;
    ColorMap color;
} BlockPlacement;

class board
{
public:
    board();
    ~board();

    std::string ToString();
    
    const BlockPlacement (*GetBoard() const)[SIZE];

    bool UpdateBoard();
    std::string palcePeice(Iblock* peice, int row, int col);

    std::vector<Iblock*> GetPeices() const;

    static int random(int min, int max);
    static std::string GetColor(ColorMap color);
    
private:
    std::vector<Iblock*> _pieces;
    BlockPlacement _gameBoard[SIZE][SIZE];
    
    

    void getAllPeices();
    bool placePeice(Iblock* peice);
    void randomizeBoard();
    
};

#endif