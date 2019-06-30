#include "../include/grid.h"
#include <iostream>

std::ostream & operator<<(std::ostream & os, Grid  grid){
	std::cout << "IHUuuuu" << std::endl;
    for (int y = 0; y< grid.map.size(); y++){
        for(int x = 0; x< grid.map[y].size(); x++  ){
            if (grid[y][x].tipo == Cell::type::free){
                os<< " ";
            }else if(grid[y][x].tipo == Cell::type::snake_head){
                os<< "\033[1;32m\u25CF\033[0m";
            }else if(grid[y][x].tipo == Cell::type::snake_body){
                os<<"\033[1;32m\u25CF\033[0m";
            }
            else if (grid[y][x].tipo == Cell::type::food)
                os<< "\033[1;33m\u2707\033[0m";
            else if (grid[y][x].tipo == Cell::type::wall){
                os <<"\u26BC";
            }
            else if (grid[y][x].tipo == Cell::type::invisible_wall){
                os <<" ";
            }
            else{
                os << " ";
            }
        }
        os<<"\n"; 
    }
    return os;
}

std::vector<Cell>& Grid::operator[](size_t pos){
    return map[pos];
}

unsigned int Grid::get_size()
{
    return map.size();
}

unsigned int Grid::get_columns()
{
    return map[0].size();
}

Grid::Grid()
{
    int a = 0;
}



Grid::Grid(int x, int y, Snake snake): snake{snake}{
    
    map.resize(y);
    for (int j = 0; j < y; j++){
        map[j].resize(x);
        for (int i = 0; i < x; i++){
            Cell aux(i,j);
            map[j][i] = aux;
            map[j][i].tipo = Cell::type::free;
        }
    }
    
    std::pair<int,int> head = snake.getheadcoords();

    map[head.second][head.first].tipo = Cell::type::snake_head;
}

void Grid::set_map_size(int x, int y)
{
    map.resize(y);
    for (int j = 0; j < y; j++){
        map[j].resize(x);
        for (int i = 0; i < x; i++){
            Cell aux(i,j);
            map[j][i] = aux;
            //map[j][i].tipo = Cell::type::free;
        }
    }
}

void Grid::set_cell_type(int x, int y, Cell::type atual_status)
{
    map[x][y].tipo = atual_status;
}

void Grid::update()
{
    auto it = snake.begin();
     for (int y = 0; y< map.size(); y++){
        for(int x = 0; x< map[y].size(); x++  ){
            if( map[y][x].tipo != Cell::type::food)
                map[y][x].tipo = Cell::type::free;
        }
    }

    map[(*it).second][(*it).first].tipo = Cell::type::snake_head;
    it++;
    while (it != snake.end())
    {
        map[(*it).second][(*it).first].tipo = Cell::type::snake_body;
        
        it++;
    }
}

