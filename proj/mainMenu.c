#include <lcom/lcf.h>
#include "video.h"
#include "mouse.h"
#include "kbd.h"
#include "pixelmaps.h"
#include "i8254.h"
#include "i8042.h"
#include "mainMenu.h"
#include "game.h"
#include "rtc.h"

extern uint8_t mouse_out_buf_byte;
extern uint8_t kbd_out_buffer_byte;
extern uint32_t counter;
uint8_t irq_kbd, irq_timer0;

int cursorOnButton(struct button b, uint32_t cursor_x, uint32_t cursor_y)
{
  if(cursor_x  >= b.x && cursor_x <= b.x + b.width)
  {
    if(cursor_y >= b.y && cursor_y <= b.y + b.height)
      return 0;
  }
  return 1;
}

void showInitialScreen(){
  vg_draw_pixmap(initialScreen, XPM_8_8_8, 0, 0); 
}

void draw_number(uint32_t num, uint32_t x, uint32_t y){
  switch(num/10){
    case 1:
      vg_draw_pixmap(char_1, XPM_8_8_8, x, y);
      break;
    case 2:
      vg_draw_pixmap(char_2, XPM_8_8_8, x, y);
      break;
    case 3:
      vg_draw_pixmap(char_3, XPM_8_8_8, x, y);
      break;
    case 4:
      vg_draw_pixmap(char_4, XPM_8_8_8, x, y);
      break;
    case 5:
      vg_draw_pixmap(char_5, XPM_8_8_8, x, y);
      break;
    case 6:
      vg_draw_pixmap(char_6, XPM_8_8_8, x, y);
      break;
    case 7:
      vg_draw_pixmap(char_7, XPM_8_8_8, x, y);
      break;
    case 8:
      vg_draw_pixmap(char_8, XPM_8_8_8, x, y);
      break;
    case 9:
      vg_draw_pixmap(char_9, XPM_8_8_8, x, y);
      break;
    default:
      vg_draw_pixmap(char_0, XPM_8_8_8, x, y);
      break;
  }
  switch(num%10){
    case 1:
      vg_draw_pixmap(char_1, XPM_8_8_8, x+10, y);
      break;
    case 2:
      vg_draw_pixmap(char_2, XPM_8_8_8, x+10, y);
      break;
    case 3:
      vg_draw_pixmap(char_3, XPM_8_8_8, x+10, y);
      break;
    case 4:
      vg_draw_pixmap(char_4, XPM_8_8_8, x+10, y);
      break;
    case 5:
      vg_draw_pixmap(char_5, XPM_8_8_8, x+10, y);
      break;
    case 6:
      vg_draw_pixmap(char_6, XPM_8_8_8, x+10, y);
      break;
    case 7:
      vg_draw_pixmap(char_7, XPM_8_8_8, x+10, y);
      break;
    case 8:
      vg_draw_pixmap(char_8, XPM_8_8_8, x+10, y);
      break;
    case 9:
      vg_draw_pixmap(char_9, XPM_8_8_8, x+10, y);
      break;
    default:
      vg_draw_pixmap(char_0, XPM_8_8_8, x+10, y);
      break;
  }
}

void draw_time(){
  draw_number(rtc_read_hour(), 840, 15);
  vg_draw_pixmap(char_colon, XPM_8_8_8, 860, 15);
  draw_number(rtc_read_min(), 870, 15);
  vg_draw_pixmap(char_colon, XPM_8_8_8, 890, 15);
  draw_number(rtc_read_sec(), 900, 15);

  draw_number(rtc_read_day(), 940, 15);
  vg_draw_pixmap(char_slash, XPM_8_8_8, 960, 15);
  draw_number(rtc_read_month(), 970, 15);
  vg_draw_pixmap(char_slash, XPM_8_8_8, 990, 15);
  draw_number(rtc_read_year(), 1000, 15);
}

void help_menu(){
  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  vg_draw_pixmap(help_screen, XPM_8_8_8, 0, 0);
  copy_buffer();

  while(kbd_out_buffer_byte != BREAK_ESC){
      kbc_ih();
      
      kbd_read_scancode(&two_byte, &make, &size, scancode);

      if(two_byte)
          continue; 
      
      tickdelay(micros_to_ticks(DELAY_US));
  } 
}

int startMenu(){
  int32_t irq_mouse;
  
  int32_t ipc_status;
  uint32_t r;
  message msg;

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  struct packet pp;

  uint8_t byte_index = 0;
  uint8_t bytes[3];

  xpm_image_t imageScreen;
  uint8_t *screen = xpm_load(initialScreen, XPM_8_8_8, &imageScreen);

  uint32_t cursor_x = 500, cursor_y = 400;

  struct mouse_ev mouse_event;


  /*----buttons-----*/
  xpm_image_t imageStart;
  uint8_t *startSprite = xpm_load(startButton, XPM_8_8_8, &imageStart);

  struct button startButton = {.image = imageStart, .sprite = startSprite, .x = 650 - imageStart.width, .y = 535 - imageStart.height, .width = imageStart.width,  .height = imageStart.height};

  xpm_image_t imageHelp;
  uint8_t *helpSprite = xpm_load(init_help_on, XPM_8_8_8, &imageHelp);

  struct button helpButton = {.image = imageHelp, .sprite = helpSprite, .x = 980 - imageHelp.width, .y = 740 - imageHelp.height, .width = imageHelp.width,  .height = imageHelp.height};


  /*----------------*/

  mouse_write_cmd(EN_DATA_REPORT);

  if(mouse_subscribe_int(&irq_mouse))
    return 1;
  
  if(kbd_subscribe_int(&irq_kbd))
    return 1;

  if(timer_subscribe_int(&irq_timer0))
    return 1;
  

  vg_draw_image(imageScreen, screen, XPM_8_8_8,0, 0);
  vg_draw_pixmap(cursor, XPM_8_8_8, cursor_x, cursor_y);
  copy_buffer();
  while(BREAK_ESC != kbd_out_buffer_byte){

      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){ 
          printf("driver_receive failed with: %d", r);
          continue;
      }

      if (is_ipc_notify(ipc_status)) { 

            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if(msg.m_notify.interrupts & irq_timer0){
                      timer_int_handler();
                      if(60 / 30 == counter)//60 / fr_rate == counter
                      {
                          counter = 0;
                          vg_clean_screen();
                          vg_draw_image(imageScreen, screen, XPM_8_8_8, 0, 0); 
                          vg_draw_pixmap(init_help_off, XPM_8_8_8, helpButton.x, helpButton.y);
                          if(cursorOnButton(startButton, cursor_x, cursor_y) == 0){
                             vg_draw_image(startButton.image, startButton.sprite, XPM_8_8_8, startButton.x, startButton.y); 
                          }
                          else if(cursorOnButton(helpButton, cursor_x, cursor_y) == 0){
                             vg_draw_image(helpButton.image, helpButton.sprite, XPM_8_8_8, helpButton.x, helpButton.y); 
                          }
                          vg_draw_pixmap(cursor, XPM_8_8_8, cursor_x, cursor_y);
                          draw_time();
                          copy_buffer(); 
                      }                  
                    }

                    if(msg.m_notify.interrupts & irq_kbd){
                      kbc_ih();
                      kbd_read_scancode(&two_byte, &make, &size, scancode);

                      if(two_byte)
                          continue; 
                    }
                    if (msg.m_notify.interrupts & irq_mouse){                    
                      mouse_ih();

                      if((mouse_out_buf_byte & BIT_3) && byte_index == 0)
                      {
                        bytes[byte_index] = mouse_out_buf_byte;
                        byte_index++;
                      }
                      else if(byte_index == 1)
                      {                       
                        bytes[byte_index] = mouse_out_buf_byte;
                        byte_index++;
                      }
                      else if(byte_index == 2)
                      {
                        bytes[byte_index] = mouse_out_buf_byte;
                        if(parse_packet(bytes, &pp))
                          return 1;
                        
                        mouse_event = *mouse_detect_event(&pp);
                        switch(mouse_event.type){
                          case LB_PRESSED:
                            if(cursorOnButton(startButton, cursor_x, cursor_y) == 0)
                            {
                              vg_clean_screen();
                              runGame();
                              if(kbd_out_buffer_byte == BREAK_ESC)
                              {
                                kbd_out_buffer_byte = 0x0;
                              }
                            }
                            else if(cursorOnButton(helpButton, cursor_x, cursor_y) == 0){
                              vg_clean_screen();
                              help_menu();
                              if(kbd_out_buffer_byte == BREAK_ESC)
                              {
                                kbd_out_buffer_byte = 0x0;
                              }
                            }
                            break;
                          case LB_RELEASED:
                            
                            break;
                          case MOUSE_MOV:
                              if(cursor_x + mouse_event.delta_x <= 1024 && cursor_y -  mouse_event.delta_y <= 768 && cursor_x + mouse_event.delta_x > 0 && cursor_y -  mouse_event.delta_y > 0){
                                cursor_x += mouse_event.delta_x;
                                cursor_y -= mouse_event.delta_y;
                              }
                              break;
                          default:
                            break;
                        };
                        byte_index = 0;
                      }  
					          }  
                    break;
                default:
                    break;
            }
        }
        else
        {
        } 
  }

  if(mouse_unsubscribe_int())
    return 1;
  
  if(kbd_unsubscribe_int())
    return 1;

  if(timer_unsubscribe_int())
    return 1;
  
  mouse_disable_data_report();

  return 0;
}
