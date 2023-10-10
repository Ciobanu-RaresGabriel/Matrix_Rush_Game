#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

#define lin 28
#define col 40

void initializeMatrix(char back[lin][col]);
void displayMatrix(char back[lin][col]);
void descriptions_and_instructions(char back[lin][col]);
void moveBar(char back[][col], short &bar_position, char button, HANDLE hConsole);
void random_shape(char back[lin][col]);
void Level_update(short Level, double *time_interval,short *updating_times_limit);
void gameover_check(char back[lin][col], bool &continue_game);

int main()
{
	char back[lin][col];
	int start_time = clock(), curent_time;
	float past_time = 0.0;
	double time_interval = 0.35;
	short updating_times=0;
	short updating_times_limit = 6;
	char button = ' ';
	short bar_position = 20;
	bool continue_game = true;
	short Level = 1;
	short score = 0, level_check = 1;
	int while_loop_starting_time;

	srand(time(NULL));
	
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(consoleHandle, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &cursorInfo);


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT windowSize = { 0, 0, col - 1, lin - 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fontInfo.dwFontSize.X = 15;
	fontInfo.dwFontSize.Y = 30;
	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
	
	
	initializeMatrix(back);
	descriptions_and_instructions(back);
	displayMatrix(back);	
	
	printf("\tPress any key to start\n\t\tGOOD LUCK!");	
	PlaySound(TEXT("1.wav"), NULL, SND_FILENAME | SND_ASYNC);
	_getch();
	system("cls");
	PlaySound(TEXT("2.wav"), NULL, SND_FILENAME | SND_ASYNC);	

	initializeMatrix(back);
	while_loop_starting_time = clock();
	random_shape(back);
	while (continue_game)
	{
		curent_time = clock();
		past_time = (float)(curent_time - start_time) / CLOCKS_PER_SEC;

		if (_kbhit())
		{
			button = _getch();
			moveBar(back,bar_position,button,hConsole);
		}

		SetConsoleCursorPosition(consoleHandle, cursorPosition);

		displayMatrix(back);

		gameover_check(back, continue_game);
		


		if (past_time >= time_interval)
		{
			for (short i = lin - 3; i > 0; i--)
				for (short j = 1; j < col - 1; j++)
				{
					back[i][j] = back[i - 1][j];
					back[i - 1][j] = ' ';
				}
			start_time = curent_time;
			updating_times++;

		}

		if (updating_times >= updating_times_limit)
		{
			random_shape(back);
			updating_times = 0;
		}

		score = (curent_time - while_loop_starting_time) / 100;
		if ((score / 100>Level - 1))
			Level++;
		else {
			printf("");
			//Nothing happens here, added just for security
			 }
		if (level_check != Level)
		{
			level_check = Level;
			PlaySound(TEXT("3.wav"), NULL, SND_FILENAME | SND_ASYNC);
		}
		else {
			printf("");
			//Nothing happens here, added just for security
		}
		printf("\n\tScore -> %d \t Level: %d\n", score, Level);
		Level_update(Level,&time_interval,&updating_times_limit);
		

	}
	SetConsoleCursorPosition(consoleHandle, cursorPosition);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

	displayMatrix(back);	
	PlaySound(TEXT("4.wav"), NULL, SND_FILENAME | SND_ASYNC);
	printf("\n\tScore -> %d \t Level: %d\n", score, Level);
	if (Level >= 9)
		printf("\t   Congratulations!\n\tY  O  U     W  I  N  !\t\t");
	else
		printf("\t C'est fini la comedie !!!\n   MUHAHAHAHAHAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	Sleep(5000);

	return 0;
}

void initializeMatrix(char back[lin][col])
{
	for (short line = 0; line < lin; line++)
	{
		for (short column = 0; column < col; column++)
		{
			if (column == 0 || column == col - 1)
				back[line][column] = '|';
			else if (line == (lin - 1) && column > 0 && column < (col - 1))
				back[line][column] = '~';
			else
				back[line][column] = ' ';
		}

	}
	back[lin - 2][18] = back[lin - 2][19] = back[lin - 2][20] = back[lin - 2][21] = back[lin - 2][17] = back[lin - 2][22] = back[lin - 2][23] = 'þ';

}
void descriptions_and_instructions(char back[lin][col])
{
	char description[] = "Welcome to the Matrix Rush Game!";
	char  description2[] = "Use 'A' and/or '4' to move the bar left";
	char  description3[] = "Use 'D' and/or '6' to move the bar right";
	char  description4[] = "Your score will increase as you survive";
	char  description5[] = "The level will increase every 100 points";
	char  description6[] = "Let's see how long you can survive!";
	char  description7[] = "!*_*!Avoid the falling shapes!*_*!";

	
	for (int i = 0; i < strlen(description); i++) {
		back[lin / 2 - 6][col / 2 - strlen(description) / 2 + i] = description[i];
	}
	for (int i = 0; i < strlen(description2); i++) {
		back[lin / 2 - 1][col / 2 - strlen(description2) / 2 + i - 1] = description2[i];
	}
	for (int i = 0; i < strlen(description3); i++) {
		back[lin / 2 + 1][col / 2 - strlen(description3) / 2 + i] = description3[i];
	}
	for (int i = 0; i < strlen(description4); i++) {
		back[lin / 2 + 3][col / 2 - strlen(description4) / 2 + i - 1] = description4[i];
	}
	for (int i = 0; i < strlen(description5); i++) {
		back[lin / 2 + 5][col / 2 - strlen(description5) / 2 + i] = description5[i];
	}
	for (int i = 0; i < strlen(description6); i++) {
		back[lin / 2 + 9][col / 2 - strlen(description6) / 2 + i] = description6[i];
	}
	for (int i = 0; i < strlen(description7); i++) {
		back[lin / 2 - 3][col / 2 - strlen(description7) / 2 + i] = description7[i];
	}

}
void displayMatrix(char back[lin][col])
{
	for (short line = 5; line < lin; line++)
	{
		for (short column = 0; column < col; column++)
		{
			printf("%c", back[line][column]);
		}
		printf("\n");
	}
}

void moveBar(char back[][col], short &bar_position, char button, HANDLE hConsole) {
	if ((button == 'a' || button == 'A' || button == '4') && bar_position >= 5) {
		bar_position--;
		back[lin - 2][bar_position + 4] = ' ';
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	else if ((button == 'd' || button == 'D' || button == '6') && bar_position <= 34) {
		bar_position++;
		back[lin - 2][bar_position - 4] = ' ';
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}

	back[lin - 2][bar_position - 2] = back[lin - 2][bar_position - 1] = back[lin - 2][bar_position] = back[lin - 2][bar_position + 1] = back[lin - 2][bar_position - 3] = back[lin - 2][bar_position + 2] = back[lin - 2][bar_position + 3] = 'þ';
}

void random_shape(char back[lin][col])
{
	short column_random = rand() % 34 + 3;
	short random_char = rand() % 16;
	short shape = rand() % 24;
	switch (random_char)
	{
	case 0:
		random_char = '*';
		break;
	case 1:
		random_char = 'œ';
		break;
	case 2:
		random_char = '™';
		break;
	case 3:
		random_char = 'ª';
		break;
	case 4:
		random_char = '<';
		break;
	case 5:
		random_char = '^';
		break;
	case 6:
		random_char = '#';
		break;
	case 7:
		random_char = '=';
		break;
	case 8:
		random_char = '®';
		break;
	case 9:
		random_char = '0';
		break;
	case 10:
		random_char = '©';
		break;
	case 11:
		random_char = '¤';
		break;
	case 12:
		random_char = 'Ð';
		break;
	case 13:
		random_char = '«';
		break;
	case 14:
		random_char = 'þ';
		break;
	case 15:
		random_char = '•';
		break;
	default:
		random_char = '¤';
		break;
	}
	switch (shape)
	{
	case 0:
		back[0][column_random] = back[1][column_random - 1] = back[1][column_random] = back[1][column_random + 1] = back[2][column_random - 2] = back[2][column_random - 1] = back[2][column_random] = back[2][column_random + 1] = back[2][column_random + 2] = back[3][column_random] = back[4][column_random - 1] = back[4][column_random + 1] = back[4][column_random] = random_char;
		break;
	case 1:
		back[0][column_random] = back[1][column_random - 1] = back[1][column_random] = back[1][column_random + 1] = back[2][column_random - 2] = back[2][column_random - 1] = back[2][column_random] = back[2][column_random + 1] = back[2][column_random + 2] = back[3][column_random - 1] = back[3][column_random] = back[3][column_random + 1] = back[4][column_random] = random_char;
		break;
	case 2:
		back[0][column_random - 1] = back[0][column_random + 1] = back[1][column_random - 2] = back[1][column_random] = back[1][column_random + 2] = back[2][column_random] = back[3][column_random - 2] = back[3][column_random] = back[3][column_random + 2] = back[4][column_random - 1] = back[4][column_random + 1] = random_char;
		break;
	case 3:
		back[0][column_random - 1] = back[0][column_random + 1] = back[1][column_random - 2] = back[1][column_random - 1] = back[1][column_random] = back[1][column_random + 1] = back[1][column_random + 2] = back[2][column_random - 2] = back[2][column_random - 1] = back[2][column_random] = back[2][column_random + 1] = back[2][column_random + 2] = back[3][column_random - 1] = back[3][column_random] = back[3][column_random + 1] = back[4][column_random] = random_char;
		break;
	case 4:
		back[0][column_random] = back[1][column_random - 1] = back[1][column_random] = back[1][column_random + 1] = back[2][column_random - 1] = back[2][column_random] = back[2][column_random + 1] = random_char;
		break;
	case 5:
		back[0][column_random] = back[1][column_random] = back[2][column_random] = back[3][column_random] = back[4][column_random] = random_char;
		break;
	case 6:
		back[0][column_random - 1] = back[0][column_random] = back[0][column_random + 1] = back[0][column_random] = back[1][column_random - 1] = back[1][column_random + 1] = back[2][column_random - 1] = back[2][column_random] = back[2][column_random + 1] = random_char;
		break;
	case 7:
		back[0][column_random] = back[0][column_random + 1] = back[0][column_random + 2] = back[1][column_random - 1] = back[1][column_random] = back[1][column_random + 1] = back[2][column_random - 2] = back[2][column_random - 1] = back[2][column_random] = back[3][column_random - 1] = back[3][column_random] = back[3][column_random + 1] = back[4][column_random + 1] = back[4][column_random] = back[4][column_random + 2] = random_char;
		break;
	case 8:

		back[0][column_random] = back[0][column_random - 1] = back[0][column_random - 2] = back[0][column_random] = back[0][column_random + 1] = back[0][column_random + 2] = back[1][column_random - 2] = back[1][column_random - 1] = back[1][column_random] = back[1][column_random + 1] = back[2][column_random] = back[2][column_random - 1] = back[2][column_random - 2] = back[3][column_random - 1] = back[3][column_random - 2] = back[4][column_random - 2] = random_char;
		break;

	case 9:
		back[0][column_random] = back[0][column_random + 1] = back[0][column_random + 2] = back[0][column_random] = back[0][column_random - 1] = back[0][column_random - 2] = back[1][column_random + 2] = back[1][column_random + 1] = back[1][column_random] = back[1][column_random - 1] = back[2][column_random] = back[2][column_random + 1] = back[2][column_random + 2] = back[3][column_random + 1] = back[3][column_random + 2] = back[4][column_random + 2] = random_char;
		break;
	case 10:
	{
		back[0][column_random - 1] = back[1][column_random - 2] = back[2][column_random - 2] = back[3][column_random - 1] = back[1][column_random - 1] = back[2][column_random - 1] = back[1][column_random] = back[2][column_random] = back[2][column_random + 1] = back[2][column_random + 2] = back[1][column_random + 1] = back[1][column_random + 2] = random_char;
		break;
	}
	case 11:
		back[0][column_random - 2] = back[0][column_random + 2] = back[0][column_random - 1] = back[0][column_random + 1] = back[0][column_random] = back[1][column_random] = back[3][column_random] = back[2][column_random] = back[4][column_random] = random_char;
		break;
	case 12:
		back[0][column_random - 2] = back[0][column_random - 1] = back[0][column_random] = back[0][column_random + 1] = back[1][column_random - 2] = back[1][column_random + 1] = back[2][column_random - 1] = back[2][column_random - 2] = back[2][column_random] = back[3][column_random - 2] = back[3][column_random] = back[4][column_random + 1] = back[4][column_random - 2] = random_char;
		break;
	case 13:
		back[0][column_random] = back[1][column_random] = back[2][column_random] = back[3][column_random] = back[3][column_random - 1] = back[4][column_random] = back[3][column_random + 1] = random_char;
		break;
	case 14:
		back[2][column_random] = back[2][column_random + 1] = back[2][column_random + 2] = back[2][column_random - 1] = back[2][column_random - 2] = back[0][column_random] = back[1][column_random] = back[3][column_random] = back[4][column_random] = random_char;
		break;
	case 15:
		back[0][column_random] = back[0][column_random - 1] = back[0][column_random + 1] = back[1][column_random - 2] = back[1][column_random + 2] = back[2][column_random - 3] = back[2][column_random + 3] = back[3][column_random] = random_char;
		break;
	case 16:
		back[0][column_random] = back[1][column_random] = back[2][column_random] = back[3][column_random] = back[4][column_random] = back[1][column_random + 1] = back[1][column_random - 1] = back[3][column_random - 1] = back[3][column_random + 1] = random_char;
		break;
	case 17:
		back[0][column_random - 2] = back[0][column_random + 2] = back[1][column_random - 1] = back[1][column_random + 1] = back[2][column_random] = back[3][column_random - 1] = back[3][column_random + 1] = back[4][column_random - 2] = back[4][column_random + 2] = random_char;
		break;
	case 18:
		back[0][column_random] = back[1][column_random] = back[2][column_random] = back[3][column_random] = back[4][column_random] = back[0][column_random + 2] = back[0][column_random + 1] = back[4][column_random - 1] = back[4][column_random - 2] = random_char;
		break;
	case 19:
		back[0][column_random] = back[0][column_random - 1] = back[0][column_random - 2] = back[0][column_random + 1] = back[0][column_random + 2] = back[1][column_random - 2] = back[3][column_random - 2] = back[2][column_random - 2] = back[2][column_random - 1] = back[2][column_random] = back[2][column_random + 1] = back[2][column_random + 2] = back[4][column_random - 2] = back[4][column_random - 1] = back[4][column_random] = back[4][column_random + 1] = back[4][column_random + 2] = random_char;
		break;
	case 20:
		back[1][column_random-2] = back[1][column_random-1] = back[2][column_random-2] = back[2][column_random-1] = random_char;
		break;
	case 21:
		back[0][column_random] = back[1][column_random - 1] = back[2][column_random] = back[1][column_random + 1] = random_char;
		break;
	case 22:
		back[0][column_random-2] = back[1][column_random-1] = back[2][column_random - 2] = back[3][column_random-1] = back[4][column_random -2] = random_char;
		break;
	case 23:
		back[0][column_random +1] = back[0][column_random +2] = back[1][column_random +1] = back[1][column_random +2]= random_char;
		break;
	default:
		back[0][column_random - 2] = back[0][column_random - 1] = back[0][column_random] = back[0][column_random + 1] = back[1][column_random - 2] = back[1][column_random + 1] = back[2][column_random - 1] = back[2][column_random - 2] = back[2][column_random] = back[3][column_random - 2] = back[3][column_random] = back[4][column_random + 1] = back[4][column_random - 2] = random_char;
		break;
	}
}

void Level_update(short Level, double *time_interval, short *updating_times_limit)
{
	switch (Level)
	{
	case 1:
		*time_interval = 0.35;
		*updating_times_limit = 6;
	case 2:
		*time_interval = 0.25;
		*updating_times_limit = 10;
		break;
	case 3:
		*time_interval = 0.15;
		*updating_times_limit = 11;
		break;
	case 4:
		*time_interval = 0.07;
		*updating_times_limit = rand() % 6 + 10;
		break;
	case 5:
		*time_interval = 0.03;
		*updating_times_limit = rand() % 6 + 13;
		break;
	case 6:
		*time_interval = 0.013;
		*updating_times_limit = rand() % 6 + 15;
		break;
	case 7:
		*time_interval = 0.010;
		*updating_times_limit = rand() % 6 + 17;
		break;
	case 8:
		*time_interval = 0.008;
		*updating_times_limit = rand() % 6 + 18;
		break;
	case 9:
		*time_interval = 0.007;
		*updating_times_limit = rand() % 6 + 18;
		break;
	default:
		*time_interval = 0.0045;
		*updating_times_limit = rand() % 6 + 16;
		break;
	}
}

void gameover_check(char back[lin][col], bool &continue_game)
{
	for (short p = 1; p < col - 1; p++)
	{
		if (back[25][p] != ' ' && back[26][p] != ' ')
		{
			continue_game = false;
			break;
		}
		else
		{
			continue_game = true;	
			
		}
	}
}
