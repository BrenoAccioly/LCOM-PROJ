#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

/** @defgroup MainMenu MainMenu
 * @{
 *
 * Functions and structures mainly used for the
 main menu.
 */
 
/**
 * @brief Used to store the elements of a button.
 * 
 */
struct button{
  xpm_image_t image;
  uint8_t *sprite;
  uint32_t width;
  uint32_t height;
  uint32_t x, y;
};


/**
 * @brief Verifies if the mouse cursor is on the given button.
 * 
 * @param b Button
 * @param cursor_x cursor horizontal position
 * @param cursor_y cursor vertical position
 * @return 0 if cursor is on button limits
 */
int cursorOnButton(struct button b, uint32_t cursor_x, uint32_t cursor_y);

/**
 * @brief Draw the given number in the specified coordinates.
 * Coordinates are in pixels.
 * @param num number
 * @param x horizontal coordinate
 * @param y vertical coordinates
 */
void draw_number(uint32_t num, uint32_t x, uint32_t y);

/**
 * @brief Presents the date and time of the system 
 using the RTC functions.
 * 
 */
void draw_time();

/**
 * @brief Initializes the help menu.
 * 
 */
void help_menu();

/**
 * @brief Initializes the leaderboard menu.
 * 
 */
void leaderboard_menu();

/**
 * @brief Show the graphics of the initial menu.
 * 
 */
void showInitialScreen();

/**
 * @brief Initializes the main menu,
 here is possible to interact with different buttons
 and features.
 * 
 * @return 0 if no errors were detected
 */
int startMenu();

#endif // MAINMENU_H_INCLUDED
