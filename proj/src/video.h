#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

/** @defgroup video video
 * @{
 *
 * Functions and structures responsible for the graphics.
 */

typedef struct {
  char VbeSignature[4];
  uint16_t VbeVersion;
  uint32_t OemStringPtr;
  uint8_t Capabilities;
  uint32_t VideoModePtr;
  uint16_t TotalMemory;

  uint16_t OemSoftwareRev;
  uint32_t OemVendorNamePtr;
  uint32_t OemProductNamePtr;
  uint32_t OemProductRevPtr;
  uint8_t Reserved;

  uint8_t OemData;
} VbeInfoBlock;

/**
 * @brief Used in the implementation of the double buffer
 * 
 */
void copy_buffer();

/**
 * @brief Change the video mode.
 * 
 * @param mode 
 * @return 0 no errors were detected
 */
int vg_change_mode(uint16_t mode);

int vg_generate_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

/**
 * @brief Draw a pixmap in the specified coordinates with the given xpm.
 * 
 * @param xpm 
 * @param type 
 * @param x 
 * @param y 
 * @return 0 if no errors were detected
 */
int vg_draw_pixmap(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y);

/**
 * @brief Draw image in the given coordinates.
 * 
 * @param image 
 * @param sprite 
 * @param type 
 * @param x 
 * @param y 
 * @return int 
 */
int vg_draw_image(xpm_image_t image, uint8_t *sprite ,enum xpm_image_type type, uint16_t x, uint16_t y);

/**
 * @brief Verifies the collision of the player with a wall.
 * 
 * @param image 
 * @param sprite 
 * @param type 
 * @param x 
 * @param y 
 * @param collision 
 * @return int 
 */
int vg_verify_collision(xpm_image_t image, uint8_t *sprite ,enum xpm_image_type type, uint16_t x, uint16_t y, bool *collision);

/**
 * @brief Verifies if the player is hit by a trap.
 * 
 * @param image 
 * @param sprite 
 * @param type 
 * @param x 
 * @param y 
 * @return int 
 */
int vg_verify_death(xpm_image_t image, uint8_t *sprite ,enum xpm_image_type type, uint16_t x, uint16_t y);

/**
 * @brief Set the pixel at x and y position with the given color.
 * 
 * @param x 
 * @param y 
 * @param color 
 * @return int 
 */
int set_pixel(uint16_t x,uint16_t y, uint32_t color);

/**
 * @brief Sets all pixels in the screen to black
 * 
 */
void vg_clean_screen();

void red_mask(uint32_t red);

#endif // VIDEO_H_INCLUDED
