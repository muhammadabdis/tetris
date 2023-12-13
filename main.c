#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 20

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 18

#define ROTATE_KEY 'w'
#define LEFT_KEY 'a'
#define RIGHT_KEY 'd'
#define DOWN_KEY 's'

void DrawBoundary(char field[]);
void DrawPiece(char screen[], char piece[]);
void DrawField(char screen[], char field[]);
void LockPiece(char screen[], char piece[]);
void DrawTetrisLine(char field[], int y);
int Rotate(int r, int x, int y);
int DoesPieceFit(char field[], char piece[], int x, int y, int r);
int DoesLineTetris(char field[], int y);

int piece = 0;
int px = FIELD_WIDTH / 2 - 2, py = 0;
int rotation = 0;

int main()
{
  int isGameOver;

  char screen[SCREEN_WIDTH * SCREEN_WIDTH];
  char field[FIELD_WIDTH * FIELD_HEIGHT];

  int speed = 50;
  int tick;

  char tetromino[7][17];
  strcpy(tetromino[0], "..A...A...A...A.");
  strcpy(tetromino[1], "..B..BB...B.....");
  strcpy(tetromino[2], ".....CC..CC.....");
  strcpy(tetromino[3], "..D..DD..D......");
  strcpy(tetromino[4], ".E...EE...E.....");
  strcpy(tetromino[5], ".F...F...FF.....");
  strcpy(tetromino[6], "..G...G..GG.....");

  DrawBoundary(field);

  while (!isGameOver)
  {
    usleep(20 * 1000);

    tick++;

    if (kbhit())
      switch (getch())
      {
      case ROTATE_KEY:
        if (DoesPieceFit(field, tetromino[piece], px, py, rotation + 1)) rotation++;
        break;
      case LEFT_KEY:
        if (DoesPieceFit(field, tetromino[piece], px - 1, py, rotation)) px--;
        break;
      case RIGHT_KEY:
        if (DoesPieceFit(field, tetromino[piece], px + 1, py, rotation)) px++;
        break;
      case DOWN_KEY:
        if (DoesPieceFit(field, tetromino[piece], px, py + 1, rotation)) py++;
        break;
      }
    
    if (tick >= speed)
    {
      tick = 0;

      if (DoesPieceFit(field, tetromino[piece], px, py + 1, rotation))
        py++;
      else
      {
        LockPiece(field, tetromino[piece]);

        int x, y;
        for (y = 0; y < 4; y++)
          if (y + py < FIELD_HEIGHT - 1 && DoesLineTetris(field, y + py))
            DrawTetrisLine(field, y + py);

        piece = rand() % 7;
        px = FIELD_WIDTH / 2 - 2, py = 0;
        rotation = 0;

        isGameOver = !DoesPieceFit(field, tetromino[piece], px, py + 1, rotation);
      }
    }

    DrawField(screen, field);

    DrawPiece(screen, tetromino[piece]);

    system("cls");

    printf("%s", screen);
  }

  return 0;
}

void DrawBoundary(char field[])
{
  int x, y;
  for (x = 0; x < FIELD_WIDTH; x++)
    for (y = 0; y < FIELD_HEIGHT; y++)
      field[y * FIELD_WIDTH + x] = (x == 0 || x == FIELD_WIDTH - 1 || y == FIELD_HEIGHT - 1) ? '#' : '.';
  
  field[FIELD_WIDTH * FIELD_HEIGHT] = '\0';
}

void DrawPiece(char screen[], char piece[])
{
  int i, x, y;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
    {
      i = Rotate(rotation, x, y);
      if (piece[i] != '.')
        screen[(y + py) * SCREEN_WIDTH + (x + px)] = piece[i];
    }
}

void DrawField(char screen[], char field[])
{
  int i, x, y;

  for (y = 0; y < SCREEN_HEIGHT; y++)
    for (x = 0; x < SCREEN_WIDTH; x++)
      screen[y * SCREEN_WIDTH + x] = (x != SCREEN_WIDTH - 1) ? ' ' : '\n';

  for (y = 0; y < FIELD_HEIGHT; y++)
    for (x = 0; x < FIELD_WIDTH; x++)
      screen[(y + 2) * SCREEN_WIDTH + x] = field[(y + 2) * FIELD_WIDTH + x];

  screen[SCREEN_WIDTH * SCREEN_HEIGHT] = '\0';
}

void LockPiece(char field[], char piece[])
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

void DrawTetrisLine(char field[], int y)
{
  int x;
  for (x = 1; x < FIELD_WIDTH - 1; x++)
    field[y * FIELD_WIDTH + x] = '=';
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

int DoesPieceFit(char field[], char piece[], int px, int py, int r)
{
  int x, y, pi, fi;

  for (y = 0; y < 4; y++)
    for (x = 0; x < 4; x++)
    {
      pi = Rotate(r, x, y);
      fi = (py + y) * FIELD_WIDTH + px + x;
      if (piece[pi] != '.' && field[fi] != '.') return 0;
    }

  return 1;
}

int DoesLineTetris(char field[], int y)
{
  int tetris = 1;
  int x;

  for (x = 1; x < FIELD_WIDTH - 1; x++)
    tetris &= field[y * FIELD_WIDTH + x] != '.';
  
  return tetris;
}
