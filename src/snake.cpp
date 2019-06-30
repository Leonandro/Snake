#include "../include/snake.h"
#include <iostream>

Snake::Snake(int x, int y){
    body.push_back( std::pair<int,int>(x,y) );
    path.push_front(std::pair<int,int>(x,y));
    directions.push_front('n');
}

Snake::Snake()
{
    int a = 0;
}

void Snake::set_snake(int x, int y)
{
    body.push_back( std::pair<int,int>(x,y) );
    path.push_front(std::pair<int,int>(x,y));
    directions.push_front('n');
}

void Snake::walk(std::pair <int, int> move_cell)
{
    body.pop_back();
    body.push_front(std::pair<int, int>{move_cell.first, move_cell.second});
}

void Snake::grow(std::pair <int, int> grow_cell)
{
    body.push_front(grow_cell);
}


std::pair<int,int> Snake::getheadcoords(){
    return body.front();
}

std::list< std::pair <int,int> >::iterator Snake::begin(){
    return body.begin();
}

std::list< std::pair <int,int> >::iterator Snake::end(){
    return body.end();
}

std::ostream& operator<<(std::ostream& os, Snake s)
{
    auto it = s.begin();
    os << "{ ";
    while(it!=s.end()){
        os <<"< "<< (*it).first <<", " << (*it).second<<" > ";
        it++;
    }
    os <<"}";

    return os;
}

