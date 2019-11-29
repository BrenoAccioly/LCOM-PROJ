#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t x, y;
} button;

void showInitialScreen();

int startMenu();

#endif // MAINMENU_H_INCLUDED
