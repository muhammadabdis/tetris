#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define FIELD_WIDTH 12
#define FIELD_HEIGHT 18

void DrawBoundary(int field[]);
void DrawPiece(int field[], char piece[], int px, int py, int rotation);
void Render(int field[]);
int Rotate(int r, int x, int y);

int main()
{
  int field[FIELD_WIDTH * FIELD_HEIGHT];

  char tetromino[7][17];
  strcpy(tetromino[0], "..A...A...A...A.");
  strcpy(tetromino[1], "..B..BB...B.....");
  strcpy(tetromino[2], ".....CC..CC.....");
  strcpy(tetromino[3], "..D..DD..D......");
  strcpy(tetromino[4], ".E...EE...E.....");
  strcpy(tetromino[5], ".F...F...FF.....");
  strcpy(tetromino[6], "..G...G..GG.....");

  int piece = 6;
  int px = FIELD_WIDTH / 2 - 2, py = 0;
  int rotation = 0;

  DrawBoundary(field);

  DrawPiece(field, tetromino[piece], px, py, rotation);

  Render(field);

  return 0;
}

void DrawBoundary(int field[])
{
  int x, y;
  for (x = 0; x < FIELD_WIDTH; x++)
    for (y = 0; y < FIELD_HEIGHT; y++)
      field[y * FIELD_WIDTH + x] = (x == 0 || x == FIELD_WIDTH - 1 || y == FIELD_HEIGHT - 1) ? '#' : '.';
}

void DrawPiece(int field[], char piece[], int px, int py, int rotation)
{
  int i, x, y;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
      field[(y + py) * FIELD_WIDTH + (x + px)] = piece[Rotate(rotation, x, y)];
}

void Render(int field[])
{
  int x, y;
  for (y = 0; y < FIELD_HEIGHT; y++)
  {
    for (x = 0; x < FIELD_WIDTH; x++)
      putchar(field[y * FIELD_WIDTH + x]);
    putchar('\n');
  }
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
