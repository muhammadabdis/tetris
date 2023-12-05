#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 18

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 18

#define ROTATE_KEY ' '
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define DOWN_KEY 's'

void DrawField(int field[]);
void DrawPiece(int field[], char piece[]);
void DrawScreen(char screen[], int field[]);
int Rotate(int r, int x, int y);
void HandleKeyPress(int c);

int piece = 0;
int px = FIELD_WIDTH / 2 - 2, py = 0;
int rotation = 0;

int main()
{
  char screen[SCREEN_WIDTH * SCREEN_WIDTH];
  int field[FIELD_WIDTH * FIELD_HEIGHT];

  char tetromino[7][17];
  strcpy(tetromino[0], "..A...A...A...A.");
  strcpy(tetromino[1], "..B..BB...B.....");
  strcpy(tetromino[2], ".....CC..CC.....");
  strcpy(tetromino[3], "..D..DD..D......");
  strcpy(tetromino[4], ".E...EE...E.....");
  strcpy(tetromino[5], ".F...F...FF.....");
  strcpy(tetromino[6], "..G...G..GG.....");

  while (1)
  {
    usleep(50 * 1000);

    DrawField(field);

    if (kbhit())
      HandleKeyPress(getch());

    DrawPiece(field, tetromino[piece]);

    DrawScreen(screen, field);

    system("cls");

    printf("%s", screen);
  }

  return 0;
}

void DrawField(int field[])
{
  int x, y;
  for (x = 0; x < FIELD_WIDTH; x++)
    for (y = 0; y < FIELD_HEIGHT; y++)
      field[y * FIELD_WIDTH + x] = (x == 0 || x == FIELD_WIDTH - 1 || y == FIELD_HEIGHT - 1) ? '#' : '.';
}

void DrawPiece(int field[], char piece[])
{
  int i, x, y;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
    {
      i = Rotate(rotation, x, y);
      if (piece[i] != '.')
        field[(y + py) * FIELD_WIDTH + (x + px)] = piece[i];
    }
}

void DrawScreen(char screen[], int field[])
{
  int i, x, y;

  for (y = 0; y < SCREEN_HEIGHT; y++)
    for (x = 0; x < SCREEN_WIDTH; x++)
    {
      i = y * SCREEN_WIDTH + x;
      screen[i] = (i % SCREEN_WIDTH != SCREEN_WIDTH - 1) ? ' ' : '\n';
    }

  screen[SCREEN_WIDTH * SCREEN_HEIGHT] = '\0';

  for (y = 0; y < FIELD_HEIGHT; y++)
    for (x = 0; x < FIELD_WIDTH; x++)
      screen[y * SCREEN_WIDTH + x] = field[y * FIELD_WIDTH + x];
}

int Rotate(int r, int x, int y)
{
  int i;
  switch (r % 4)
  {
  case 0: i = y * 4 + x; break;
  case 1: i = 12 + y - x * 4; break;
  case 2: i = 15 - y * 4 - x; break;
  case 3: i = 3 - y + x * 4; break;
  }
  return i;
}

void HandleKeyPress(int c)
{
  switch (c)
  {
  case ROTATE_KEY: rotation++; break;
  case LEFT_KEY: px--; break;
  case RIGHT_KEY: px++; break;
  case DOWN_KEY: py++; break;
  }
}
