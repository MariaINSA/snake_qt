#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

// Types de segments pour choisir le pixmap
typedef enum { HEAD, BODY, TAIL, CORNER, CORNER_TAIL, BEFORE_TAIL} snakeParts;

// bouton qui renvoie les touches flechees a la fenetre parent
class SnakeButton : public QPushButton
{
public:
    SnakeButton(const QString &texte, QWidget *parent = nullptr)
        : QPushButton(texte, parent) {}
protected:
    void keyPressEvent(QKeyEvent *e) override
    {
        if (parent() != nullptr)
            QCoreApplication::sendEvent(parent(), e);
    }
};

// Fenêtre principale du jeu
class SnakeWindow : public QFrame
{
  protected:
    static const int HAUTEUR_BOUTONS = 40;
    static const QStringList paths; // Declaration only

    // Ressources graphiques
    QPixmap* pixmapSnake; // Liste d'images pour le serpent
    QPixmap* pixmapFruits; // Liste d'images pour les fruits
    QPixmap pixmapMur; // Image pour les murs

    SnakeButton *btnAjouterMur;
    SnakeButton *btnSupprimerMur;
    Jeu jeu; // Instance de la logique de jeu
    

  public:
    //constructeur
    SnakeWindow(QWidget *pParent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~SnakeWindow();
    void handleTimer(); // Mise à jour à chaque tick du jeu
    // Recupere la texture appropriée selon la position du segment dans le corps
    QPixmap getMap(const std::list<Position>::const_iterator &,const std::list<Position> &);

  protected:
    void paintEvent(QPaintEvent *); // Affichage graphique
    void keyPressEvent(QKeyEvent *); // Controle des directions

  private slots:
    void clickAjouterMur(); // Slot lié au bouton d'ajout
    void clickSupprimerMur(); // Slot lié au bouton de suppression

};

#endif
