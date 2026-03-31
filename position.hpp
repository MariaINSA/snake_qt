#ifndef POSITION_HPP
#define POSITION_HPP

// Directions possibles pour le mouvement du serpent
typedef enum {GAUCHE, HAUT,DROITE, BAS} Direction;

class Position
{
  public:
    int x, y; // Coordonnées de la case

    Position();        // Constructeur par défaut (0,0)
    Position(int,int); // Constructeur spécifique (x,y)

    // Opérateurs de comparaison entre deux positions
    bool operator==(const Position &) const;
    bool operator!=(const Position &) const;

    // Détermine la direction relative vers une autre position
    Direction quadrant(const Position &) const;
};

#endif