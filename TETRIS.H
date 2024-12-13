
#ifndef H_TETRIS_H
#define H_TETRIS_H

#define MAX_GAME_SPEED          5000L

#define VIDEO_BUF               0x0b8000000
#define VID_SYM(_color, _sym) \
        ((_color << 8) | _sym)
#define SCREEN_WIDTH           40
#define SCREEN_HEIGHT          25

#define FIELD_X                10
#define FIELD_Y                2
#define FIELD_HEIGHT           20
#define FIELD_WIDTH            10
#define FIELD_CHARW            1
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

// funtion proto
void initVideo(void);
void terminateVideo(void);
void EraseFigure(void);
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

#endif
