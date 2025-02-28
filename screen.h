#ifndef SCREEN_H
#define SCREEN_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define GET_SCREEN_BUFFER(screen, name, width) char (*name)[width] = (char (*)[width])screen.buffer // sick

typedef struct Screen {
  int width;
  int height;
  int xPadding;
  int yPadding;
  char *buffer;
} Screen;

static inline void initScreen(Screen *screen, int height, int width, char fill) {
  struct winsize ws;
  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)) {
    printf("Error getting terminal size\n");
  }

  assert(width < ws.ws_col);
  assert(height < ws.ws_row);

  screen->width = width;
  screen->height = height;
  screen->xPadding = (ws.ws_col - width) / 2;
  screen->yPadding = (ws.ws_row - height) / 2;
  screen->buffer = (char*)malloc(sizeof(char) * width * height);
  memset(screen->buffer, fill, width * height);
}

static inline void drawScreen(Screen *screen) {
  printf("\033[H\033[2J");
  for(int i = 0; i < screen->height; ++i) {
    printf("\033[%d;%dH", i + screen->yPadding, screen->xPadding);
    printf("%.*s", screen->width, &screen->buffer[i*screen->width]);
  }
  printf("\033[H");
  fflush(stdout);
}

#endif // SCREEN_H
