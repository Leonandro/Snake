#ifndef CELL_H
#define CELL_H

class Cell
{
    private:
    
    int x;
    
    int y;


    public:
    enum type { snake_body, wall, free , snake_head, food, invisible_wall};

    type tipo;
    
    Cell(int , int);
    Cell()
    {/*empty*/}

    Cell& operator=(Cell & rhs){
        x = rhs.x;
        y = rhs.y;
        return *this;
    }
};

#endif