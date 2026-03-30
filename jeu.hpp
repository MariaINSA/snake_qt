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
    std::list<Position> snake2;
    Direction dirSnake, dirSnake2;
    bool mort1, mort2;
    bool mangeur; //who ate the fruit?? 0-> 1, 1->2
    int score, score2;
    Fruit *currFruit; // bool mangeFruit;

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
    const std::list<Position> &getSnake2() const;
    // Retourne la fruit en lecture seule
    const Fruit* getFruit() const;

    // Indique si la case a une position donnee existe et est libre
    bool posValide(const Position &) const;
    bool posValide2(const Position &) const;

    // Modifie la direction
    void setDirection(Direction);
    void setDirection2(Direction);

    bool getMort1() const;
    bool getMort2() const;

    int getScore1() const;
    int getScore2() const;

    //ajouter mur
    void ajouterMur();
    //suprimer mur
    void supprimerMur();


    //augmenter le score
    void upScore();
    // creer une nouveau fruit
    void new_fruit();

    //changer l'ordre de le snake
    void reverseSnake();

    // Retourne la direction
    Direction getDirection() const;
    Direction getDirection2() const;

};

#endif
