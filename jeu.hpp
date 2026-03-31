#ifndef JEU_HPP
#define JEU_HPP

#include <list>
#include "fruit.hpp"
#include "position.hpp"

// Type de contenu pour une case de la grille
typedef enum {VIDE, MUR} Case;

class Jeu
{
  protected:
    Case *terrain;                  // "Tableau" représentant la grille (0 et 1)
    int largeur, hauteur;           // Nombre de cases en largeur et en hauteur
    std::list<Position> snake;      // Liste des segments du serpent 1
    std::list<Position> snake2;     // Liste des segments du serpent 2
    Direction dirSnake, dirSnake2;  // Directions actuelles
    bool mort1, mort2;              // État de vie des joueurs
    bool mangeur;                   // Qui a mangé le fruit? (0->1 ou 1->2)
    int score, score2;              // Scores de chaque joueur
    Fruit *currFruit;               // Fruit sur la carte
    bool drillActive1, drillActive2;// Mode "perceuse" (mange les murs)
    int drillTimer1, drillTimer2;   // "Durée" restante du drill

  public:
    Jeu();
    Jeu(const Jeu &);
    ~Jeu();

    Jeu &operator=(const Jeu &);

    bool init();   // Initialise une nouvelle partie de jeu
    void evolue(); // Evolution du jeu chaque "tick" de timer (mouvement, collisions)

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

    // Modifie la direction
    void setDirection(Direction);
    void setDirection2(Direction);

    // Retourne si le jeueur est mort ou non
    bool getMort1() const;
    bool getMort2() const;

    // Retourne le score de chaque joueur
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

    //changer l'ordre du snake
    void reverseSnake();

    // Retourne la direction
    Direction getDirection() const;
    Direction getDirection2() const;

    // Gestion du effet Drill
    bool getDrill1() const;
    bool getDrill2() const;
    void setDrillActive();


};

#endif
