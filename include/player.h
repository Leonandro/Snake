#ifndef PLAYER_H
#define PLAYER_H
#include "snake.h"
#include <vector>
#include "grid.h"
#include <utility>
#include "list.h"
class Player{
    
    private:
    std::vector<Grid> levels;
    
    int current_level;

    public: 
    std::vector<Snake> snake;
    
    std::pair<int,int> make_decision();

    Player(){
        current_level = 0;
    };

    void walk(int, int);

    double calculate_distance(  std::pair<int,int> std::pair<int,int>) ;
   
    void grow(int,int) ;

    void update();

    Grid get_current_level();

    void read_config(std::string);

    std::vector<Grid> get_levels();
};

#endif
