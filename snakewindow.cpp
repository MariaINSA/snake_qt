#include <iostream>
#include "snakewindow.hpp"

using namespace std;
const QStringList SnakeWindow::paths={
    "./data/snake_head.png",
    "./data/snake_mid.png",
    "./data/snake_tail.png",
    "./data/snake_coin.png",
    "./data/snake_coin_derniere.png",
    "./data/snake_avant.png",
    "./data/apple_snake.png",
    "./data/banana_snake.png",
    "./data/strawberry_snake.png",
};

SnakeWindow::SnakeWindow(QWidget *pParent, Qt::WindowFlags flags):QFrame(pParent, flags)
{
    cout<<"cp0"<<endl;
    // Taille des cases en pixels
    int largeurCase, hauteurCase;
    pixmapSnake = new QPixmap[6]; // Manual allocation

    for (int i = 0; i < 6; i++) {
        // load() returns true on success
        if (!pixmapSnake[i].load(paths[i])) {
            cout<<"Impossible d'ouvrir "<<paths[i].toStdString()<<endl;
            exit(-1);
        }
    }

    pixmapFruits = new QPixmap[3];
    for (int i =0;i<3;i++){
        if (!pixmapFruits[i].load(paths[i+6])) {
            cout<<"Impossible d'ouvrir "<<paths[i+6].toStdString()<<endl;
            exit(-1);
        }
    }

    if (pixmapMur.load("./data/mur.bmp")==false)
    {
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }
    jeu.init();

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase);

    QTimer *timer =new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleTimer);
    timer->start(100);

}

QPixmap SnakeWindow::getMap(const list<Position>::const_iterator &pos_it,const list<Position> &snake){
    //rotation de tete de snake depends de la position de la prochaine piece
    // rotation de corps et choix de coin (et sa rotation) depends de la piece d'avant et apres
    // rotation du tail est avec la piece d'avant
    int rotation;
    Position current_pos=*pos_it;
    Position past_pos,next_pos;
    Direction after, before;
    //check if head
    if(snake.front()==current_pos){
        next_pos=*(next(pos_it));
        after=current_pos.quadrant(next_pos);
        rotation=((after+2)%4)*90;
        return pixmapSnake[HEAD].transformed(QTransform().rotate(rotation));
    }
    //check if tail
    else if(current_pos==snake.back()){
        past_pos=*(prev(pos_it));
        before=current_pos.quadrant(past_pos);
        rotation=((before)%4)*90;
        return pixmapSnake[TAIL].transformed(QTransform().rotate(rotation));
    } //else, it is body
    else{
        next_pos=*(next(pos_it));
        past_pos=*(prev(pos_it));
        before=current_pos.quadrant(past_pos);
        after=current_pos.quadrant(next_pos);
        //check if second to last could be flat or coin :/ (special case)
        if(snake.back()==next_pos){
            //is it flat
            if (before==(after+2)%4){
                //we adjust this according to the after (the tail)
                //typedef enum {GAUCHE, HAUT,DROITE, BAS} Direction;
                rotation=((after+2)%4)*90;
                return pixmapSnake[BEFORE_TAIL].transformed(QTransform().rotate(rotation));
            }else{//kill me now
                //to flip scale(-1, 1) to not (1,1) 
                int flip;
                if ((before == GAUCHE && after == HAUT)) {
                    rotation = 180; flip=-1;
                }else if(before == HAUT && after == GAUCHE){
                    rotation=90; flip=1;
                }else if (before == HAUT && after == DROITE) {
                    rotation = 270; flip=-1;
                } else if(before == DROITE && after == HAUT){
                    rotation = 180; flip=1;
                }else if ((before == DROITE && after == BAS)) {
                    rotation = 0; flip=-1;
                } else if(before == BAS && after == DROITE){
                    rotation = 270; flip=1;
                } else if ((before == BAS && after == GAUCHE)) {
                    rotation = 90; flip=-1;
                } else{
                    rotation = 0;flip=1;
                }
                return pixmapSnake[CORNER_TAIL].transformed(QTransform().rotate(rotation).scale(flip, 1));
            }
        }
        else{
            //check if it is flat (same dir before after) -> before and after are oposed
            if (before==(after+2)%4){
                if (before % 2) // vertical
                    return pixmapSnake[BODY].transformed(QTransform().rotate(90));
                else
                    return pixmapSnake[BODY];
            // si non, coin
            }else{
                if ((before == GAUCHE && after == HAUT) || (before == HAUT && after == GAUCHE)) {
                    rotation = 90; 
                } else if ((before == HAUT && after == DROITE) || (before == DROITE && after == HAUT)) {
                    rotation = 180;
                } else if ((before == DROITE && after == BAS) || (before == BAS && after == DROITE)) {
                    rotation = 270;
                } else if ((before == BAS && after == GAUCHE) || (before == GAUCHE && after == BAS)) {
                    rotation = 0;
                }
                return pixmapSnake[CORNER].transformed(QTransform().rotate(rotation));
            }
        }
    }
    //just in case
    return pixmapSnake[BODY];
}

void SnakeWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    
    Position pos;
    
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (pos.y=0;pos.y<jeu.getNbCasesY();pos.y++)
        for (pos.x=0;pos.x<jeu.getNbCasesX();pos.x++)
			if (jeu.getCase(pos)==MUR)
                painter.drawPixmap(pos.x*largeurCase, pos.y*hauteurCase, pixmapMur);

    //Dessine la fruite
    const Fruit* fruit= jeu.getFruit();
    pos=fruit->getPos(); 
    painter.drawPixmap(pos.x*largeurCase, pos.y*hauteurCase, pixmapFruits[fruit->getType()]);

    // Dessine le serpent
    const list<Position> &snake = jeu.getSnake();
    if (!snake.empty())
    {
        list<Position>::const_iterator itSnake;
        for (itSnake=snake.begin(); itSnake!=snake.end(); itSnake++){
            QPixmap curr=this->getMap(itSnake,snake);
            painter.drawPixmap(itSnake->x*largeurCase, itSnake->y*hauteurCase, curr);
        }
    }
}

void SnakeWindow::keyPressEvent(QKeyEvent *event)
{
    Direction key_dir;
    if (event->key()==Qt::Key_Left)
        key_dir=GAUCHE;
    else if (event->key()==Qt::Key_Right)
        key_dir=DROITE;
    else if (event->key()==Qt::Key_Up)
        key_dir=HAUT;
    else if (event->key()==Qt::Key_Down)
        key_dir=BAS;

    //just to check if it is not the contrary direction
    if (jeu.getDirection() != (key_dir+2)%4)
        jeu.setDirection(key_dir);
    update();
}

void SnakeWindow::handleTimer()
{
    jeu.evolue();
    update();
}

SnakeWindow::~SnakeWindow() {
    delete[] pixmapSnake;
    delete[] pixmapFruits;
}