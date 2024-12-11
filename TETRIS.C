
#define VIDEO_BUF               0x0b8000000
#define VID_SYM(_color, _sym) \
        ((_color << 8) | _sym)
#define SCREEN_WIDTH           80
#define SCREEN_HEIGHT          25

#define FIELD_X                30
#define FIELD_Y                2
#define FIELD_HEIGHT           20
#define FIELD_WIDTH            10
#define FIELD_CHARW            2
#define BORDERS                2

#define CHAR_SPACE                      0X020
#define CHAR_VERT_LINE                  0x0b3
#define CHAR_HORIZ_LINE                 0x0c4
#define CHAR_LEFT_BOTTOM_CORNER         0x0c0
#define CHAR_RIGHT_BOTTOM_CORNER        0x0d9
#define CHAR_FULL_BLOCK                 0x0db
#define CHAR_ESC                        0x01b

#define VK_LEFT                         0x4b
#define VK_RIGHT                        0x4d
#define VK_DOWN                         0x50

#define COLOR_WHITE_ON_BLACK            0x0f
#define COLOR_GREY                      0x08
#define COLOR_CYAN                      0x03

#define FIGURE_HEIGHT                   4
#define FIGURE_WIDTH                    4

#define MAX_GAME_SPEED          50000L

#include <stdio.h>
#include <conio.h>
#include <dos.h>

// funtion proto
void drawField(void);
void drawFigure(void);
void DrawScore(unsigned long value);
void onUpdate(void);
void OnKeyPressed(unsigned char key);
char *NextFigure(void);
char checkCollides(void);
void fixFigure(void);
void drawAt(int x, int y, char *str, int color);
void drawCharAt(int x, int y, char ch, int color);
void fixedUpdate(void);

// Game field
static char field[FIELD_HEIGHT * FIELD_WIDTH];

//Game managing variables
static unsigned long tics = MAX_GAME_SPEED;
static unsigned char key;
static unsigned long speed;

// Figures
static char *curr_figure;
static char curr_x = 0;
static char curr_y = 0;

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

int main(void)
{
  int i;
  char can_exit = 0;
  speed = 1;
  clrscr();
  for(i = 0; i < 200; i++)
    field[i] = 0;
  drawField();
  curr_figure = (char*) l_figure;
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
  return 0;
}

void drawField()
{
  unsigned char symbol = 0;
  unsigned int video_sym = 0;
  int row;
  int col;
  int far *vid = (int far*) VIDEO_BUF + FIELD_Y * 80 + FIELD_X;
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
    vid += 80 - FIELD_WIDTH * FIELD_CHARW - BORDERS;
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

  int far *vid = (int far*) VIDEO_BUF + FIELD_Y * 80 + FIELD_X;
  vid += curr_y * 80 + curr_x * FIELD_CHARW + 1;

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
    vid += 80 - FIGURE_WIDTH * FIELD_CHARW;
  }
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

char *NextFigure()
{
  char *figure = (char *)square;
  if(curr_figure == square)
    figure = (char *)l_figure;
  return figure;
}

void drawCharAt(int x, int y, char ch, int color)
{
  int far *vid = (int far*) VIDEO_BUF + 80 * y + x;
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
   char score_buffer[50];
   sprintf(score_buffer,"Youre score 0x%lx",(unsigned long) value);
   drawAt(0, 0, score_buffer, (COLOR_GREY << 12) | (COLOR_CYAN << 8));
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

  int far *vid = (int far*) VIDEO_BUF + FIELD_Y * 80 + FIELD_X;
  vid += curr_y * 80 + curr_x * FIELD_CHARW + 1;

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
    vid += 80 - FIGURE_WIDTH * FIELD_CHARW;
  }
}

void fixedUpdate(void)
{
  char buf[50];
  sprintf(buf,"tics = %6ld", (unsigned long) tics);
  drawAt(0, 1, buf, (COLOR_GREY << 12) | (COLOR_CYAN << 8));
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
      if(checkCollides())
        curr_x++;
        else
        {
          curr_x++;
          EraseFigure();
          curr_x--;
          drawFigure();
        }
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