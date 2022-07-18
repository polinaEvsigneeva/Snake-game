#include "classesSnake.h"



void Move(int x, int y) {// przesuwa kursor na podana pozycje
	COORD pos = {NULL};
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Map::setColor(int color) {// ustawia color
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(console, color);
}


	void Map::clear() {
		//czyszczenie mapy
		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				board[i][j] = ' ';
			}
		}
		//wypelnienie bramki
		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				board[i][0] = '|';
				board[i][mapWidth - 1] = '|';
				board[0][j] = '-';
				board[mapHeight - 1][j] = '-';
			}
		}
		// ustawianie owocow na mapie
		for (int i = 0; i < fruitsCount; i++) {
			board[fruits[i].x][fruits[i].y] = '*';
		}
	}

	void Map::generateFruit() {// funkcja generuje wspolrzedne dla owocow
		int fruitX, fruitY;
		srand((unsigned)time(NULL));
		for (int i = 0; i < fruitsCount; i++) {
			fruitX = rand() % (30 - 1 + 1) + 1;
			fruitY = rand() % (70 - 1 + 1) + 1;
			fruits[i].x = fruitX;
			fruits[i].y = fruitY;
		}
	}



	void Map::show(Snake& two) {// funkcja wypisuje mape
		Move(0, 0);
		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				if (i == two.getY() && j == two.getX()) {// ustawia color dla glowy węża
					setColor(two.getCl());
					cout << board[i][j];
					setColor(7);
				}
				else {
					cout << board[i][j];
				}
			}
			cout << endl;
		}
	}

	bool Map::isFreeCell(int x, int y) {// sprawdza czy komorka planszy jest wolna
		if (!cellInMap(x, y)) return false;
		return (board[y][x] != '@') && (board[y][x] != '+');
	}
	bool Map::cellInMap(int x, int y) {// czy ta komorka znajduje sie w przedzialach mapy
		return (x >= 1) && (x < mapWidth - 1) && (y >= 1) && (y < mapHeight - 1);
	}


	//metoda ustawia licznik prób na mapie dla dwoch graczy
	void Map::drawScore(Snake& one, Snake& two, int outTextPosX, int outTextPosY) {
		isFreeCell(outTextPosX, outTextPosY);
		char str[50] = {NULL};
		sprintf_s(str, "%s: %d | %s: %d",  one.getName(), one.getLives(), two.getName(), two.getLives());
		for (int i = 0; i < strlen(str); i++) {
			board[outTextPosY][outTextPosX + i] = str[i];
		} 
	}

	//metoda ustawia licznik prób na mapie dla jednego gracza
	void Map::drawScoreForOnePlayer(Snake& one, int outTextPosX, int outTextPosY) {
		isFreeCell(outTextPosX, outTextPosY);
		char str[50] = { NULL };
		sprintf_s(str, "%s: %d", one.getName(), one.getLives());
		for (int i = 0; i < strlen(str); i++) {
			board[outTextPosY][outTextPosX + i] = str[i];
		}
	}

	Snake::Snake() {
		x = 5;
		y = 10;
		mapa = NULL;
		dir = dirRight;
		len = 0;
		lives = 0;
		eatenFruites = 0;
		for (int i = 0; i < 10; i++) {
			nameOnBoard[i] = ' ';
		}
		color = 0;
	}
	void Snake::addTail(int _x, int _y) {// dodaje jeden element do ogonu weza
		moveTail(_x, _y);
		len++;
		if (len >= length) {
			len = length - 1;
		}
	}
	void Snake::moveTail(int _x, int _y) {// przesuwa ogon za glową
		for (int i = len - 1; i >= 0; i--) {
			tail[i + 1] = tail[i];
		}
			tail[0].x = _x;
			tail[0].y = _y;
	}

	Snake::Snake(Map* _mapa, char name[], int _color) {
		mapa = _mapa;
		init(0, 0, dirRight);
		lives = 0;
		eatenFruites = 0;
		for (int i = 0; i < strlen(name); i++) {
			nameOnBoard[i] = name[i];
		}
		color = _color;
	}
	void Snake::init(int _x, int _y, direction _dir) {// metoda zaczyna grę od nowa
		x = _x;
		y = _y;
		dir = _dir;
		len = 0;
		mapa->generateFruit();
	}
	void Snake::putOnMap() {// ustawia na mapie glowę i ogon
		 mapa->board[y][x] = '@';
		for (int i = 0; i < len - 1; i++) {
			mapa->board[tail[i].y][tail[i].x] = '+';
		}
		
	}
	
	Snake::result Snake::userControl(char w, char s, char a, char d) {
		
		if (_kbhit()) {
			switch (_getch()) {
			case 'w':
				dir = dirUp;
				break;
			case 'a':
				dir = dirLeft;
				break;
			case 's':
				dir = dirDown;
				break;
			case 'd':
				dir = dirRight;
				break;
			case 77:
				dir = dirRight;
				break;
			case 75:
				dir = dirLeft;
				break;
			case 72:
				dir = dirUp;
				break;
			case 80:
				dir = dirDown;
				break;
			}
		}
		return move();
	}

	void Snake::addTailCount(int _x, int _y, int count) {// dodaje 5 elementow do ogonu
		for (int i = 0; i < count; i++) {
			addTail(_x, _y);
		}
	}
	int Snake::liczScore() {
		return ++lives;
	}

	void Snake::liczFruites() {
		 ++eatenFruites;
	}
	
	Snake::result Snake::move() {
		POINT old = {NULL};// pozycja glowy przed ruchem
		old.x = x;
		old.y = y;

		if (dir == dirUp) y--;
		if (dir == dirDown) y++;
		if (dir == dirLeft) x--;
		if (dir == dirRight) x++;

		if (!mapa->isFreeCell(x, y)) {
			return kill;
		}

		if (mapa->board[y][x] == '*') {// dodajemy do ogonu 5 elementow, jesli waz zjadl owoc
			addTailCount(old.x, old.y, 5);
			liczFruites();
		}
		else {
			moveTail(old.x, old.y);
		}
		return live;
	}
	Snake::result Snake::copmuterControl() {
		// zbiera jedzenie
		// poruszajac sie w linii pionowej przegladamy cala kolumne i jesli waz znajduje jedzenie, to zmienia kierunek
		if ((dir == dirDown) || (dir == dirUp)) {
			int i;
			for (i = 0; i < mapWidth; i++) {
				if (mapa->board[y][i] == '*') {
					break;
				}
			}
			
			if (i < mapWidth) 
				dir = (i < x) ? dirLeft : dirRight;
				
		}
		// tak samo w linii poziomowej
		if ((dir == dirRight) || (dir == dirLeft)) {
			int i;
			for (i = 0; i < mapHeight; i++) {
				if (mapa->board[i][x] == '*')
					break;
			}
			
			if (i < mapHeight) 
				dir = (i < y) ? dirUp : dirDown;
			
		}

		POINT pos = {x, y};
		if (dir == dirDown) pos.y++;
		if (dir == dirLeft) pos.x--;
		if (dir == dirRight) pos.x++;
		if (dir == dirUp) pos.y--;

		// jesli waz nie moze przesujac sie do tej pozycji - zmieniamy kierunek
		if (!mapa->isFreeCell(pos.x, pos.y)) { 
			if ((dir == dirDown) || (dir == dirUp)) {// sprawdzamy jaki waz ma kierunek
				dir = mapa->isFreeCell(x - 1, y) ? dirLeft : dirRight;// sprawdzamy jaka komorka planszy jest wolna: lewa czy prawa
			}
			else if ((dir == dirLeft) || (dir == dirRight)) {
				dir = mapa->isFreeCell(x, y - 1) ? dirUp : dirDown;
			}
		}
		return move();
	}
	 
	bool Snake::winner() {
		bool win = 1;
		if (eatenFruites == 10) {
			return win;
		}
		return !win;
	}

	bool Snake::compareScore(Snake& one, Snake& two) {
		bool win = 1;
		if (one.eatenFruites > two.eatenFruites) {
			return win;
		}
		else if (one.eatenFruites < two.eatenFruites) {
			return !win;
		}
	}

	void Snake::messageBeforeNewGame(Snake& one) {
		mapa->setColor(12);
		Move(15, 15);
		cout << "Player " << one.nameOnBoard << " was killed" << endl;
		system("pause");
		mapa->setColor(7);

	}

	void Snake::CheckName(char name[]) {
		cin >> name;
		while (getchar() != '\n' || strlen(name) > 10) {
			cout << "name must be less than 10 letters.Fix it: \n" << endl;
			cin >> name;
		}
	}


	Game::Game() {
		pom = &start;
		userChoiceHowToPlay = 0;
		waz = NULL;
		wazSecond = NULL;

	}

	void Game::rules() {
		int choice;
		start.setColor(10);
		cout << "HELLO! THIS IS THE GAME SNAKE. WOULD YOU LIKE TO READ THE RULES ABOUT THIS GAME OR DO YOU WANT TO START NOW?" << endl;
		cout << "Author: Palina Yeusihneyeva" << endl;
		start.setColor(12);
		cout << "CHOOSE 1 - TO READ THE RULES" << endl;
		start.setColor(15);
		cout << "CHOOSE 2 - TO START THE GAME" << endl;
		cin >> choice;
		if (choice == 1) {
			cout << "In the game, the player controls a snake that moves around the framed board collecting food (or other items)," << endl;
			cout << "trying not to hit his head against the walls surrounding the game board, as well as against part of his own body." << endl;
			cout << "As the snake eats a piece of food, its tail gets longer and longer, making it difficult to play." << endl;
			cout << "The player controls the direction of movement of the snake with the arrow keys (up, down, left, right) and the 'w', 'a', 's', 'd' buttons" << endl;
			cout << "Each player has 3 lives, i.e. 3 tries each. The used trial counts when the snake dies, i.e. it hits a wall or a part of its own or another player's body" << endl;
			cout << "THE PLAYER WILL HAVE THREE POSSIBILITIES TO PLAY:\nWith himself\nwith the other player\nwith the computer" << endl;
			cout << "If you choose to play with the second player on one laptop: The first player will control the bond using the 'a', 'w', 's', 'd' buttons, and the second - left, down, up, right arrows" << endl;
			cout << "The rules for two players are the same, but you still need not to hit the other snake" << endl;
			cout << "In the case of a game with a second player or a bot,if any of the two players does not reach 10 points, the player who eats more pieces of food during these three attempts is the winner." << endl;
			cout << "In the case of choosing to play with himself, the player controls the snake with the 'a', 'w', 's', 'd' keys and the player wins when he eats 10 pieces of food within three tries" << endl;
		}
	}
	void Game::menu() {
		while (1) {
			rules();
			cout << "How do you want to play:"<< endl <<"1) One" << endl << "2) With friend" << endl << "3) With bot" << endl;
			cin >> userChoiceHowToPlay;
			system("cls");
			starting();
		}

	}

	void Game::starting() {
		if (userChoiceHowToPlay == 1) {
			char name[10] = { NULL };
			cout << "Write your name" << endl;
			waz->CheckName(name);
			waz = new Snake(&start, name, 10);
			waz->init(10, 5, dirRight);


			do {
				if (waz->userControl('W', 'S', 'A', 'D') == kill) {
					waz->messageBeforeNewGame(*waz);// wypisujemy ze waz zmarl
					waz->init(10, 5, dirRight);// zaczynamy grę od nowa
					int count = waz->liczScore();// liczymy probe
	
					if (count == 3) {
						start.setColor(14);
						cout << "You lose:(((( Do you want to start again?" << endl;
						cout << "Your number of eaten fruties: " << waz->getFruites() << endl;
						cout << "choose 1 - to start again//////// click 'esc' to finish the game" << endl;
						start.setColor(7);
						continueGame();
						break;
					}
				}


				else {
					if (waz->winner() == 1) {// jesli gracz zjadl 10 owocow w ciagu trzech prob
						start.setColor(14);
						cout << "You win!!! Do you want to start again?" << endl;
						cout << "choose 1 - to start again//////// click 'esc' to finish the game" << endl;
						start.setColor(7);
						continueGame();
						break;
					}
				}
				
				start.clear();
				waz->putOnMap();
				start.drawScoreForOnePlayer(*waz, 1, 1);// 1 i 1 - to pozycja dla licznika zyc na mapie
				start.show(*waz);
				Sleep(20);
			} while (GetKeyState(VK_ESCAPE) >= 0);
		}
		else {
			char name[10] = { NULL }, name2[10] = {NULL};
			cout << "Write name for first snake" << endl;
			waz->CheckName(name);
			cout << "Write name for second snake" << endl;
			wazSecond->CheckName(name2);

			waz = new Snake(&start, name, 7);
			wazSecond = new Snake(&start, name2, 10);
			waz->init(10, 5, dirRight);
			wazSecond->init(60, 5, dirLeft);
			do {
				int count = 0, countSecond = 0;
				bool isMainPlayerKilled = waz->userControl('W', 'S', 'A', 'D') == kill;
				bool isSecondPlayerKilled = 0;

				if (userChoiceHowToPlay ==  2) {
					isSecondPlayerKilled = wazSecond->userControl(72, 80, 75, 77) == kill;
				}

				if (userChoiceHowToPlay ==  3) {
					isSecondPlayerKilled = wazSecond->copmuterControl() == kill;
				}

				if (isMainPlayerKilled || isSecondPlayerKilled) {
					Move(15, 15);

					if (isMainPlayerKilled)
						waz->messageBeforeNewGame(*waz);

					if (isSecondPlayerKilled)
						wazSecond->messageBeforeNewGame(*wazSecond);

					waz->init(10, 5, dirRight);
					wazSecond->init(60, 5, dirLeft);


					if (isMainPlayerKilled) {
						count = waz->liczScore();
					}
					else {
						countSecond = wazSecond->liczScore();
					}

					if (count == 3 || countSecond == 3) {// jesli ktos z dwoch graczy wykorzystal swoje proby
						if(waz->compareScore(*waz, *wazSecond) == 1){// porownujemy ilosc zjedzonych owocow
							start.setColor(14);
							cout << "The second snake lose:((((" << endl << "the first snake win!!!!! Do you want to start again?" << endl;
							cout << "choose 1 - to start again//////// click 'esc' to finish the game" << endl;
							HowMuchEatenFruites();
							start.setColor(7);
							continueGame();
							break;
							delete(wazSecond);
						}


						else {
							start.setColor(14);
							cout << "The first snake lose:((((" << endl << "the second snake win!!!!! Do you want to start again?" << endl;
							cout << "choose 1 - to start again//////// click 'esc' to finish the game" << endl;
							HowMuchEatenFruites();
							start.setColor(7);
							continueGame();
							break;
							delete(wazSecond);
						}
					}
				}

				else {
					if (waz->winner() == 1) {
						cout << "The second snake lose:((((" << endl << "the first snake win!!!!! Do you want to start again?" << endl;
						cout << "choose 1 - to start again//////// click 'esc' to finish the game" << endl;
						HowMuchEatenFruites();
						continueGame();
						break;
						delete(wazSecond);
					}

					else if(wazSecond->winner() == 1) {
						cout << "The first snake lose:((((" << endl << "the second snake win!!!!! Do you want to start again?" << endl;
						cout << "choose 1 - to start again//////// click 'esc' to finish the game" << endl;
						HowMuchEatenFruites();
						continueGame();
						break;
						delete(wazSecond);
					}
				}
				start.clear();
				waz->putOnMap();
				wazSecond->putOnMap();
				start.drawScore(*waz, *wazSecond, 1, 1);
				start.show(*wazSecond);
				Sleep(20);
			} while (GetKeyState(VK_ESCAPE) >= 0);
		}
	}
	
	void Game::continueGame() {// uzytkownik wybiera czy chce zagrac jeszcze raz
		int choice;
		cin >> choice;
		if (choice == 1) {
			system("cls");
			menu();
			delete(waz);
		}
		else {
			delete(waz);
		}
	}

	void Game::HowMuchEatenFruites() {// wypisuje ilosc zjedzonych owocow dla dwoch graczy na koniec gry
		cout << "number of eaten fruties player " << waz->getName() << ":" << waz->getFruites() << endl;
		cout << "number of eaten fruties player " << wazSecond->getName() << ":" << wazSecond->getFruites() << endl;
	}





	int main() {
	Game obiect;
	obiect.menu();
	return 0;
}