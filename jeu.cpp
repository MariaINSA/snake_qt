#include <iostream>
#include <cassert>
#include "jeu.hpp"

using namespace std;
Jeu::Jeu()
{
    terrain = nullptr;
    currFruit = nullptr;
    largeur = 0; hauteur = 0;
    dirSnake = DROITE;
}

Jeu::Jeu(const Jeu &jeu):snake(jeu.snake)
{
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;
    score = jeu.score;
    
    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;

    if (jeu.currFruit != nullptr) {
        Position p = jeu.currFruit->getPos();
        Fruit_type t = jeu.currFruit->getType();
        
        if (t == BANANA) currFruit = new Banana(p, t);
        else if (t == FRAISE) currFruit = new Fraise(p, t);
        else currFruit = new Fruit(p, t);
    } else {
        currFruit = nullptr;
    }
}

Jeu::~Jeu()
{
    if (terrain!=nullptr)
        delete[] terrain;
    if (currFruit!=nullptr)
        delete[] currFruit;
}

Jeu &Jeu::operator=(const Jeu &jeu)
{
    if (terrain!=nullptr)
        delete[] terrain;

    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;
    snake = jeu.snake;

    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
    return *this;
}

bool Jeu::init()
{
	int x, y;
	// list<Position>::iterator itSnake;

	const char terrain_defaut[15][21] = {
		"####..##############",
		"#........##........#",
		"#.#####..##...####.#",
		"#........##........#",
		"#..................#",
		"#..................#",
		"....................",
		"....................",
		"....................",
		"....................",
		"#..................#",
		"#..................#",
		"#.....#......#.....#",
		"#.....#......#.....#",
        "####..##############"
    };

	largeur = 20;
	hauteur = 15;
    score=0;
	terrain = new Case[largeur*hauteur];

	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
                terrain[y*largeur+x] = MUR;
            else
                terrain[y*largeur+x] = VIDE;

    int longueurSerpent = 5;
    snake.clear();

    Position posTete;
    posTete.x = 15;
    posTete.y = 8; 
	for (int i=0; i<longueurSerpent; i++)
    {
        snake.push_back(posTete);
        posTete.x--;
    }

    this->new_fruit();

    return true;
}

void Jeu::evolue()
{
    Position posTest;
	list<Position>::iterator itSnake;
    
    //typedef enum {GAUCHE, HAUT,DROITE, BAS} Direction;


    int depX[] = {-1, 0, 1, 0};
    int depY[] = {0, -1, 0, 1};

    posTest.x = snake.front().x + depX[dirSnake];
    posTest.y = snake.front().y + depY[dirSnake];

    if (posValide(posTest))
    {
        if(posTest!=currFruit->getPos()){
            snake.pop_back();
        }else{ //procedure for eaten fruit (varies for fruit)
            currFruit->onEaten(*this);
            this->new_fruit();
        }
        snake.push_front(posTest);
    }
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

Case Jeu::getCase(const Position &pos) const
{
    assert(pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur);
    return terrain[pos.y*largeur+pos.x];
}

const list<Position> &Jeu::getSnake() const
{
    return snake;
}

bool Jeu::posValide(const Position &pos) const
{
    if (pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur
        && terrain[pos.y*largeur+pos.x]==VIDE)
    {
        list<Position>::const_iterator itSnake;
        itSnake = snake.begin();
        while (itSnake!=snake.end() && *itSnake!=pos)
            itSnake++;
        return (itSnake==snake.end());
    }
    else
        return false;
}

void Jeu::setDirection(Direction dir)
{
    dirSnake = dir;
}

void Jeu::upScore()
{
    score++;
    //snake goes longer
    cout<<"New score is "<<score<<endl;

}

void Jeu::new_fruit() {
    //nettoyer la memoire

    if (currFruit != nullptr) {
        delete currFruit;
    }


    // coordonnees aleatoires
    Position posTest;
    do{
        posTest.x = rand() % largeur;
        posTest.y = rand() % hauteur; 
    } while (!posValide(posTest));

    
    // 3. Pick a random type (0 to 2)
    int type = rand() % 3;

    switch(type) {
        case POMME:
            currFruit = new Fruit(posTest,POMME); break;
        case BANANA:
            currFruit = new Banana(posTest, BANANA); break;
        case FRAISE:
            currFruit = new Fraise(posTest, FRAISE); break;
    }

}

const Fruit* Jeu::getFruit() const{
    return currFruit;
}

Direction Jeu::getDirection() const
{
    return dirSnake;
}