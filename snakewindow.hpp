#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

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


class SnakeWindow : public QFrame
{
  protected:
    static const int HAUTEUR_BOUTONS = 40;
    static const QStringList paths; // Declaration only
    QPixmap* pixmapSnake;
    QPixmap* pixmapFruits; 
    QPixmap pixmapMur;

    SnakeButton *btnAjouterMur;
    SnakeButton *btnSupprimerMur;

    Jeu jeu;
    

  public:
    SnakeWindow(QWidget *pParent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~SnakeWindow();
    void handleTimer();
    QPixmap getMap(const std::list<Position>::const_iterator &,const std::list<Position> &);

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

  private slots:
    void clickAjouterMur();
    void clickSupprimerMur();

};

#endif
