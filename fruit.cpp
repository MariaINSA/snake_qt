#include "fruit.hpp"
#include "jeu.hpp" // Now we include the full Jeu definition
void Fruit::onEaten(Jeu& jeu) {
    //on a normal situation -> you eat the fruit:
    // score +1 and the snake goes longer
    jeu.upScore();
}
