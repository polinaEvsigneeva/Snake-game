#include <iostream>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>


using namespace std;

#define mapWidth 80 
#define mapHeight 40 
#define length 1000
#define fruitsCount 4

class Snake;
struct Map {
	char board[mapHeight][mapWidth];
	POINT fruits[fruitsCount];// dla wspólrzędnych owoców
	void clear();
	void generateFruit();
	bool isFreeCell(int x, int y);
	bool cellInMap(int x, int y);
	void show(Snake& two);
	void setColor(int color);
	void drawScore(Snake& one, Snake& two, int OutTextPosX, int OutTextPosY);
	void drawScoreForOnePlayer(Snake& one, int OutTextPosX, int OutTextPosY);
};

class Snake {
protected:
	enum direction { dirUp, dirRight, dirLeft, dirDown };
	enum result { live, kill };
	int x, y;// pozycja glowy węża
	Map* mapa;
	direction dir;
	POINT tail[length];
	int len;// dla aktualnego rozmiaru ogonu
	int lives;
	int eatenFruites;
	char nameOnBoard[10];// imie dla gracza 
	int color;
public:
	Snake();
	void addTail(int _x, int _y);
	void moveTail(int _x, int _y);
	Snake(Map* _mapa, char name[], int color);
	void init(int _x, int _y, direction _dir);
	void putOnMap();
	result userControl(char w, char s, char a, char d);
	void addTailCount(int _x, int _y, int count);
	int liczScore();
	void liczFruites();
	result move();
	result copmuterControl();
	bool winner();
	bool compareScore(Snake& one, Snake& two);
	int getX() { return x; }
	int getY() { return y; }
	int getCl() { return color; }
	char* getName() { return nameOnBoard; }
	int getLives() { return lives; }
	void messageBeforeNewGame(Snake& one);
	void CheckName(char name[]);
	int getFruites() { return eatenFruites; };
};

class Game :public Snake {
protected:
	int userChoiceHowToPlay;
	Map start;
	Map* pom = NULL;
	Snake* waz;
	Snake* wazSecond;
public:
	Game();
	void rules();
	void menu();
	void starting();
	void continueGame();
	void HowMuchEatenFruites();
};

