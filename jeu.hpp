#ifndef JEU_HPP
#define JEU_HPP

#include <list>
#include "fruit.hpp"
#include "position.hpp"

typedef enum {VIDE, MUR} Case;

class Jeu
{
  protected:
    Case *terrain;
    int largeur, hauteur; // Nombre de cases en largeur et en hauteur
    std::list<Position> snake;
    Direction dirSnake;
    int score;
    Fruit *currFruit; bool mangeFruit;

  public:
    Jeu();
    Jeu(const Jeu &);
    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();
    void evolue();

    // Retourne les dimensions (en nombre de cases)
    int getNbCasesX() const;
    int getNbCasesY() const;

    // Retourne la case a une position donnee
    Case getCase(const Position &) const;

    // Retourne la liste des elements du serpent en lecture seule
    const std::list<Position> &getSnake() const;
    // Retourne la fruit en lecture seule
    const Fruit* getFruit() const;

    // Indique si la case a une position donnee existe et est libre
    bool posValide(const Position &) const;

    // Modifie la direction
    void setDirection(Direction);

    //augmenter le score
    void upScore();
    void new_fruit();

    // Retourne la direction
    Direction getDirection() const;

};

#endif
