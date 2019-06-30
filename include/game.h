#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "grid.h"
#include "cell.h"
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

/**
 * @brief The class who will run and control the game
 * 
 */
class game 
{

    private:
		std::vector<Grid> game_levels;
		std::vector <Snake> fdp;
        std::vector <std::pair<int, int>> spawns;
        int num_linhas, num_colunas;
        size_t apples;
        size_t level;
        size_t lives;
        size_t num_levels;
        int x_food_coord, y_food_coord;
    
    public:

        /**
         * @brief Construct a new game object
         * 
         * @param game_input the path to the config file 
         */
        game(std::string game_input);
        
        /**
         * @brief Process the evens, in this case, find the path or kill the snake. And render all moves.
         * 
         */
        void update();

        /**
         * @brief Show one snapshot of the the actual game maze
         * 
         */
        void render();

        /**
         * @brief print the header
         * 
         */
        void callheader();

        /**
         * @brief Find a way to the apple, using basic backtracking.
         * 
         * @return true if find a path
         * @return false  otherwise
         */
        bool find_path( Grid&, size_t, size_t);

        /**
         * @brief Calculate the possibles moves for the snake in consideration of the position and the direction.
         * 
         * @param maze the map game
         * @param x x-asis coord
         * @param y y-asiscoord
         * @return size_t the number os possibilities
         */
        size_t possible_moves(Grid& maze, size_t x, size_t y);

        /**
         * @brief Move the snake into a valid cell
         * 
          * @param maze the map game
         * @param x x-asis coord
         * @param y y-asiscoord
         * @param dir the actual direction of the snake
         */
        void move_snake(Grid& maze, size_t x, size_t y, char dir);

        /**
         * @brief make the backtrack of the snake to find a way fot the apple
         * 
         */
        void backtrack();

        /**
         * @brief Set the randomly the food in the maze
         * 
         */
        void set_food_coord();

        /**
         * @brief refresh the snake in the map
         * 
         */
        void show_snake();

        /**
         * @brief calculate a path to the death, commonly, the near path.
         * 
         */
        void dumb_move();

        /**
         * @brief clean the maze for the begin or redo in a level.
         * 
         */
        void reset_maze();

    
};


#endif
