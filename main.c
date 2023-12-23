#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <unistd.h>

#define SCREEN_WIDTH 49
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
void RemoveLines(char field[], int lines[], int len);
void ScoreToString(char scoreboard[], int score);
void DrawScoreboard(char screen[], char scoreboard[]);
void Display(char screen[], char field[], char piece[], char scoreboard[]);
int Rotate(int r, int x, int y);
int DoesPieceFit(char field[], char piece[], int x, int y, int r);
int DoesLineTetris(char field[], int y);

int px = FIELD_WIDTH / 2 - 2, py = 0;
int rotation = 0;

int main()
{
  int isGameOver;
  int speed = 50;
  int tick;

  int piece = rand() % 7;
  int pn = 1;

  int score = 0;

  char screen[SCREEN_WIDTH * SCREEN_WIDTH];
  char field[FIELD_WIDTH * FIELD_HEIGHT];
  char scoreboard[13];

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
    usleep(2 * 1000);
    tick++;

    int lines[FIELD_HEIGHT - 1], li = -1;

    if (kbhit())
      switch (getch())
      {
      case ROTATE_KEY:
        if (DoesPieceFit(field, tetromino[piece], px, py, rotation + 1))
          rotation++;
        break;
      case LEFT_KEY:
        if (DoesPieceFit(field, tetromino[piece], px - 1, py, rotation))
          px--;
        break;
      case RIGHT_KEY:
        if (DoesPieceFit(field, tetromino[piece], px + 1, py, rotation))
          px++;
        break;
      case DOWN_KEY:
        if (DoesPieceFit(field, tetromino[piece], px, py + 1, rotation))
          py++;
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

        int y;
        for (y = 0; y < 4; y++)
          if (y + py < FIELD_HEIGHT - 1 && DoesLineTetris(field, y + py))
          {
            DrawTetrisLine(field, y + py);
            lines[++li] = y + py;
          }

        piece = rand() % 7;
        px = FIELD_WIDTH / 2 - 2, py = 0;
        rotation = rand() % 4;
        pn++;
        score += 25;
        isGameOver = !DoesPieceFit(field, tetromino[piece], px, py + 1, rotation);

        if (li >= 0) score += 1 << (li + 1) * 100;
        if (pn % 50 == 0 && speed >= 10) speed--;
      }
    }

    ScoreToString(scoreboard, score);
    Display(screen, field, tetromino[piece], scoreboard);

    if (li >= 0)
    {
      usleep(200 * 1000);
      RemoveLines(field, lines, li + 1);
      Display(screen, field, tetromino[piece], scoreboard);

      int i;
      for (i = 0; i < FIELD_HEIGHT - 1; i++)
        lines[i] = 0;
      li = -1;
    }
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

void Display(char screen[], char field[], char piece[], char scoreboard[])
{
  DrawField(screen, field);
  DrawPiece(screen, piece);
  DrawScoreboard(screen, scoreboard);
  system("cls");
  printf("%s", screen);
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
      if (piece[pi] != '.' && field[fi] != '.')
        return 0;
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

void RemoveLines(char field[], int lines[], int len)
{
  int i, j, y, x;
  for (i = 0; i < len; i++)
    for (y = lines[i]; y >= 0; y--)
      for (x = 1; x < FIELD_WIDTH - 1; x++)
      {
        j = (y - 1) * FIELD_WIDTH + x;
        field[y * FIELD_WIDTH + x] = (j) >= 0 ? field[j] : '.';
      }
}

void ScoreToString(char scoreboard[], int score)
{
  char label[] = "SCORE: ";

  int i, d, s = score;

  for (i = 0; i < 7; i++)
      scoreboard[i] = label[i];

  for (i = 12; i >= 7; i--)
    if (s > 0)
    {
      d = s % 10;
      scoreboard[i] = d + '0';
      s /= 10;
    }
    else
      scoreboard[i] = '0';
}

void DrawScoreboard(char screen[], char scoreboard[])
{
  int x;
  for (x = 0; x < 13; x++)
    screen[2 * SCREEN_WIDTH + (x + FIELD_WIDTH - 1 + 6)] = scoreboard[x];
}