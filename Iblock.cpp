#include "Iblock.h"
#include <fstream>
#include <iostream>
#include <sstream>

Iblock::Iblock(std::string fileName)
{
    this->cols = 0;
    this->rows = 0;
    this->name = fileName;
    this->readBlockFile(fileName);
    this->placeable = true;
}

Iblock::~Iblock()
{
    int i = 0;
    for(i = 0; i < rows; i++)
    {
        delete[] this->_peice[i];
    }
    delete[] this->_peice;
}

bool** Iblock::GetPeice() const
{
    return this->_peice;
}

int Iblock::getCols() const
{
    return this->cols;
}

int Iblock::getRows() const
{
    return this->rows;
}

std::string Iblock::GetName() const
{
    return this->name;
}

void Iblock::setUnplaceable() 
{
    this->placeable = false;
}

bool Iblock::getPlaceable() const
{
    return this->placeable;
}

void Iblock::readBlockFile(std::string fileName)
{
    if(!(this->cols == 0 || this->rows == 0))
    {
        std::cerr << "block already contains a peice" << std::endl;
        return;
    }

    std::ifstream blockFile("./peices/" + fileName);
    if(!blockFile.is_open())
    {
        std::cerr << "couldnt find file specified" << std::endl;
        return;
    }

    std::string line = "";
    std::string block = "";
    int i = 0;
    int j = 0;
    int row = 0;
    int col = 0;
    int maxCol = 0;

    //getting file into a string
    while(std::getline(blockFile, line))
    {
        std::istringstream iss(line);
        block += line + "\n";
    }

    //counting rows and cols
    for (i = 0; i < block.length(); i++)
    {
        if(block[i] == '\n') 
        {
            if(col > maxCol)
            {
                maxCol = col;
            }
            col = 0;
            row++;
        }
        else
        {
            col += 1;
        }
    }

    this->cols = maxCol;
    this->rows = row;

    int blockIndex = 0;

    //creating the 2d array of the peice and placing the peice in it
    this->_peice = new bool*[row];
    for (i = 0; i < row; i++)
    {
        this->_peice[i] = new bool[maxCol]{false};
        for (j = 0; j < maxCol; j++)
        {
            if (block[blockIndex] == '\n') 
            {
                break;
            }
            if (block[blockIndex] == '*')
            {
                this->_peice[i][j] = true;
            }
            blockIndex++;
        }
        blockIndex++;
    }
}

std::string Iblock::ToString() const
{
    if(!this->placeable)
    {
        return "❌";
    }
    int i = 0;
    int j = 0;
    std::string ret = "";
    for(i = 0; i < this->rows; i++)
    {
        for(j = 0; j < this->cols; j++)
        {
            if(this->_peice[i][j] == true)
            {
                ret += "*";
            }
            else
            {
                ret += " ";
            }
        }
        ret += "\n";
    }

    return ret;
}