#include <iostream>
#include "windows.h"
#include <vector>

using namespace std;

#define W 37 + 42
#define H 18 + 11

enum Control
{
	pause,
	play
};

void gotoxy(const COORD &coord, char ch)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << ch;
}

void gotoxy(const COORD &coord, const char *ch)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	cout << ch;
}

struct Menu
{
	bool settings, about, exit;
	int record = 0;
} menu;

Control ctrl;

void Frame()
{
	COORD border;
	border.X = 41; border.Y = 10;
	gotoxy(border, R"(
					 #######################################
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #                                     #
					 #######################################)");

	border.Y = 0;

	gotoxy(border, R"(
					 _____             _                 __
					/  ___|           | |               /  |
					\ `--. _ __   __ _| | _____  __   __`| |
					 `--. \ '_ \ / _` | |/ / _ \ \ \ / / | |
					/\__/ / | | | (_| |   <  __/  \ V / _| |_
					\____/|_| |_|\__,_|_|\_\___|   \_/  \___/
	)");
}
void Menu()
{
	COORD title;
	title.X = 10;
	title.Y = 10;

	if(!menu.settings && !menu.about) // default
	{
		gotoxy(title,"1.New Game");

		title.Y = 12;
		gotoxy(title,"2.Setting");

		title.Y = 14;
		gotoxy(title,"3.About");

		title.Y = 16;
		gotoxy(title,"4.Exit");
	}
	else if(menu.settings) // settings
	{
		gotoxy(title,"0.Back");

		title.Y = 12;
		gotoxy(title,"NOT RELEASED");
	}
	else // about
	{
		gotoxy(title,"0.Back");

		title.Y = 12;
		gotoxy(title,"CONTROL:");

		title.Y = 13;
		gotoxy(title, "Pause: escape (esc)");

		title.Y = 14;
		gotoxy(title, "Movement: arrows");

		title.Y = 15;
		gotoxy(title,R"(       up)");

		title.Y = 16;
		gotoxy(title,R"(       ^)");

		title.Y = 17;
		gotoxy(title,R"(       |)");

		title.Y = 18;
		gotoxy(title,R"(left<-   ->right)");

		title.Y = 19;
		gotoxy(title,R"(       |)");

		title.Y = 20;
		gotoxy(title,R"(       v)");

		title.Y = 21;
		gotoxy(title,R"(      down)");

		title.Y = 23;
		gotoxy(title,"BUGS:");

		title.Y = 24;
		gotoxy(title, "snake collision");
	}

	title.X = 100;
	title.Y = 10;
	gotoxy(title,("Record: " + std::to_string(menu.record)).c_str());
}

class Fruit
{
	COORD pos;
public:
	Fruit()
	{
		Spawn();
	}
	void Spawn()
	{
		pos.X = rand() % W;
		pos.Y = rand() % H;
	}
	COORD &getPos()
	{
		return pos;
	}
	void Draw()
	{
		gotoxy(pos, '@');
	}
};

void Reset();

class Player
{
	vector<COORD> body;
	int dx = 0, dy = 0;
public:
	Player()
	{
		Spawn();
	}
	void Restart()
	{
		body.clear();
		Spawn();
		dx = dy = 0;
	}
	void Draw()
	{
		if(ctrl == play)
		{
			for(int i = body.size() - 1; i > 0; i--)
			{
				body[i].X = body[i-1].X;
				body[i].Y = body[i-1].Y;
			}

			body[0].X += dx;
			body[0].Y += dy;
		}

		if(Collision(body[0], 1) || ((body[0].X <= 41 || body[0].X >= 79) || (body[0].Y <= 10 || body[0].Y >= 29)))
		{
			Reset();
		}

		for(int i = 0; i < body.size(); i++)
		{
			gotoxy(body[i], '#');
		}
	}
	void Move(int dx, int dy)
	{
		this->dx = dx;
		this->dy = dy;
	}
	bool Eat(Fruit *apple)
	{
		if(Collision(apple->getPos(), 0))
		{
			Add();

			do
			{
				apple->Spawn();
			}
			while(Collision(apple->getPos(), 0));
			return true;
		}
		return false;
	}
private:
	bool Collision(COORD &coord, int start)
	{
		for(int i = start; i < body.size(); i++)
		{
			if(coord.X == body[i].X && coord.Y == body[i].Y) return true;
		}
		return false;
	}
	void Spawn()
	{
		COORD temp;
		temp.X = rand() % W;
		temp.Y = rand() % H;
		body.push_back(temp);
	}

	void Add()
	{
		COORD temp;
		temp.X = body[body.size() - 1].X;
		temp.Y = body[body.size() - 1].Y;
		body.push_back(temp);
	}
};

Player *snake = nullptr;
Fruit *apple = nullptr;

void Reset()
{
	snake->Restart();
	apple->Spawn();
	menu.record = 0;
}

void movePlayer()
{
	if(GetAsyncKeyState(VK_UP))
	{
		snake->Move(0, -1);
	}
	if(GetAsyncKeyState(VK_DOWN))
	{
		snake->Move(0, 1);
	}
	if(GetAsyncKeyState(VK_LEFT))
	{
		snake->Move(-1, 0);
	}
	if(GetAsyncKeyState(VK_RIGHT))
	{
		snake->Move(1, 0);
	}
	if(GetAsyncKeyState(VK_ESCAPE))
	{
		ctrl = pause;
	}
}
void Navigation()
{
	if(GetAsyncKeyState('0'))
	{
		if(menu.settings) menu.settings = false;
		if(menu.about) menu.about = false;
	}
	if(GetAsyncKeyState('1'))
	{
		if(!menu.settings && !menu.about)
		{
			if(snake == nullptr)
			{
				snake = new Player();
				apple = new Fruit();
			}
			else
			{
				Reset();
			}

			ctrl = play;
		}
	}
	if(GetAsyncKeyState('2'))
	{
		if(!menu.about) menu.settings = true;
	}
	if(GetAsyncKeyState('3'))
	{
		if(!menu.settings) menu.about = true;
	}
	if(GetAsyncKeyState('4'))
	{
		if(!menu.settings && !menu.about) menu.exit = true;
	}
	if(GetAsyncKeyState(VK_ESCAPE))
	{
		ctrl = play;
	}
}

int main()
{
	srand(time(NULL));

	while(!menu.exit)
	{
		Frame();
		Menu();

		if(apple != nullptr) apple->Draw();

		if(ctrl == play)
			movePlayer();
		else if(ctrl == pause)
			Navigation();

		if(snake != nullptr)
		{
			if(snake->Eat(apple)) menu.record++;
			snake->Draw();
		}

		Sleep(250);
		system("cls");
	}

	delete snake;
	delete apple;

	return 0;
}
