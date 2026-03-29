#ifndef FRUIT_HPP
#define FRUIT_HPP

//#include "jeu.hpp"
#include "position.hpp"
#include <list>

class Jeu;

typedef enum {POMME, BANANA, FRAISE} Fruit_type;

class Fruit {
public:
    Fruit(Position position, Fruit_type ftype) : pos(position), fruit_type(ftype){}
    
    virtual ~Fruit() {}
    virtual void onEaten(Jeu& game);

    Position getPos() const { return pos; }
    Fruit_type getType() const { return fruit_type; }

protected:
    Position pos;
    Fruit_type fruit_type;
};

class Banana : public Fruit {
public:
    using Fruit::Fruit; // Inherit the constructor
    void onEaten(Jeu& game) override;/* {
        Fruit::onEaten(game); // Still get the score!
        game.reverseSnake();  // Special banana power
    }*/
};

class Fraise : public Fruit {
public:
    using Fruit::Fruit;

    void onEaten(Jeu& game) override {
        Fruit::onEaten(game);
        //game.slowDown(200);   // Special slow power
    }
};



#endif