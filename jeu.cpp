#include <iostream>
#include <cassert>
#include <vector>
#include "jeu.hpp"

using namespace std;
Jeu::Jeu()
{
    terrain = nullptr;
    currFruit = nullptr;
    largeur = 0; hauteur = 0;
    dirSnake = DROITE;
    dirSnake2 = GAUCHE;
    mort1 = false;
    mort2 = false;
    mangeur=0;
    score=0; score2=0;
    drillActive1 = false; drillActive2=false;
    drillTimer1=0; drillTimer2=0;

}

Jeu::Jeu(const Jeu &jeu) : snake(jeu.snake), snake2(jeu.snake2)
{
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;
    dirSnake2 = jeu.dirSnake2;
    score = jeu.score;
    mort1     = jeu.mort1;
    mort2     = jeu.mort2;
    mangeur   = jeu.mangeur;
    score2    = jeu.score2;
    drillActive1 = jeu.drillActive1; 
    drillActive2= jeu.drillActive2;
    drillTimer1=jeu.drillTimer1; drillTimer2=jeu.drillTimer1;
    
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
    dirSnake2 = jeu.dirSnake2;
    mort1     = jeu.mort1;
    mort2     = jeu.mort2;
    snake     = jeu.snake;
    snake2    = jeu.snake2;
    score     = jeu.score;
    score2    = jeu.score2;
    mangeur   =jeu.mangeur;
    drillActive1 = jeu.drillActive1; 
    drillActive2= jeu.drillActive2;
    drillTimer1=jeu.drillTimer1; drillTimer2=jeu.drillTimer1;    

    if (jeu.terrain!=nullptr)
    {
        terrain = new Case[largeur*hauteur];
        for (int c=0; c<largeur*hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;

    if (jeu.currFruit!=nullptr){
        this->new_fruit();
    }
    else
        currFruit = nullptr;
    
    return *this;
}

bool Jeu::init(){

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
    score=0; score2=0;
    drillActive1 =false;  drillActive2= false;
    drillTimer1=0; drillTimer2=0;
    
	terrain = new Case[largeur*hauteur];

    int x, y;
	for(y=0;y<hauteur;++y)
		for(x=0;x<largeur;++x)
            if (terrain_defaut[y][x]=='#')
                terrain[y*largeur+x] = MUR;
            else
                terrain[y*largeur+x] = VIDE;

    // serpent 1 : part de la droite
    snake.clear();
    Position t1(15, 7);
    for (int i = 0; i < 5; i++){
        snake.push_back(t1);
        t1.x--;
    }

    // serpent 2 : part de la gauche
    snake2.clear();
    Position t2(4, 9);
    for (int i = 0; i < 5; i++){
        snake2.push_back(t2);
        t2.x++;
    }

    dirSnake  = DROITE;
    dirSnake2 = GAUCHE;
    mort1 = false;
    mort2 = false;

    this->new_fruit();

    return true;
}

void Jeu::evolue(){    

    //typedef enum {GAUCHE, HAUT,DROITE, BAS} Direction;
    int depX[] = {-1, 0, 1, 0};
    int depY[] = {0, -1, 0, 1};

    //drill countdown 
    if (drillTimer1 > 0) {
        drillTimer1--;
    if (drillTimer1 == 0) drillActive1 = false;
    }else if (drillTimer2 > 0) {
        drillTimer2--;
    if (drillTimer2 == 0) drillActive2 = false;
    }

    // calcule la prochaine case de chaque serpent
    Position pos1 = snake.front();
    Position pos2 = snake2.front();
    if (!mort1){
        pos1.x += depX[dirSnake];
        pos1.y += depY[dirSnake];

        // traverse les bords
        if (pos1.x < 0)        pos1.x = largeur - 1;
        if (pos1.x >= largeur) pos1.x = 0;
        if (pos1.y < 0)        pos1.y = hauteur - 1;
        if (pos1.y >= hauteur) pos1.y = 0;
    }

    if (!mort2){
        pos2.x += depX[dirSnake2];
        pos2.y += depY[dirSnake2];

        if (pos2.x < 0)        pos2.x = largeur - 1;
        if (pos2.x >= largeur) pos2.x = 0;
        if (pos2.y < 0)        pos2.y = hauteur - 1;
        if (pos2.y >= hauteur) pos2.y = 0;
    }

    // collision tete contre tete : les deux perdent
    if (!mort1 && !mort2 && pos1 == pos2){
        mort1 = true;
        mort2 = true;
    } else
    {
        // vérifie si joueur 1 touche un mur ou un serpent
        if (!mort1)
        {
            if (terrain[pos1.y * largeur + pos1.x] == MUR)
            {
                if (drillActive1) {
                    // on mange le mur
                    terrain[pos1.y * largeur + pos1.x] = VIDE; 
                } else {
                    mort1 = true;
                }
            }
            else
            {
                for (const Position &s : snake2)
                    if (s == pos1) { mort1 = true; break; }
                
                if (!mort1)
                {
                    auto fin = snake.end();
                    --fin;
                    for (auto it = snake.begin(); it != fin; ++it)
                        if (*it == pos1) { mort1 = true; break; }
                }
            }
        }

        // vérifie si joueur 2 touche un mur ou un serpent
        if (!mort2)
        {
            if (terrain[pos2.y * largeur + pos2.x] == MUR)
            {
                if (drillActive2) {
                    // on mange le mur
                    terrain[pos2.y * largeur + pos2.x] = VIDE; 
                } else {
                    mort2 = true;
                }
            }
            else
            {
                for (const Position &s : snake)
                    if (s == pos2) { mort2 = true; break; }

                if (!mort2)
                {
                    auto fin = snake2.end();
                    --fin;
                    for (auto it = snake2.begin(); it != fin; ++it)
                        if (*it == pos2) { mort2 = true; break; }
                }
            }
        }
    }

    // avance les serpents encore vivants
    if (!mort1){
        snake.push_front(pos1);
        if(pos1!=currFruit->getPos()){
            snake.pop_back();
        }else{ //procedure for eaten fruit (varies for fruit)
            mangeur=0; //queue
            currFruit->onEaten(*this);
            this->new_fruit();
        }
    }

    if (!mort2){
        snake2.push_front(pos2);
        if(pos2!=currFruit->getPos()){
            snake2.pop_back();
        }else{ //procedure for eaten fruit (varies for fruit)
            mangeur=1;
            currFruit->onEaten(*this);
            this->new_fruit();
        }
    }

    // si les deux sont morts on relance
    if (mort1 && mort2)
        init();
}

int Jeu::getNbCasesX() const { return largeur;}

int Jeu::getNbCasesY() const { return hauteur;}

bool Jeu::getMort1()    const { return mort1;    }
bool Jeu::getMort2()    const { return mort2;    }

int Jeu::getScore1()    const { return score;    }
int Jeu::getScore2()    const { return score2;   }

Case Jeu::getCase(const Position &pos) const
{
    assert(pos.x>=0 && pos.x<largeur && pos.y>=0 && pos.y<hauteur);
    return terrain[pos.y*largeur+pos.x];
}

const list<Position> &Jeu::getSnake() const { return snake;}
const list<Position> &Jeu::getSnake2() const { return snake2;}

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

        itSnake = snake2.begin();
        while (itSnake!=snake2.end() && *itSnake!=pos)
            itSnake++;
        return (itSnake==snake2.end());
    }
    else
        return false;
}

void Jeu::setDirection(Direction dir){ dirSnake = dir;}
void Jeu::setDirection2(Direction dir){ dirSnake2 = dir;}

void Jeu::ajouterMur()
{
    vector<int> cases_libres;

    for (int i = 0; i < largeur * hauteur; i++)
    {
        if (terrain[i] != VIDE) continue;

        Position p(i % largeur, i / largeur);
        bool prise = false;
        for (const Position &s : snake)  if (s == p) { prise = true; break; }
        for (const Position &s : snake2) if (s == p) { prise = true; break; }

        if (!prise)
            cases_libres.push_back(i);
    }

    if (!cases_libres.empty())
        terrain[cases_libres[rand() % cases_libres.size()]] = MUR;
}

void Jeu::supprimerMur()
{
    vector<int> murs;
    for (int i = 0; i < largeur * hauteur; i++)
        if (terrain[i] == MUR)
            murs.push_back(i);

    if (!murs.empty())
        terrain[murs[rand() % murs.size()]] = VIDE;
}

void Jeu::upScore(){
    if (!mangeur)
        score++;
    else
        score2++;
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

void Jeu::reverseSnake() {
    //on utilise la fonction
    if (!mangeur){
        snake.reverse();

        // Faut changer selon la position de la tail (ou dans ce cas, la tete)
        if (snake.size() >= 2) {
            list<Position>::const_iterator tete = snake.begin();
            list<Position>::const_iterator neck = std::next(tete);

            if (tete->x < neck->x)      dirSnake = GAUCHE;
            else if (tete->x > neck->x) dirSnake = DROITE;
            else if (tete->y < neck->y) dirSnake = HAUT;
            else if (tete->y > neck->y) dirSnake = BAS;
        }
    }else{
        snake2.reverse();

        // Faut changer selon la position de la tail (ou dans ce cas, la tete)
        if (snake2.size() >= 2) {
            list<Position>::const_iterator tete = snake2.begin();
            list<Position>::const_iterator neck = std::next(tete);

            if (tete->x < neck->x)      dirSnake2 = GAUCHE;
            else if (tete->x > neck->x) dirSnake2 = DROITE;
            else if (tete->y < neck->y) dirSnake2 = HAUT;
            else if (tete->y > neck->y) dirSnake2 = BAS;
        }
    }
}

const Fruit* Jeu::getFruit() const{
    return currFruit;
}

Direction Jeu::getDirection() const
{
    return dirSnake;
}

Direction Jeu::getDirection2() const{
    return dirSnake2;
}

bool Jeu::getDrill1() const{return drillActive1;}
bool Jeu::getDrill2() const{return drillActive2;}

void Jeu::setDrillActive(){
    if (mangeur == 0) {
        drillActive1 = true;
        drillTimer1 = 30; // Lasts for 50 frames
    } else if (mangeur==1) {
        drillActive2 = true;
        drillTimer2 = 30;
    }
}
