#include <iostream>
#include <cassert>
#include "position.hpp"

using namespace std;

Position::Position()
{
}

Position::Position(int a, int b)
{
    x=a;
    y=b;
}

bool Position::operator==(const Position &pos) const
{
    return (x==pos.x && y==pos.y);
}

bool Position::operator!=(const Position &pos) const
{
    return (x!=pos.x || y!=pos.y);
}

Direction Position::quadrant(const Position &pos) const{
    // 0:GAUCHE, 1: DROITE, 2:HAUT, 3:BAS
    int diffx = pos.x - this->x;
    int diffy = pos.y - this->y;

    //if (pos==*this){return -1;} //same pos (should never happen !!!!)
    // is it to the right/left?
    if (abs(diffx) > abs(diffy)) {
        return (diffx > 0) ? DROITE : GAUCHE;
    //is it up or down?
    } else{
        return (diffy > 0) ? BAS : HAUT;
    }
}
