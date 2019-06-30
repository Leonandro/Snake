#ifndef SNAKE_H
#define SNAKE_H
#include "list.h"
#include "cell.h"
#include <utility>
#include <algorithm>
#include <stack>
#include <deque>
#include <list>

#define PI 3.14159265

class Snake{

    private:
    /**
     * @brief A linked list of pair <int, int> to represente the positions of eache peace of the snake body
     * 
     */

    public:
    std::list< std::pair <int,int> > body;
    std::deque<std::pair<int, int>> path;
    std::deque<std::pair<int, int>> visited;
    std::deque<char> directions;

    /**
     * @brief Moves the body of the snake in the direction of the geometric vector <x,y>
     * 
     * @param x How the snake should move in the x axis
     * @param y How the snake should move in the y axis
     */
    void walk(std::pair <int, int>);

    Snake(int x, int y);

    Snake();

    /**
     * @brief Increse the body of the snake at 1 block
     * 
     */
    void grow (std::pair <int, int>);

    /**
     * @brief Set the snake head
     */
    void set_snake(int, int);

    std::pair<int,int> getheadcoords();



    std::list< std::pair <int,int> >::iterator begin();
    std::list< std::pair <int,int> >::iterator end();

    /**
     * @brief Opertor << overloading for the snake body
     * 
     * @return std::ostream& 
     */
    friend std::ostream & operator<<(std::ostream &, Snake);

};

#endif