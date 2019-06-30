cmake_minimum_required(VERSION 3.5)
project (snaze VERSION 1.0.0 LANGUAGES CXX )


#--------------------------------
# This is for old cmake versions
set (CMAKE_CXX_STANDARD 11)
#--------------------------------


#=== Driver target ===

add_executable( game "src/grid.cpp" "src/cell.cpp" "src/main.cpp" "src/snake.cpp" "src/game.cpp")