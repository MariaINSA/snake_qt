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

    if (pixmapMur.load("./data/mur.bmp")==false){
        cout<<"Impossible d'ouvrir mur.bmp"<<endl;
        exit(-1);
    }

    jeu.init();
    btnAjouterMur = new SnakeButton("Ajout mur", this);
    btnAjouterMur->setGeometry(5, 5, 100, 30);

    btnSupprimerMur = new SnakeButton("Suppr mur", this);
    btnSupprimerMur->setGeometry(115, 5, 100, 30);

    connect(btnAjouterMur,   &QPushButton::clicked, this, &SnakeWindow::clickAjouterMur);
    connect(btnSupprimerMur, &QPushButton::clicked, this, &SnakeWindow::clickSupprimerMur);

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    resize(jeu.getNbCasesX()*largeurCase, jeu.getNbCasesY()*hauteurCase + HAUTEUR_BOUTONS);

    QTimer *timer =new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleTimer);
    timer->start(100);
}

Direction getWrappedDirection(Position current, Position neighbor) {
    // Let's assume your grid size is 'width' and 'height'
    int dx = neighbor.x - current.x;
    int dy = neighbor.y - current.y;

    // Horizontal Wrap Check
    if (abs(dx) > 1) {
        // If dx is very positive, neighbor is on the right, 
        // but since we wrapped, current is actually to the RIGHT of neighbor.
        return (dx > 0) ? GAUCHE : DROITE;
    }
    // Vertical Wrap Check
    if (abs(dy) > 1) {
        return (dy > 0) ? HAUT : BAS;
    }

    // Normal case (no wrap)
    return current.quadrant(neighbor);
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
        //after=current_pos.quadrant(next_pos);
        after=getWrappedDirection(current_pos, next_pos);

        rotation=((after+2)%4)*90;
        return pixmapSnake[HEAD].transformed(QTransform().rotate(rotation));
    }
    //check if tail
    else if(current_pos==snake.back()){
        past_pos=*(prev(pos_it));
        //before=current_pos.quadrant(past_pos);
        before=getWrappedDirection(current_pos, past_pos);

        rotation=((before)%4)*90;
        return pixmapSnake[TAIL].transformed(QTransform().rotate(rotation));
    } //else, it is body
    else{
        next_pos=*(next(pos_it));
        past_pos=*(prev(pos_it));
        //before=current_pos.quadrant(past_pos);
        before=getWrappedDirection(current_pos, past_pos);
        //after=current_pos.quadrant(next_pos);
        after=getWrappedDirection(current_pos, next_pos);

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

// met une couleur sur un pixmap sans toucher la transparence
static QPixmap coloriser(const QPixmap &src, QColor c)
{
    QPixmap res = src;
    QPainter p(&res);
    p.setCompositionMode(QPainter::CompositionMode_SourceAtop);
    p.fillRect(res.rect(), c);
    p.end();
    return res;
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
                painter.drawPixmap(pos.x*largeurCase, pos.y*hauteurCase+ HAUTEUR_BOUTONS, pixmapMur);

    //Dessine la fruite
    const Fruit* fruit= jeu.getFruit();
    pos=fruit->getPos(); 
    painter.drawPixmap(pos.x*largeurCase, pos.y*hauteurCase+ HAUTEUR_BOUTONS, pixmapFruits[fruit->getType()]);

    // Dessine le serpent
    // serpent 1 : gris si mort, normal sinon
    const list<Position> &snake = jeu.getSnake();
    if (!snake.empty()){
        list<Position>::const_iterator itSnake;
        for (itSnake=snake.begin(); itSnake!=snake.end(); itSnake++){
            QPixmap curr=this->getMap(itSnake,snake);
            // serpent 1 : gris si mort, normal sinon
            curr= jeu.getMort1() ? coloriser(curr,  QColor(150, 150, 150, 180)) : curr;
            painter.drawPixmap(itSnake->x*largeurCase, itSnake->y*hauteurCase + HAUTEUR_BOUTONS, curr);
        }
    }

    const list<Position> &snake2 = jeu.getSnake2();
    if (!snake2.empty()){
        list<Position>::const_iterator itSnake;
        for (itSnake=snake2.begin(); itSnake!=snake2.end(); itSnake++){
            QPixmap curr=this->getMap(itSnake,snake2);
            // serpent 1 : gris si mort, normal sinon
            curr= jeu.getMort2() ? coloriser(curr,  QColor(150, 150, 150, 180)) : curr;
            painter.drawPixmap(itSnake->x*largeurCase, itSnake->y*hauteurCase + HAUTEUR_BOUTONS, curr);
        }
    }

    // message si un joueur a perdu
    painter.setFont(QFont("Arial", 14, QFont::Bold));

    if (jeu.getMort1())
    {
        painter.setPen(QColor(200, 50, 50));
        painter.drawText(10, jeu.getNbCasesY() * hauteurCase / 2 + HAUTEUR_BOUTONS, "Joueur 1 perdu !");
    }
    if (jeu.getMort2())
    {
        painter.setPen(QColor(50, 50, 200));
        painter.drawText(jeu.getNbCasesX() * largeurCase - 200,
                         jeu.getNbCasesY() * hauteurCase / 2 + 30 + HAUTEUR_BOUTONS,
                         "Joueur 2 perdu !");
    }

    // Set up the font and color
    painter.setPen(Qt::black);

    // Draw Score Player 2
    QString score2 = QString("J2: %1").arg(jeu.getScore2());
    // Calculate width to align to the right side
    int score2Width = painter.fontMetrics().horizontalAdvance(score2);
    painter.drawText(this->width() - score2Width - 20, 30, score2);

    // Draw Score Player 1
    QString score1 = QString("J1: %1").arg(jeu.getScore1());
    int score1Width = painter.fontMetrics().horizontalAdvance(score1);
    painter.drawText(this->width() -score1Width - score2Width - 25, 30, score1);
}

void SnakeWindow::keyPressEvent(QKeyEvent *event)
{
    Direction key_dir;
    int joueur=0; //0 for none, 1for 1 et 2 for 2

    // joueur 1 : fleches
    if (!jeu.getMort1())
    {
        if (event->key() == Qt::Key_Left){
            key_dir=GAUCHE; joueur=1;
        } else if (event->key() == Qt::Key_Right){
            key_dir=DROITE; joueur=1;
        } else if (event->key() == Qt::Key_Up){
            key_dir=HAUT; joueur=1;
        } else if (event->key() == Qt::Key_Down){
            key_dir=BAS; joueur=1;
        }
    }

    // joueur 2 : zqsd
    if (!jeu.getMort2())
    {
        if (event->key() == Qt::Key_Q){
            key_dir=GAUCHE; joueur=2;
        } else if (event->key() == Qt::Key_D){
            key_dir=DROITE; joueur=2;
        } else if (event->key() == Qt::Key_Z){
            key_dir=HAUT; joueur=2;
        } else if (event->key() == Qt::Key_S){
            key_dir=BAS; joueur=2;
        }
    }

    //just to check if it is not the contrary direction
    if (joueur==1){
        if (jeu.getDirection() != (key_dir+2)%4)
            jeu.setDirection(key_dir);
    }else if(joueur==2){
        if (jeu.getDirection2() != (key_dir+2)%4)
            jeu.setDirection2(key_dir);
    }
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

void SnakeWindow::clickAjouterMur()   { jeu.ajouterMur();   update(); }
void SnakeWindow::clickSupprimerMur() { jeu.supprimerMur(); update(); }