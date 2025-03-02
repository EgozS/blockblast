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
    std::vector<int> rmRows;
    std::vector<int> rmCols;
    int i = 0;
    int j = 0;
    bool fullRow = true;
    bool fullCol = true;

    //identify full rows and columns
    for (i = 0; i < SIZE; i++) 
    {
        fullRow = true;
        fullCol = true;

        for (j = 0; j < SIZE; j++) 
        {
            if (!this->_gameBoard[i][j].placed) fullRow = false;
            if (!this->_gameBoard[j][i].placed) fullCol = false;
        }

        if (fullRow) 
        {
            rmRows.push_back(i);
        }
        if (fullCol) 
        {
            rmCols.push_back(i);
        }
    }

    //clear identified rows
    for (auto row : rmRows) {
        for (int i = 0; i < SIZE; i++) {
            this->_gameBoard[row][i].placed = false;
            this->_gameBoard[row][i].color = EMPTY;
        }
    }

    //clear identified cols
    for (auto col : rmCols) {
        for (int i = 0; i < SIZE; i++) {
            this->_gameBoard[i][col].placed = false;
            this->_gameBoard[i][col].color = EMPTY;
        }
    }

    return !rmRows.empty() || !rmCols.empty(); // Return true if something was removed
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
    if((peice->getCols() - 1 + col > SIZE - 1) || (peice->getRows() - 1 + row > SIZE - 1))
    {
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
            if(peice->GetPeice()[j][i] == true)
            {
                this->_gameBoard[row + j][col + i].placed = true;
                this->_gameBoard[row + j][col + i].color = ColorMap(randomColor);
            }
        }
    }

    return "";
}