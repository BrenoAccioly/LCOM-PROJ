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

int vg_change_mode(uint16_t mode);

int vg_generate_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step);

int vg_draw_pixmap(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y);

int set_pixel(uint16_t x,uint16_t y, uint32_t color);

void vg_clean_screen();

int vg_get_vbe_controller_info(vg_vbe_contr_info_t *info_p);

#endif // VIDEO_H_INCLUDED
