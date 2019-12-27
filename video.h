#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

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


void copy_buffer();

int vg_change_mode(uint16_t mode);

int vg_generate_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

int vg_draw_pixmap(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y);

int vg_draw_image(xpm_image_t image, uint8_t *sprite ,enum xpm_image_type type, uint16_t x, uint16_t y);

int vg_verify_collision(xpm_image_t image, uint8_t *sprite ,enum xpm_image_type type, uint16_t x, uint16_t y, bool *collision);

int vg_verify_death(xpm_image_t image, uint8_t *sprite ,enum xpm_image_type type, uint16_t x, uint16_t y);

int set_pixel(uint16_t x,uint16_t y, uint32_t color);

void vg_clean_screen();

void red_mask(uint32_t red);

#endif // VIDEO_H_INCLUDED
