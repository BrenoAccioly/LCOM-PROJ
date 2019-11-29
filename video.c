#include <lcom/lcf.h>
#include <machine/int86.h>
#include "video.h"

static uint16_t h_res;  /* Horizontal resolution in pixels */
static uint16_t v_res; /* Vertical resolution in pixels */
static uint8_t bits_per_pixel; /* Number of VRAM bits per pixel */

static uint8_t memory_model;
static uint8_t red_mask_size;
static uint8_t green_mask_size;
static uint8_t blue_mask_size;
static uint8_t rsvd_mask_size;

static uint8_t red_field_pos;
static uint8_t green_field_pos;
static uint8_t blue_field_pos;
static uint8_t rsvd_field_pos;



static uint8_t *video_mem; /* Process (virtual) address to which VRAM is mapped */

static unsigned int vram_base; /* VRAM’s physical addresss */
static unsigned int vram_size; /* VRAM’s size, but you can use the frame-buffer size, instead */

int vg_call_vbe_function(uint8_t function){
  reg86_t reg86;
  reg86.ah = 0x4F;
  reg86.al = function;
  
  reg86.intno = 0x10;
  if(sys_int86(&reg86) == EFAULT){
    printf("set_vbe_mode(): sys_int86() failed \n");
    return 1;
  }
  if(reg86.ax != 0x0)
    return 1;
  return 0;
}

int vg_change_mode(uint16_t mode){
  
  reg86_t reg86;
  reg86.ah = 0x4F;
  reg86.al  = 0x02;
  reg86.bx = mode | 1<<14;
  reg86.intno = 0x10;  

   /* Make the BIOS call */
  if(sys_int86(&reg86) == EFAULT){
    printf("set_vbe_mode(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}

void  *(vg_init)(uint16_t mode){

  reg86_t reg86;
  vbe_mode_info_t vmi_p;
  
  vbe_get_mode_info(mode, &vmi_p);

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;
  bits_per_pixel = vmi_p.BitsPerPixel;

  red_mask_size = vmi_p.RedMaskSize;
  green_mask_size = vmi_p.GreenMaskSize;
  blue_mask_size = vmi_p.BlueMaskSize;
  rsvd_mask_size = vmi_p.RsvdMaskSize;
  memory_model = vmi_p.MemoryModel;


  red_field_pos = vmi_p.RedFieldPosition;
  green_field_pos = vmi_p.GreenFieldPosition;
  blue_field_pos = vmi_p.BlueFieldPosition;
  rsvd_field_pos = vmi_p.RsvdFieldPosition;
  memory_model = vmi_p.MemoryModel;

  int r;
  struct minix_mem_range mr; /* physical memory range */

  // initialize vram_base/size

  vram_base = vmi_p.PhysBasePtr;
  vram_size = v_res * h_res * (bits_per_pixel / 8);

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

  if(video_mem == MAP_FAILED)
    panic("couldn’t map video memory");

    memset(&reg86, 0, sizeof(reg86));


  reg86.ax = 0x4F02;
  reg86.bx = mode | 1<<14;
  reg86.intno = 0x10;  

   /* Make the BIOS call */
  if(sys_int86(&reg86) == EFAULT){
    printf("set_vbe_mode(): sys_int86() failed \n");
    return NULL;
  }


  return video_mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

  uint8_t *aux_pointer = video_mem;
  aux_pointer += (h_res * y + x)*(bits_per_pixel / 8);
  for(uint32_t  i = 0; i < len; i ++){
    
    if(memory_model == 0x04){
      *(aux_pointer)++ = color;
    }
    else
    {
      *(aux_pointer)++ = color & 0xFF;
      *(aux_pointer)++ = (color >> 8) & 0xFF;
      *(aux_pointer)++ = (color >> 16) & 0xFF;
    }
  }

  return 0;
}

int (vg_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width,uint16_t height, uint32_t color){

  // fazer verificacao de limites depois
  for(uint32_t i = 0; i < height; i++){
    vg_draw_hline(x, y + i, width, color);
  }

  return 0;
}

int vg_generate_pattern(uint8_t no_rectangles, uint32_t first, uint8_t step){
  uint16_t height = v_res/no_rectangles;
  uint16_t width  = h_res/no_rectangles;
  uint32_t color = first;

  for(uint32_t row = 0; row < no_rectangles; row++){
    for(uint32_t col = 0; col < no_rectangles; col++){

        // packed/indexed pixel memory_model = 0x04 
        if(memory_model == 0x04){
          color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel) ;
        }
        // direct color memory_model = 0x06
        else{
          uint8_t red, green, blue;

          red   = (((first >> red_field_pos) & (BIT(red_mask_size) - 1)) + col * step) % (1 << red_mask_size);

          green = (((first >> green_field_pos) & (BIT(green_mask_size) - 1))  + row * step) % (1 << green_mask_size);

          blue =  (((first >> blue_field_pos) & (BIT(blue_mask_size) - 1))  + (col + row) * step) % (1 << blue_mask_size);

          color = red << red_field_pos | green << green_field_pos | blue << blue_field_pos;
        }
        vg_draw_rectangle(col*width, row*height, width, height, color);
    }
  }
  
  return 0;
}

int set_pixel(uint16_t x,uint16_t y, uint32_t color){
  
  if(x > h_res || y > v_res || x < 0 || y < 0)
    return 1;

  uint8_t *aux_pointer = video_mem;
  aux_pointer += (h_res * y + x)*(bits_per_pixel / 8);
  if(memory_model == 0x04)
    *aux_pointer = color & 0xFF;
  else{
      *(aux_pointer)++ = color & 0xFF;
      *(aux_pointer)++ = (color >> 8) & 0xFF;
      *(aux_pointer)++ = (color >> 16) & 0xFF;
  }

  return 0;
}

void vg_clean_screen(){
  uint8_t *aux_pointer = video_mem;

  for(int i=0; i < h_res * v_res * bits_per_pixel / 8; i++){
      *(aux_pointer)++ = 0x0;
  }
}


int vg_draw_pixmap(xpm_map_t xpm, enum xpm_image_type type, uint16_t x, uint16_t y){
  
  uint32_t index = 0;
  xpm_image_t image;
  uint8_t *sprite = xpm_load(xpm, type, &image);
  
  if(sprite == NULL){
    panic("Failed to load xpm...");
    return 1;
  }

  for(uint32_t j = y; j < y + image.height; j++){
    for(uint32_t i = x; i < x + image.width; i++)
    {
      if(memory_model == 0x04)
        set_pixel(i, j, sprite[index]);
      else{
        uint32_t color = sprite[index] << red_field_pos; index++;
        color |= sprite[index] << green_field_pos; index++;
        color |= sprite[index] <<  blue_field_pos;
        set_pixel(i, j, color);
      }
      index++;
    }
  }
  
  return 0;
}

int vg_get_vbe_controller_info(vg_vbe_contr_info_t *info_p){

  /*mmap_t map_info;
  VbeInfoBlock info_block;
	if(lm_alloc(sizeof(info_block), &map_info) == NULL)    return 1;

  info_block.VbeSignature[0] = 'V';
  info_block.VbeSignature[1] = 'B';
  info_block.VbeSignature[2] = 'E';
  info_block.VbeSignature[3] = '2';

  reg86_t reg86;
  reg86.ah = 0x4F;
  reg86.al = 0x0;
  reg86.es = (map_info.phys >> 4) & 0xF000;
  reg86.di = map_info.phys & 0xFFFF;
  reg86.intno = 0x10;
  if(sys_int86(&reg86) == EFAULT){
    printf("set_vbe_mode(): sys_int86() failed \n");
    return 1;
  }
  if(reg86.ax != 0x0)
    return 1; 
  
  *info_block = *(VbeInfoBlock*)map_info.virt;
  info_p->VBESignature = info_block.VbeSignature;

  lm_free(&map_info);
  */
  return 0;
  
}
