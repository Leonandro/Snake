
#ifndef GRID_H
#define GRID_H
#include <vector>
#include <ostream>
#include <sstream>
#include "cell.h"
#include "snake.h"

class Grid{

    private:
    std::vector< std::vector <Cell> > map;


    public:
    Snake snake;

    /**
     * @brief Constructor
     * 
     */
    Grid (int, int, Snake);

    Grid();

    /**
     * @brief Set the map size
     * 
     * @param x 
     * @param y 
     */
    void set_map_size(int x, int y);

    /**
     * @brief Set one cell type in the map
     * 
     * @param x 
     * @param y 
     * @param atual_status 
     */
    void set_cell_type(int x, int y, Cell::type atual_status);

    friend std::ostream & operator << (std::ostream & os , Grid grid);

    std::vector<Cell>& operator[](size_t pos);

    void update ();

    unsigned int get_size();
    unsigned int get_columns();

    void read_config();

};

#endif

