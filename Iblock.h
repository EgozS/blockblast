#ifndef Iblock_H
#define Iblock_H

#pragma once
#define MAX_SIZE 3

#include <string>


class Iblock
{
public:
    Iblock(std::string fileName);
    ~Iblock();
    std::string ToString() const;
    void readBlockFile(std::string fileName);
    bool** GetPeice() const;
    int getCols() const;
    int getRows() const;
    std::string GetName() const;
    void setUnplaceable();
    bool getPlaceable() const;

protected:
    bool placeable;
    std::string name;
    bool** _peice;
    int cols;
    int rows;
};

#endif