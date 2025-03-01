#include "player.h"

player::player(board* board)
{
    this->_score = 0;   
    this->_board = board;
    this->RegenerateBlocks();
}

player::~player()
{

}

void player::RegenerateBlocks()
{
    this->_blocks.clear();
    int randomBlock = board::random(0, this->_board->GetPeices().size() - 1);
    this->_blocks.push_back(this->_board->GetPeices()[randomBlock]);
    randomBlock = board::random(0, this->_board->GetPeices().size() - 1);
    this->_blocks.push_back(this->_board->GetPeices()[randomBlock]);
    randomBlock = board::random(0, this->_board->GetPeices().size() - 1);
    this->_blocks.push_back(this->_board->GetPeices()[randomBlock]);
}

void player::AddToScore(int score)
{
    this->_score += score;
}

std::vector<Iblock*> player::GetBlocks()
{
    return this->_blocks;
}