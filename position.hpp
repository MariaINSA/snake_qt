#ifndef POSITION_HPP
#define POSITION_HPP

typedef enum {GAUCHE, HAUT,DROITE, BAS} Direction;

class Position
{
  public:
    int x, y;
    Position();
    Position(int,int);
    bool operator==(const Position &) const;
    bool operator!=(const Position &) const;
    Direction quadrant(const Position &) const;
};

#endif