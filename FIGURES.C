#include "tetris.h"
#include <stdio.h>
#include <dos.h>

// Game field
char field[FIELD_HEIGHT * FIELD_WIDTH] = {0};

// Sprites of figures
static const char square[16] =
    {
        3, 3, 0, 0,
        3, 3, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0};

static const char l_figure[16] =
    {
        8, 0, 0, 0,
        8, 0, 0, 0,
        8, 8, 0, 0,
        0, 0, 0, 0};

// Figures
char *curr_figure = (char *)l_figure;
char curr_x = 0;
char curr_y = 0;

void fixFigure()
{
  unsigned char point_x, point_y;
  unsigned char i, j;

  for (i = 0; i < FIGURE_WIDTH * FIGURE_HEIGHT; i++)
  {
    point_y = curr_y + i / FIGURE_HEIGHT;
    point_x = curr_x + i % FIGURE_WIDTH;
    j = point_y * FIELD_WIDTH + point_x;
    if (curr_figure[i])
      field[j] = curr_figure[i];
  }
}

void EraseFigure(void)
{
  unsigned char symbol = 0;
  unsigned int video_sym = 0;
  int row, col, scale;
  int i;

  int far *vid = (int far *)VIDEO_BUF + FIELD_Y * SCREEN_WIDTH + FIELD_X;
  vid += curr_y * SCREEN_WIDTH + curr_x * FIELD_CHARW + 1;

  for (row = 0; row < FIGURE_HEIGHT; row++)
  {
    for (col = 0; col < FIGURE_WIDTH; col++)
    {
      symbol = ' ';
      i = row * 4 + col;
      if (!curr_figure[i])
        vid += FIELD_CHARW;
      else
      {
        video_sym = VID_SYM(curr_figure[i], symbol);
        for (scale = 0; scale < FIELD_CHARW; scale++)
        {
          *vid = video_sym;
          vid++;
        }
      }
    }
    vid += SCREEN_WIDTH - FIGURE_WIDTH * FIELD_CHARW;
  }
}
char *NextFigure()
{
  char *figure = (char *)square;
  if (curr_figure == square)
    figure = (char *)l_figure;
  return figure;
}

unsigned char checkCollides()
{
  unsigned char collides = 0;
  unsigned char point_x, point_y;
  unsigned char i, j;

  for (i = 0; i < FIGURE_WIDTH * FIGURE_HEIGHT; i++)
  {
    point_y = curr_y + i / FIGURE_HEIGHT;
    point_x = curr_x + i % FIGURE_WIDTH;
    j = point_y * FIELD_WIDTH + point_x;

    if ((curr_figure[i] != 0) && (field[j] != 0))
      collides = 1;
    if (curr_figure[i] && (point_x >= FIELD_WIDTH))
      collides = 1;
    if (curr_figure[i] && (point_y >= FIELD_HEIGHT))
      collides = 1;
  }
  return collides;
}
