#ifndef FRUIT_HPP
#define FRUIT_HPP

//#include "jeu.hpp"
#include "position.hpp"
#include <list>

// Déclaration de jeu
class Jeu;

// Types de fruits
typedef enum {POMME, BANANA, FRAISE} Fruit_type;

// Classe de base pour les fruits
class Fruit {
public:
    Fruit(Position position, Fruit_type ftype) : pos(position), fruit_type(ftype){}
    
    virtual ~Fruit() {}
    // Effet quand le fruit est mangé (upScore)
    virtual void onEaten(Jeu& game);

    Position getPos() const { return pos; }
    Fruit_type getType() const { return fruit_type; }

protected:
    Position pos;
    Fruit_type fruit_type;
};

// Fils : Banane
class Banana : public Fruit {
public:
    using Fruit::Fruit; // Inherit the constructor
    void onEaten(Jeu& game) override;
};

// Fils : Fraise
class Fraise : public Fruit {
public:
    using Fruit::Fruit;
    void onEaten(Jeu& game) override;
};



#endif