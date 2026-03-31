#include "fruit.hpp"
#include <iostream>
#include "jeu.hpp" // Now we include the full Jeu definition
using namespace std;
void Fruit::onEaten(Jeu& jeu) {
    //on a normal situation -> you eat the fruit:
    // score +1 and the snake goes longer
    jeu.upScore();
}
void Banana::onEaten(Jeu& game) {
        Fruit::onEaten(game);
        game.reverseSnake();
}

void Fraise::onEaten(Jeu& game){
    Fruit::onEaten(game);
    game.setDrillActive();
}