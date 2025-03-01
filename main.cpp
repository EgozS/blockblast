#include "board.h"
#include "Iblock.h"
#include "player.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

using namespace ftxui;

#define GRID_SIZE 8

enum SIDES {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

typedef struct point 
{
    int row;
    int col;

    bool operator==(point a)
    {
        if(this->col == a.col && this->row == a.row)
        {
            return true;
        }
        return false;
    }
} point;

point GetSideMax(std::vector<point> pointsVec, SIDES side);

point GetSideMax(std::vector<point> pointsVec, SIDES side)
{
    point maxColPoint;
    switch (side)
    {
    case TOP:
        return pointsVec[0];
        break;
    case BOTTOM:
        return pointsVec[pointsVec.size()-1];
        break;
    case LEFT:
        for(auto point : pointsVec)
        {
            if(point.col == 0)
            {
                return point;
            }
        }
        return pointsVec[0];
        break;
    case RIGHT:
        for(auto point : pointsVec)
        {
            if(point.col == GRID_SIZE - 1)
            {
                return point;
            }
        }
        return pointsVec[0];
        break;
    
    default:
        return pointsVec[0];
        break;
    }

    return maxColPoint;
}

// Game state
const BlockPlacement (*grid)[GRID_SIZE] = nullptr;
point firstPoint {1,1};
std::vector<point> selectedPoints;
int score = 0;
std::vector<Iblock*> items;  // Item slots
int selected_item_index = -1;
std::string note = "";
Iblock* block = nullptr;
Iblock* usedBlock = new Iblock(".");


// Function to render the grid
Element RenderGrid() {
    std::vector<Element> rows;
    for (int r = 0; r < GRID_SIZE; ++r) {
        std::vector<Element> cells;
        for (int c = 0; c < GRID_SIZE; ++c) {
            bool is_selected = (std::find(selectedPoints.begin(), selectedPoints.end(), point{r,c}) != selectedPoints.end());
            auto cell = text(board::GetColor(grid[r][c].color)) | border;
            if (is_selected) {
                cell = cell | inverted;
            }
            cells.push_back(cell);
        }
        rows.push_back(hbox(cells));
    }
    return vbox(rows);
}

// Function to render the item slots
// Function to render the item slots with a larger text box for multi-line display
Element RenderItems() {
    std::vector<Element> item_rows;
    const int items_per_row = 3;  // Number of items per row
    const int max_lines = 5;      // Maximum number of lines for item box

    // Split the items into rows
    for (size_t i = 0; i < items.size(); i += items_per_row) {
        std::vector<Element> item_boxes;
        for (size_t j = i; j < i + items_per_row && j < items.size(); ++j) {
            // Create a string representation of the item (split into lines if needed)
            std::string item_text = items[j]->ToString();

            // Create multiple lines (up to max_lines)
            std::vector<std::string> lines;
            size_t start = 0;
            while (start < item_text.size() && lines.size() < max_lines) {
                size_t end = item_text.find('\n', start);
                if (end == std::string::npos) end = item_text.size();
                lines.push_back(item_text.substr(start, end - start));
                start = end + 1;
            }

            // Create a box for the item with a fixed height and enough space for up to max_lines
            std::vector<Element> item_lines;
            for (const auto& line : lines) {
                item_lines.push_back(text(line) | center);
            }

            // Check if this item is selected
            bool is_selected = (j == selected_item_index);

            // Style the item box (highlight selected item with inverted colors)
            Element item_box = vbox(item_lines) | border | size(WIDTH, GREATER_THAN, 10) | size(HEIGHT, GREATER_THAN, 3);
            if (is_selected) {
                item_box = item_box | inverted | bold | bgcolor(Color::Yellow);  // Highlight selected item
            }

            item_boxes.push_back(item_box);
        }
        item_rows.push_back(hbox(item_boxes));
    }

    return vbox(item_rows) | center;
}


// Function to render the score display
Element RenderScore() {
    return text("Score: " + std::to_string(score)) | bold | center;
}

Element RenderNote() {
    return text(note) | bold | center;
}

class GridComponent : public ComponentBase {
public:
    GridComponent(board* gameBoard, player* player) : _gameBoard(gameBoard), _player(player) {
        Add(Renderer([&] { 
            return vbox({
                RenderGrid(), 
                separator(),
                RenderItems(),
                separator(),
                RenderScore(),
                RenderNote(),
            }) | center;
        }));
    }

    bool OnEvent(Event event) override {
        int i = 0;
        int j = 0;
        bool selectedItem = false;

        if (event == Event::ArrowUp) {
            if(GetSideMax(selectedPoints, TOP).row != 0) //if not at top
            {
                for(auto& point : selectedPoints)
                {
                    point.row--;
                }
            }
            //note = std::to_string(selectedPoints[i].row) + std::to_string(selectedPoints[i].col);
        }
        if (event == Event::ArrowDown) {
            if(GetSideMax(selectedPoints, BOTTOM).row != GRID_SIZE - 1) //if not at bottom
            {
                for(auto& point : selectedPoints)
                {
                    point.row++;
                }
            }
            //note = std::to_string(selectedPoints[i].row) + std::to_string(selectedPoints[i].col);
        }
        if (event == Event::ArrowLeft) {
            if(GetSideMax(selectedPoints, LEFT).col != 0) //if not at left most side
            {
                for(auto& point : selectedPoints)
                {
                    point.col--;
                }
            }
            //note = std::to_string(selectedPoints[i].row) + std::to_string(selectedPoints[i].col);
        }
        if (event == Event::ArrowRight) { 
            if(GetSideMax(selectedPoints, RIGHT).col != GRID_SIZE - 1) //if not at right most side
            {
                for(auto& point : selectedPoints)
                {
                    point.col++;
                }
            }
            //note = std::to_string(selectedPoints[i].row) + std::to_string(selectedPoints[i].col);
        }

        // Handling item selection using 1, 2, 3 keys
        if (event == Event::Character('1')) {
            if (!items.empty() && items[0]->getPlaceable()) {
                // Select the first item
                block = items[0];

                if(selected_item_index != 0)
                {
                    selectedItem = true;
                }
                selected_item_index = 0;
                //note = std::to_string(selected_item_index);
                
            }
        }
        if (event == Event::Character('2')) {
            if (items.size() > 1 && items[1]->getPlaceable()) {
                // Select the second item
                block = items[1];

                if(selected_item_index != 1)
                {
                    selectedItem = true;
                }
                selected_item_index = 1;
                //note = std::to_string(selected_item_index);
            }
        }
        if (event == Event::Character('3')) {
            if (items.size() > 2 && items[2]->getPlaceable()) {
                // Select the third item
                block = items[2];

                if(selected_item_index != 2)
                {
                    selectedItem = true;
                }

                selected_item_index = 2;
                //note = std::to_string(selected_item_index);
            }
        }

        if (event == Event::Character('\n')) //trying to palce
        {
            if(selected_item_index != -1)
            {
                note = this->_gameBoard->palcePeice(block, selectedPoints[0].row, selectedPoints[0].col);
                if(note == "")
                {
                    this->_gameBoard->UpdateBoard();
                    items[selected_item_index] = usedBlock;
                    selected_item_index = -1;
                    selectedPoints.clear();
                    
                    for (auto& item : items)
                    {
                        if (item->getPlaceable())
                        {
                            return true;
                        }
                    }
                    items.clear();
                    auto blocks = this->_player->GetBlocks();
                    for(auto block : blocks)
                    {
                        items.push_back(block);
                    }
                }
            }
        }

        if(selectedItem)
        {
            auto blockTable = block->GetPeice();

            selectedPoints.clear();
                
            for(i = 0; i < block->getRows(); i++)
            {
                for(j = 0; j < block->getCols(); j++)
                {
                    if(blockTable[i][j])
                    {
                        selectedPoints.push_back(point{firstPoint.row + i, firstPoint.col + j});                        
                    }
                }
            }
        }

        return true;
    }

private:
    board* _gameBoard;
    player* _player;
};

int main() {
    selectedPoints.push_back(firstPoint);
    board gameBoard;
    player player(&gameBoard);
    grid = gameBoard.GetBoard();
    usedBlock->setUnplaceable();
    auto blocks = player.GetBlocks();
    for(auto block : blocks)
    {
        items.push_back(block);
    }
    

    auto screen = ScreenInteractive::TerminalOutput();
    auto grid_component = Make<GridComponent>(&gameBoard, &player);
    screen.Loop(grid_component);

    return 0;
}
