#include "tetris.h"
#include <stdio.h>
#include <conio.h>
#include <dos.h>

extern char *curr_figure;
extern char curr_x;
extern char curr_y;
extern char *field;

void initVideo(void)
{
  asm{
    mov ax, 01h
    int 10h
  }
}

void terminateVideo(void)
{
  asm{
    mov ax, 03h
    int 10h
  }
}


void drawCharAt(int x, int y, char ch, int color)
{
  int far *vid = (int far*) VIDEO_BUF + SCREEN_WIDTH * y + x;
  *vid = (color | ch);
}

void drawAt(int x, int y, char *str, int color)
{
  int i;
  for(i = 0; str[i]; i++)
     drawCharAt(x + i, y, str[i], color);
}

void DrawScore(unsigned long value)
{
   char score_buffer[50]={0};
   sprintf(score_buffer,"Youre score 0x%lx",(unsigned long) value);
   drawAt(0, 0, score_buffer, (COLOR_CYAN << 8));
}

void drawField()
{
  unsigned char symbol = 0;
  unsigned int video_sym = 0;
  int row, col, scale;
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
      for(scale = 0; scale < FIELD_CHARW; scale++)
          { *vid = video_sym; vid ++;}
    }
    *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_VERT_LINE; vid++;
    vid += SCREEN_WIDTH - FIELD_WIDTH * FIELD_CHARW - BORDERS;
  }
  // Drawing bottom
  *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_LEFT_BOTTOM_CORNER; vid++;
  for(col = 0; col < FIELD_WIDTH; col++)
  {
      for(scale = 0; scale < FIELD_CHARW; scale++)
          { *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_HORIZ_LINE; vid++;}
  }
  *vid = COLOR_WHITE_ON_BLACK << 8 | CHAR_RIGHT_BOTTOM_CORNER; vid++;
}

void drawFigure()
{
  unsigned char symbol = 0;
  unsigned int video_sym = 0;
  int row, col, scale;
  int i;

  int far *vid = (int far*) VIDEO_BUF + FIELD_Y * SCREEN_WIDTH + FIELD_X;
  vid += curr_y * SCREEN_WIDTH + curr_x * FIELD_CHARW + 1;

  for(row = 0; row < FIGURE_HEIGHT; row++)
  {
    for(col = 0; col < FIGURE_WIDTH; col++)
    {
      symbol = CHAR_FULL_BLOCK;
      i = row * 4 + col;
      if (!curr_figure[i]) vid += FIELD_CHARW;
      else
      {
        video_sym = VID_SYM(curr_figure[i], symbol);
       for(scale = 0; scale < FIELD_CHARW; scale++)
           { *vid = video_sym; vid ++;}
      }
    }
    vid += SCREEN_WIDTH - FIGURE_WIDTH * FIELD_CHARW;
  }
}
