#include "board.h"
#include <dirent.h>
#include <iostream>
#include <random>

board::board()
{
    getAllPeices();
    randomizeBoard();
    UpdateBoard();
}

board::~board()
{

}

std::string board::ToString()
{
    int i = 0;
    int j = 0;
    std::string board = "";

    for(i = 0; i < SIZE; i++)
    {
        board += "|";
        for (j = 0; j < SIZE; j++)
        {
            board += "-----";
        }
        board += "|\n";
        board += "| ";
        for(j = 0; j < SIZE; j++)
        {
            if (this->_gameBoard[i][j].placed)
            {
                board += board::GetColor(_gameBoard[i][j].color);
            }
            else
            {
                board += "   ";
            }
            if (j != SIZE - 1)
            {
                board += " |";
            }
        }
        board += " |\n";
    }
    board += "|";
    for (j = 0; j < SIZE; j++)
    {
        board += "-----";
    }
    board += "|\n";

    return board;
}

const BlockPlacement (*board::GetBoard() const)[SIZE] {
    return _gameBoard;  // ✅ Returns pointer to 2D array
}


int board::random(int min, int max)
{
    static bool first = true;
    if (first) 
    {  
       srand( time(NULL) );
       first = false;
    }
    return min + rand() % (( max + 1 ) - min);
}

std::string board::GetColor(ColorMap color)
{
    //🟥🟪🟦🟩🟨🟧
    switch (color)
    {
    case RED:
        return "🟥";
        break;
    case PURPLE:
        return "🟪";
        break;
    case BLUE:
        return "🟦";
        break;
    case GREEN:
        return "🟩";
        break;
    case YELLOW:
        return "🟨";
        break;
    case ORANGE:
        return "🟧";
        break;
    default:
        return "⬛";
        break;
    }
}

void board::getAllPeices()
{
    std::vector<std::string> fileNames;
    DIR* dir;
    struct dirent *en;
    std::string currFile = "";
    dir = opendir("./peices");
    if(dir)
    {
        while( ((en = readdir(dir)) != NULL))
        {
            currFile = std::string(en->d_name);
            if(!(currFile == "." || currFile == ".."))
            {
                fileNames.push_back(currFile);
                //std::cout << currFile << std::endl;
            }
        }
        closedir(dir);
    }
    else
    {
        std::cerr << "couldnt open ./peices!" << std::endl;
    }
    
    for(auto file : fileNames)
    {
        Iblock* block = new Iblock(file);
        //std::cout << file << std::endl;
        //std::cout << block->ToString() << std::endl;

        this->_pieces.push_back(block);
    }

}

std::vector<Iblock*> board::GetPeices() const
{
    return this->_pieces;
}

bool board::UpdateBoard()
{
    int i = 0;
    int j = 0;
    std::vector<int> rmRows;
    std::vector<int> rmCols;

    //finding which cols and rows needs to be removed
    for(i = 0; i < SIZE; i++)
    {
        if(this->_gameBoard[i][0].placed)
        {
            for(j = 1; j < SIZE; j++)
            {
                if(!this->_gameBoard[i][j].placed)
                {
                    break;
                }
                if(j == SIZE-1)
                {
                    rmRows.push_back(i);
                }
            }
        }
        if(this->_gameBoard[0][i].placed)
        {
            for(j = 1; j < SIZE; j++)
            {
                if(!this->_gameBoard[j][i].placed)
                {
                    break;
                }
                if(j == SIZE-1)
                {
                    rmCols.push_back(i);
                }
                
            }
        }
    }  

    if(!rmRows.empty())
    {
        for (auto row : rmRows)
        {
            for(i = 0; i < SIZE; i++)
            {
                this->_gameBoard[row][i].placed = false;
            }
        }
    }
    if(!rmCols.empty())
    {
        for (auto col : rmCols)
        {
            for(i = 0; i < SIZE; i++)
            {
                this->_gameBoard[i][col].placed = false;
            }
        }
    }

    return true;
}

void board::randomizeBoard()
{
    int i = 0;
    int randomPeice = 0;
    bool placeStatus = false;

    for(i = 0; i < STARTING_PEICES; i++)
    {
        do
        {
            randomPeice = random(0, this->_pieces.size() - 1);
            placeStatus = placePeice(this->_pieces[randomPeice]);
        } while(!placeStatus);
        
    }
}

/// @brief this funcion gets a peice and tries to place it on the board
/// @param peice a peice to place
/// @return true if placed false if failed to place (no free spot)
bool board::placePeice(Iblock* peice)
{
    int randomColor = random(1,6);
    int randomCol = random(0, 7);
    int randomRow = random(0, 7);

    int i = 0;
    int j = 0;

    //checking if peice fits in bounds
    if((peice->getCols() + randomCol > SIZE) || (peice->getRows() + randomRow > SIZE))
    {
        // std::cout << "Random location for generation outside play area for: " << peice->GetName() << std::endl;
        // std::cout << "Random location generated: (" << randomRow << ", " << randomCol << ")" << std::endl;
        return false;
    }

    //checking if it dosent overlap with anything else
    for(i = 0; i < peice->getCols(); i++)
    {
        for(j = 0; j < peice->getRows(); j++)
        {
            if(this->_gameBoard[randomRow + j][randomCol + i].placed && peice->GetPeice()[j][i])
            {
                // std::cout << "Another block is already placed in this location!" << std::endl;
                // std::cout << this->ToString() << std::endl;
                return false;
            }
        }
    }

    //if got here, no block is in the way and its in the bounds
    for(i = 0; i < peice->getCols(); i++)
    {
        for(j = 0; j < peice->getRows(); j++)
        {
            this->_gameBoard[randomRow + j][randomCol + i].placed = true;
            this->_gameBoard[randomRow + j][randomCol + i].color = ColorMap(randomColor);
        }
    }

    return true;

}


std::string board::palcePeice(Iblock* peice, int row, int col)
{
    int randomColor = random(1,6);
    int i = 0;
    int j = 0;

    //checking if peice fits in bounds
    if((peice->getCols() + col > SIZE) || (peice->getRows() + row > SIZE))
    {
        // std::cout << "Random location for generation outside play area for: " << peice->GetName() << std::endl;
        // std::cout << "Random location generated: (" << row << ", " << col << ")" << std::endl;
        return "Cant palce a peice outside the play area!";
    }

    //checking if it dosent overlap with anything else
    for(i = 0; i < peice->getCols(); i++)
    {
        for(j = 0; j < peice->getRows(); j++)
        {
            if(this->_gameBoard[row + j][col + i].placed && peice->GetPeice()[j][i])
            {
                // std::cout << "Another block is already placed in this location!" << std::endl;
                // std::cout << this->ToString() << std::endl;
                return "There is already a block there!";
            }
        }
    }

    //if got here, no block is in the way and its in the bounds
    for(i = 0; i < peice->getCols(); i++)
    {
        for(j = 0; j < peice->getRows(); j++)
        {
            this->_gameBoard[row + j][col + i].placed = true;
            this->_gameBoard[row + j][col + i].color = ColorMap(randomColor);
        }
    }

    return "";
}