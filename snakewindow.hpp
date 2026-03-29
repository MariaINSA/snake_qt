#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtCore/QtCore>
#include <QtWidgets/QtWidgets>
#include "jeu.hpp"

typedef enum { HEAD, BODY, TAIL, CORNER, CORNER_TAIL, BEFORE_TAIL} snakeParts;

class SnakeWindow : public QFrame
{
  protected:
    static const QStringList paths; // Declaration only
    QPixmap* pixmapSnake;
    QPixmap pixmapMur;
    QPixmap* pixmapFruits; 
    Jeu jeu;

  public:
    SnakeWindow(QWidget *pParent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    ~SnakeWindow();
    void handleTimer();
    QPixmap getMap(const std::list<Position>::const_iterator &,const std::list<Position> &);

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
};

#endif
