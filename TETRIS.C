#include "tetris.h"
#include <stdio.h>
#include <conio.h>
#include <dos.h>

// imports
extern char *curr_figure;
extern char *field;
extern char curr_x;
extern char curr_y;

// Game managing variables
static unsigned long tics = MAX_GAME_SPEED;
static unsigned char key;
static unsigned long speed;

int main(void)
{
  int i;
  char can_exit = 0;
  speed = 1;
  initVideo();
  clrscr();
  for (i = 0; i < 200; i++)
    field[i] = 0;
  drawField();
  while (!can_exit)
  {
    onUpdate();
    if (kbhit())
    {
      key = getch();
      switch (key)
      {
      case CHAR_ESC:
        can_exit = 1;
        DrawScore(key);
        break;
      default:
        OnKeyPressed(key);
        break;
      }
    }
  }
  clrscr();
  terminateVideo();
  return 0;
}

void fixedUpdate(void)
{
  char buf[50] = {0};
  sprintf(buf, "tics = %6ld", (unsigned long)tics);
  drawAt(0, 1, buf, (COLOR_CYAN << 8));
  tics += speed;
}

void onUpdate(void)
{
  if (tics > MAX_GAME_SPEED)
  {
    tics = 0;
    DrawScore(key);
    curr_y++;
    if (checkCollides())
    {
      curr_y--;
      fixFigure();
      curr_y = -1;
      curr_x = 3;
      curr_figure = NextFigure();
    }
    else
    {
      curr_y--;
      EraseFigure();
      curr_y++;
      drawFigure();
    }
  }
  fixedUpdate();
}

void OnKeyPressed(unsigned char key)
{

  DrawScore(key);
  switch (key)
  {
  case VK_LEFT:
    if (!curr_x)
      break;
    curr_x--;
    if (!checkCollides())
    {
      curr_x++;
      EraseFigure();
      curr_x--;
      drawFigure();
    }
    else
      ++curr_x;
    break;
  case VK_RIGHT:
    curr_x++;
    if (!checkCollides())
    {
      curr_x--;
      EraseFigure();
      curr_x++;
      drawFigure();
    }
    else
      curr_x--;
    break;
  case VK_DOWN:
    tics = MAX_GAME_SPEED;
  }
}