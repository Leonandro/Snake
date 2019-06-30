#include "../include/game.h"

#include <iostream>
#include <string>

int main (int argc, char *argv[]){

    if(argc > 1)
    {    
        std::string flag{argv[1]};
        if(flag.find("-a") != std::string::npos){
            if( argc > 2)
            {
                std::string dir{argv[2]};
                game joguinho(dir);
                joguinho.update();
            }
        }
    }

    else
    {
        game joguinho("../input/game1.dat");
        joguinho.update();
    }
    
}
