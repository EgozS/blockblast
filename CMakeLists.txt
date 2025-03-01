cmake_minimum_required(VERSION 3.14)
project(MyFTXUIProject)

include(FetchContent)

# Fetch FTXUI
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(ftxui)

# Add your source files
add_executable(blocks main.cpp board.cpp Iblock.cpp player.cpp)

target_include_directories(blocks PRIVATE ${ftxui_SOURCE_DIR}/include)

# Link FTXUI libraries
target_link_libraries(blocks PRIVATE ftxui::screen ftxui::dom ftxui::component)
