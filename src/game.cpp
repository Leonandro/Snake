#include "../include/game.h"
#include <iostream>
#include <cmath>
#include <ostream>
#include <unistd.h>
#include <algorithm>


game::game(std::string pat_)
{
    std::fstream file;
    file.open(pat_);
    int lines = 0, columns = 0;
    num_levels = 0;
    if(file.is_open())
    {
        while (file >> columns >> lines)
        {
            std::string e;
            std::getline(file, e);

            Grid level_;
            level_.set_map_size(lines, columns);
            for (int i = 0; i < columns; i++)
            {
                std::string line;
                std::getline(file, line);
                for (int j = 0; j < lines; j++)
                {
                    if (line[j] == '#')
                    {
                        level_[i][j].tipo = Cell::wall;
                    }
                    else if (line[j] == ' ')
                    {
                        level_[i][j].tipo = Cell::free;
                    }
                    else if (line[j] == '.')
                    {
                        level_[i][j].tipo = Cell::invisible_wall;
                    }
                    else if (line[j] == '*')
                    {
                        fdp.push_back(Snake(i, j));
                        level_[i][j].tipo = Cell::snake_head;
                        spawns.push_back(std::pair<int, int> {i, j});
                    }
                }
            }
            //level[4][20].tipo = Cell::type::food;
            game_levels.push_back(level_);
            num_levels++;
        }
        level = 0;
        apples = 10;
        lives = 5;

        set_food_coord();
    }
}

void game::render()
{
    std::cout << game_levels[level] << std::endl;
}

void game::update()
{
    system("clear");
    callheader();
    render();
    std::cin.get();
    while (apples != 0)
    {
        if(find_path(game_levels[level], x_food_coord, y_food_coord))
        {
            while(!( fdp[level].path.size() == 1 ) )
            {
                fdp[level].path.pop_back();
                fdp[level].directions.pop_back();
                show_snake();
                usleep(80000);
                system("clear");
                callheader();
                render();
            }
            apples--;
            set_food_coord();
            fdp[level].visited.clear();
            fdp[level].grow(fdp[level].path.back());
        }
        else
        { 
            dumb_move();
            while(!( fdp[level].path.size() == 1 ) )
            {
                fdp[level].path.pop_back();
                fdp[level].directions.pop_back();
                show_snake();
                usleep(80000);
                system("clear");
                callheader();
                render();

            }

            lives--;

            if(lives > 0)
            {
                fdp[level].path.clear();
                fdp[level].directions.clear();
                fdp[level].visited.clear();
                fdp[level].body.clear();
                reset_maze();
                fdp[level].set_snake(spawns[level].first, spawns[level].second);
                std::cout << "\033[1;31mYOU DIE (NO SOLUTIONS)\033[0m\nPress Enter to continue";
                std::cin.get();
                system("clear");
                update();
            }
            else
            {
                std::cout << "\033[1;31mYOU LOOSE\033[0m\n";
                std::quick_exit(1);
            }
        }
    }
    level++;
    if(level < num_levels) 
    {
        fdp[level].path.clear();
        fdp[level].directions.clear();
        fdp[level].visited.clear();
        fdp[level].body.clear();
        fdp[level].set_snake(spawns[level].first, spawns[level].second);
        set_food_coord();
        apples = 10;
        update();
    }

    else
    {
        std::cout << "\033[1;32m YOU WIN\033[0m\n";
    }
    
    
}

bool game::find_path(Grid & maze, size_t food_x, size_t food_y)
{
    //bool shits_happen = false;
    while( ( ( fdp[level].path.front().first != food_x ) || ( fdp[level].path.front().second != food_y ) ))
    {
        if(possible_moves(maze, fdp[level].path.front().first, fdp[level].path.front().second) != 0)
        {
            move_snake(maze, fdp[level].path.front().first, fdp[level].path.front().second, fdp[level].directions.front());
        }

        else
        {
            if(fdp[level].path.size() == 1) return false;

            else backtrack();
        }
        
    }
    return true;
}

size_t game::possible_moves(Grid & maze, size_t x, size_t y)
{
    size_t possible_ways = 0;
    if(fdp[level].directions.front() != 's')
    {
        if(x > 0)
        {
            if(maze[x - 1][y].tipo == Cell::type::free or maze[x - 1][y].tipo == Cell::type::food ) 
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x - 1, y}) == fdp[level].visited.end())
                {
                    possible_ways++;
                }
            }
        }
    }

    if(fdp[level].directions.front() != 'n')
    {
        if(x < maze.get_size())
        {
            if(maze[x + 1][y].tipo == Cell::type::free or maze[x + 1][y].tipo == Cell::type::food )
            
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x + 1, y}) == fdp[level].visited.end())
                {    
                    possible_ways++;
                }
        }
    }

    if(fdp[level].directions.front() != 'o')
    {
        if(x < maze.get_columns())
        {
            if(maze[x][y + 1].tipo == Cell::type::free or maze[x][y + 1].tipo == Cell::type::food )
            
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x, y + 1}) == fdp[level].visited.end())
                {    
                    possible_ways++;
                }
        }
    }

    if(fdp[level].directions.front() != 'l')
    {
        if(x > 0)
        {
            if(maze[x][y - 1].tipo == Cell::type::free or maze[x][y - 1].tipo == Cell::type::food ) 
            
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x, y - 1}) == fdp[level].visited.end())
                {
                    possible_ways++;
                }
        }
    }

    return possible_ways;
}

void game::move_snake(Grid & maze, size_t x, size_t y, char dir)
{
    int menor_val = 9999;
    int menor_id = 0;
    if(dir == 'n')
    {
        if( x > 0 )
        {
            if(maze[x - 1][y].tipo == Cell::type::free || maze[x - 1][y].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x - 1, y}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x - 1)) + abs(y_food_coord - y) ) < menor_val)
                    {
                        menor_id = 1;
                        menor_val = (abs(x_food_coord - (x - 1)) + abs(y_food_coord - y) );
                    }
                }
            }

        }

        if( y < maze.get_columns() )
        {
            if(maze[x][y + 1].tipo == Cell::type::free || maze[x][y + 1].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x, y + 1}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x)) + abs(y_food_coord - (y + 1) ) ) < menor_val)
                    {
                        menor_id = 3;
                        menor_val = (abs(x_food_coord - (x)) + abs(y_food_coord - (y + 1)) );
                    }
                }
            }
        }

        if( y > 0)
        {
            if(maze[x][y - 1].tipo == Cell::type::free || maze[x][y - 1].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x, y - 1}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x)) + abs(y_food_coord - (y - 1) ) < menor_val) )
                    {
                        menor_id = 4;
                        menor_val = (abs(x_food_coord - (x)) + abs(y_food_coord - (y - 1) ) );
                    }
                }
            }
        }

    }

    else if (dir == 's')
    {
        if( x < maze.get_size() )
        {
            if(maze[x + 1][y].tipo == Cell::type::free || maze[x + 1][y].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x + 1, y}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x + 1)) + abs(y_food_coord - y) ) < menor_val)
                    {
                        menor_id = 2;
                        menor_val = (abs(x_food_coord - (x + 1)) + abs(y_food_coord - y) );
                    }
                }
            }

        }

        if( y < maze.get_columns() )
        {
            if(maze[x][y + 1].tipo == Cell::type::free || maze[x][y + 1].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x, y + 1}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x)) + abs(y_food_coord - (y + 1) ) ) < menor_val)
                    {
                        menor_id = 3;
                        menor_val = (abs(x_food_coord - (x)) + abs(y_food_coord - (y + 1)) );
                    }
                }
            }
        }

        if( y > 0)
        {
            if(maze[x][y - 1].tipo == Cell::type::free || maze[x][y - 1].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x, y - 1}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x)) + abs(y_food_coord - (y - 1) ) ) < menor_val)
                    {
                        menor_id = 4;
                        menor_val = (abs(x_food_coord - (x)) + abs(y_food_coord - (y - 1)) );
                    }
                }
            }
        }
    }

    else if(dir == 'l')
    {
        if( y < maze.get_columns() )
        {
            if(maze[x][y + 1].tipo == Cell::type::free || maze[x][y + 1].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x, y + 1}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x)) + abs(y_food_coord - (y + 1) ) ) < menor_val)
                    {
                        menor_id = 3;
                        menor_val = (abs(x_food_coord - (x)) + abs(y_food_coord - (y + 1) ) );
                    }
                }
            }
        }

        if( x > 0 )
        {
            if(maze[x - 1][y].tipo == Cell::type::free || maze[x - 1][y].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x - 1, y}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x - 1)) + abs(y_food_coord - y) ) < menor_val)
                    {
                        menor_id = 1;
                        menor_val = (abs(x_food_coord - (x - 1)) + abs(y_food_coord - y) );
                    }
                }
            }

        }

        if( x < maze.get_size() )
        {
            if(maze[x + 1][y].tipo == Cell::type::free || maze[x + 1][y].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x + 1, y}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x + 1)) + abs(y_food_coord - y) ) < menor_val)
                    {
                        menor_id = 2;
                        menor_val = (abs(x_food_coord - (x + 1)) + abs(y_food_coord - y) );
                    }
                }
            }

        }
    }

    else if (dir == 'o')
    {
        if( y > 0)
        {
            if(maze[x][y - 1].tipo == Cell::type::free || maze[x][y - 1].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x, y - 1}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x)) + abs(y_food_coord - (y - 1) ) ) < menor_val)
                    {
                        menor_id = 4;
                        menor_val = (abs(x_food_coord - (x)) + abs(y_food_coord - (y - 1) ) );
                    }
                }
            }
        }

        if( x > 0 )
        {
            if(maze[x - 1][y].tipo == Cell::type::free || maze[x - 1][y].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x - 1, y}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x - 1)) + abs(y_food_coord - y) ) < menor_val)
                    {
                        menor_id = 1;
                        menor_val = (abs(x_food_coord - (x - 1)) + abs(y_food_coord - y) );
                    }
                }
            }

        }

        if( x < maze.get_size() )
        {
            if(maze[x + 1][y].tipo == Cell::type::free || maze[x + 1][y].tipo == Cell::type::food)
            {
                if(std::find(fdp[level].visited.begin(), fdp[level].visited.end(), std::pair<int, int> {x + 1, y}) == fdp[level].visited.end())
                {
                    if( (abs(x_food_coord - (x + 1)) + abs(y_food_coord - y) ) < menor_val)
                    {
                        menor_id = 2;
                        menor_val = (abs(x_food_coord - (x + 1)) + abs(y_food_coord - y) );
                    }
                }
            }

        }
    }

    if(menor_id == 1)
    {
        fdp[level].path.push_front(std::pair<int, int> {x-1, y});
        fdp[level].visited.push_front(std::pair<int, int> {x-1, y});
        fdp[level].directions.push_front('n');
        maze[fdp[level].body.back().first][fdp[level].body.back().second].tipo = Cell::type::free;
        return;
    }

    else if(menor_id == 2)
    {
        fdp[level].path.push_front(std::pair<int, int> {x+1, y});
        fdp[level].visited.push_front(std::pair<int, int> {x+1, y});
        fdp[level].directions.push_front('s');
        maze[fdp[level].body.back().first][fdp[level].body.back().second].tipo = Cell::type::free;
        return;
    }

    else if (menor_id == 3)
    {
        fdp[level].path.push_front(std::pair<int, int> {x, y + 1});
        fdp[level].visited.push_front(std::pair<int, int> {x, y + 1});
        fdp[level].directions.push_front('l');
        maze[fdp[level].body.back().first][fdp[level].body.back().second].tipo = Cell::type::free;
        return;
    }

    else if(menor_id == 4)
    {
        fdp[level].path.push_front(std::pair<int, int> {x, y - 1});
        fdp[level].visited.push_front(std::pair<int, int> {x, y - 1});
        fdp[level].directions.push_front('o');
        maze[fdp[level].body.back().first][fdp[level].body.back().second].tipo = Cell::type::free;
        return;
    }
}

void game::backtrack()
{
    fdp[level].path.pop_front();
    fdp[level].directions.pop_front();
}

void game::set_food_coord()
{
    srand(time(NULL));
    x_food_coord = 0; 
    y_food_coord = 0;
    while (game_levels[level][x_food_coord][y_food_coord].tipo != Cell::type::free)
    {
        x_food_coord = rand() % game_levels[level].get_size();
        y_food_coord = rand() % game_levels[level].get_columns();
    }

    game_levels[level][x_food_coord][y_food_coord].tipo = Cell::type::food;
}

void game::show_snake()
{
    game_levels[level][fdp[level].body.back().first][fdp[level].body.back().second].tipo = Cell::type::free;

    fdp[level].walk(fdp[level].path.back() );
    
    for (auto a : fdp[level].body)
    {
        game_levels[level][a.first][a.second].tipo = Cell::type::snake_body;
    }
}

void game::dumb_move()
{
    while(possible_moves(game_levels[level], fdp[level].path.front().first, fdp[level].path.front().second) != 0)
    {
        move_snake(game_levels[level], fdp[level].path.front().first, fdp[level].path.front().second, fdp[level].directions.front());
    }
}

void game::callheader()
{
    std::cout << "\033[1;33m        ---> Welcome to the classic Snake Game <--- \033[0m\n";
    std::cout << "\033[1;33m ----------------------------------------------------------- \033[0m\n";
    std::cout << "\033[1;33m Current Level: \033[0m"  << "\033[1;32m"<< level << "\033[0m";
    std::cout << "\033[1;33m  |  Snake lives: \033[0m" << "\033[1;32m"<< lives << "\033[0m";
    std::cout << "\033[1;33m  |  Apples to eat: \033[0m" << "\033[1;32m"<< apples << "\033[0m";
    std::cout << "\033[1;33m\n ----------------------------------------------------------- \033[0m\n";
    std::cout << "\033[1;33m >>> Press <ENTER> to start the game! \033[0m\n";
    std::cout << "\033[1;33m\n\n Lives: \033[0m";
    for(int i = 0; i < lives; i++)
    std::cout << "\033[1;31m"<< "♡ " << "\033[0m";
    std::cout << "\033[1;33m  |  Sentido:    \033[0m" << "\033[1;32m"<< fdp[level].directions.back() << "\033[0m";
    std::cout << "\033[1;33m  |  Food eaten: \033[0m" << "\033[1;32m"<< 10 - apples << "\033[0m" << "\033[1;32m of 10\n\033[0m";

}

void game::reset_maze()
{
    for(size_t x = 0; x < game_levels[level].get_size(); x++)
    {
        for(size_t y = 0; y < game_levels[level].get_columns(); y++ )
        {
            if(game_levels[level][x][y].tipo != Cell::type::wall 
                && game_levels[level][x][y].tipo != Cell::type::invisible_wall 
                && game_levels[level][x][y].tipo != Cell::type::food)
            {
                game_levels[level][x][y].tipo = Cell::type::free;
            }
        }
    }
}