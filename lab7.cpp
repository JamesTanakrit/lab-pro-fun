#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>

int x = 38, y = 20, moveDi = 1, sc=0;

void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void draw_ship(int x, int y)
{
	gotoxy(x, y);
	setcolor(2, 0);
	printf(" <-0-> ");
}

void erasor_ship(int x, int y)
{
	gotoxy(x, y);
	setcolor(0, 0);
	printf("       ");
}

void draw_bullet(int x, int y)
{
	gotoxy(x, y);
	setcolor(4, 0);
	printf("^");
}

void clear_bullet(int x, int y)
{
	gotoxy(x, y);
	setcolor(0, 0);
	printf(" ");
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void move(int a)
{
	if (a == 0 && x > 0)
	{
		erasor_ship(x, y);
		draw_ship(--x, y);
	}
	else if (a == 2 && x < 73)
	{
		erasor_ship(x, y);
		draw_ship(++x, y);
	}
	else
	{
		erasor_ship(x, y);
		draw_ship(x, y);
	}
}

void score(int x, int y)
{
	gotoxy(x, y);
	printf("score: ");
}

void add_score()
{
	sc = sc + 10;
	setcolor(7, 0);
	gotoxy(71, 0);
	printf("%d", sc);
}

struct star 
{
	int x, y;
	bool alive;
};
star enemy[20];

int main()
{
	char ch = '.';
	int bx[5], by[5], bullet[5] = { 0, 0, 0, 0, 0 }, current = 0;
	setcursor(0);
	srand(time(NULL));
	score(65, 0);
	for (int i = 0; i <= 20; i++)
	{
		enemy[i].x = (rand() % 61) + 10;
		enemy[i].y = (rand() % 4) + 2;
		setcolor(6, 0);
		gotoxy(enemy[i].x, enemy[i].y);
		printf("*");
	}
	do
	{
		if (_kbhit())
		{
			ch = _getch();
			if (ch == 'a')
			{
				moveDi = 0;
			}
			if (ch == 's')
			{
				moveDi = 1;
			}
			if (ch == 'd')
			{
				moveDi = 2;
			}
			if (ch == ' ' && bullet[current] == 0)
			{
				bullet[current] = 1;
				bx[current] = x + 3;
				by[current] = y + 1;
				current++;
				current %= 5;
				Beep(80, 10);
			}
			fflush(stdin);
		}
		for (int i = 0; i < 5; i++)
		{
			if (bullet[i] == 1)
			{
				clear_bullet(bx[i], by[i]);
				if (by[i] > 1)
				{
					draw_bullet(bx[i], --by[i]);
				}
				else
				{
					bullet[i] = 0;
				}
			}
			for (int j = 0; j < 20; j++)
			{
				if (bx[i] == enemy[j].x)
				{
					if (by[i] == enemy[j].y)
					{
						setcolor(0, 0);
						gotoxy(bx[i], by[i]);
						printf(" ");
						add_score();
						Beep(110, 30);
						bullet[i] = 0;
						enemy[j].x = (rand() % 61) + 10;
						enemy[j].y = (rand() % 4) + 2;
						gotoxy(enemy[j].x, enemy[j].y);
						setcolor(6, 0);
						printf("*");
						
					}
				}
			}
		}
		move(moveDi);
		Sleep(70);
	} while (ch != 'x');
	return 0;
}
