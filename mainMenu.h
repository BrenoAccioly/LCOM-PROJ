#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

struct button{
  xpm_image_t image;
  uint8_t *sprite;
  uint32_t width;
  uint32_t height;
  uint32_t x, y;
};

void showInitialScreen();

void show_date_time();

int startMenu();

#endif // MAINMENU_H_INCLUDED
