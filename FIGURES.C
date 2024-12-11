#include "tetris.h"
#include <stdio.h>
#include <conio.h>
#include <dos.h>

// Game field
char field[FIELD_HEIGHT * FIELD_WIDTH];

// Sprites of figures
static const char square[16] =
{
  3, 3, 0, 0,
  3, 3, 0, 0,
  0, 0, 0, 0,
  0, 0, 0, 0
};

static const char l_figure[16] =
{
  8, 0, 0, 0,
  8, 0, 0, 0,
  8, 8, 0, 0,
  0, 0, 0, 0
};

// Figures
char *curr_figure=(char*)l_figure;
char curr_x = 0;
char curr_y = 0;

void drawField()
{
  unsigned char symbol = 0;
  unsigned int video_sym = 0;
  int row;
  int col;
  int far *vid = (int far*) VIDEO_BUF + FIELD_Y * SCREEN_WIDTH + FIELD_X;
  for(row = 0; row < FIELD_HEIGHT; row++)
  {
    *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_VERT_LINE; vid++;
    for(col = 0; col < FIELD_WIDTH; col++)
    {
      symbol = CHAR_SPACE;
      if (field[col])
      symbol = CHAR_FULL_BLOCK;
      video_sym = VID_SYM(field[col], symbol);
      *vid = video_sym; vid ++;
      *vid = video_sym; vid ++;
    }
    *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_VERT_LINE; vid++;
    vid += SCREEN_WIDTH - FIELD_WIDTH * FIELD_CHARW - BORDERS;
  }
  // Drawing bottom
  *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_LEFT_BOTTOM_CORNER; vid++;
  for(col = 0; col < FIELD_WIDTH; col++)
  {
    *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_HORIZ_LINE; vid++;
    *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_HORIZ_LINE; vid++;
  }
  *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_RIGHT_BOTTOM_CORNER; vid++;
}

void drawFigure()
{
  unsigned char symbol = 0;
  unsigned int video_sym = 0;
  int row;
  int col;
  int i;

  int far *vid = (int far*) VIDEO_BUF + FIELD_Y * SCREEN_WIDTH + FIELD_X;
  vid += curr_y * SCREEN_WIDTH + curr_x * FIELD_CHARW + 1;

  for(row = 0; row < FIGURE_HEIGHT; row++)
  {
    for(col = 0; col < FIGURE_WIDTH; col++)
    {
      symbol = CHAR_FULL_BLOCK;
      i = row * 4 + col;
      if (!curr_figure[i]) vid += 2;
      else
      {
        video_sym = VID_SYM(curr_figure[i], symbol);
        *vid = video_sym; vid ++;
        *vid = video_sym; vid ++;
      }
    }
    vid += SCREEN_WIDTH - FIGURE_WIDTH * FIELD_CHARW;
  }
}

void fixFigure()
{
  unsigned char point_x, point_y;
  unsigned char i, j;

  for(i = 0; i < FIGURE_WIDTH * FIGURE_HEIGHT; i++)
  {
    point_y = curr_y + i / FIGURE_HEIGHT;
    point_x = curr_x + i % FIGURE_WIDTH;
    j = point_y * FIELD_WIDTH + point_x;
    field[j] = curr_figure[i];
  }
    //char i;
}

void EraseFigure(void)
{
  unsigned char symbol = 0;
  unsigned int video_sym = 0;
  int row;
  int col;
  int i;

  int far *vid = (int far*) VIDEO_BUF + FIELD_Y * SCREEN_WIDTH + FIELD_X;
  vid += curr_y * SCREEN_WIDTH + curr_x * FIELD_CHARW + 1;

  for(row = 0; row < FIGURE_HEIGHT; row++)
  {
    for(col = 0; col < FIGURE_WIDTH; col++)
    {
      symbol = ' ';
      i = row * 4 + col;
      if (!curr_figure[i]) vid += 2;
      else
      {
        video_sym = VID_SYM(curr_figure[i], symbol);
        *vid = video_sym; vid ++;
        *vid = video_sym; vid ++;
      }
    }
    vid += SCREEN_WIDTH - FIGURE_WIDTH * FIELD_CHARW;
  }
}
char *NextFigure()
{
  char *figure = (char *)square;
  if(curr_figure == square)
    figure = (char *)l_figure;
  return figure;
}
