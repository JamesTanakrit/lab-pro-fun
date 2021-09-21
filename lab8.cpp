#include <stdio.h>
#include <windows.h>
#include <time.h>
#define scount 80
#define screen_x 80
#define screen_y 25
HANDLE rHnd;
HANDLE wHnd;
DWORD fdwMode;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
COORD star[scount];
int shx, shy, color = 7, count = 0;

int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
		ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void clear_buffer()
{
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 7;
		}
	}
}

void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}

void init_star()
{
	for (int i = 0; i < 80; i++)
	{
		star[i].X = rand() % 80;
		star[i].Y = rand() % 25;
	}
}

void star_fall()
{
	int i;
	for (i = 0; i < scount; i++)
	{
		if (star[i].Y >= screen_y - 1)
		{
			star[i] = { SHORT(rand() % screen_x),1 };
		}
		else
		{
			star[i] = { star[i].X, SHORT(star[i].Y + 1) };
		}
	}
}

void fill_star_to_buffer()
{
	for (int i = 0; i < 80; i++)
	{
		consoleBuffer[star[i].X + screen_x * star[i].Y].Char.AsciiChar = '*';
		consoleBuffer[star[i].X + screen_x * star[i].Y].Attributes = 7;
	}
}

void fill_ship_to_buffer()
{
	consoleBuffer[shx + screen_x * shy].Char.AsciiChar = 'O';
	consoleBuffer[shx + screen_x * shy].Attributes = color;
}

bool check_collision()
{
	for (int i = 0; i < 80; i++)
	{
		if (shx == star[i].X && shy == star[i].Y)
		{
			count++;
			star[i].X = rand() % 80;
			star[i].Y = 1;
			if (count == 10)
			{
				return true;
			}
		}
	}
	return false;
}

int main()
{
	int i;
	srand(time(NULL));
	init_star();
	bool play = true;
	DWORD numEvents = 0;
	DWORD numEventsRead = 0;
	setConsole(screen_x, screen_y);
	setMode();
	while (play)
	{
		GetNumberOfConsoleInputEvents(rHnd, &numEvents);
		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i)
			{
				if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true)
				{
					if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
					{
						play = false;
					}
					else if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c')
					{
						color = rand() % 255 + 1;
					}
				}
				else if (eventBuffer[i].EventType == MOUSE_EVENT) {
					int posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					int posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
					if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
					{
						color = rand() % 255 + 1;
					}
					else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED)
					{
						shx = posx;
						shy = posy;
					}
				}
			}
			delete[] eventBuffer;
		}
		star_fall();
		clear_buffer();
		fill_star_to_buffer();
		fill_ship_to_buffer();
		if (check_collision())
		{
			play = false;
		}
		fill_buffer_to_console();
		Sleep(100);
	}
	return 0;
}