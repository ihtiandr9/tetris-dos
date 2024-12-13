#include "tetris.h"
#include <stdio.h>
#include <conio.h>
#include <dos.h>

extern char *curr_figure;
extern char curr_x;
extern char curr_y;
extern char *field;

//Game managing variables
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
  for(i = 0; i < 200; i++)
    field[i] = 0;
  drawField();
  while(!can_exit)
  {
    onUpdate();
    if(kbhit())
    {
      key = getch();
      switch( key )
      {
        case CHAR_ESC: can_exit = 1; DrawScore(key); break;
        default: OnKeyPressed(key); break;
      }
    }
  }
  clrscr();
  terminateVideo();
  return 0;
}


char checkCollides()
{
  unsigned char collides = 0;
  unsigned char point_x, point_y;
  unsigned char i, j;

  for(i = 0; i < FIGURE_WIDTH * FIGURE_HEIGHT; i++)
  {
    point_y = curr_y + i / FIGURE_HEIGHT;
    point_x = curr_x + i % FIGURE_WIDTH;
    j = point_y * FIELD_WIDTH + point_x;

    if(curr_figure[i] & field[j])
      collides = 1;
    if(curr_figure[i] && (point_x >= FIELD_WIDTH))
      collides = 1;
    if(curr_figure[i] && (point_y >= FIELD_HEIGHT))
      collides = 1;
  }
  return collides;
}

void fixedUpdate(void)
{
  char buf[50]={0};
  sprintf(buf,"tics = %6ld", (unsigned long) tics);
  drawAt(0, 1, buf, (COLOR_CYAN << 8));
  tics += speed;
}

void onUpdate(void)
{
    if(tics > MAX_GAME_SPEED)
    {
      tics = 0;
      DrawScore(key);
      curr_y++;
      if(checkCollides())
      {
        curr_y--;
        fixFigure();
        curr_y = -1;
        curr_x = 3;
        curr_figure = NextFigure();
      } else {
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
  switch(key)
  {
    case VK_LEFT:
      if(!curr_x) break;
      curr_x--;
      if(!checkCollides())
      {
          curr_x++;
          EraseFigure();
          curr_x--;
          drawFigure();
      }else
           ++curr_x;
      break;
    case VK_RIGHT:
      curr_x++;
      if(checkCollides())
        curr_x--;
        else
        {
          curr_x--;
          EraseFigure();
          curr_x++;
          drawFigure();
        }
      break;
    case VK_DOWN:
      tics = MAX_GAME_SPEED;
  }
}