#include <lcom/lcf.h>
#include "video.h"
#include "mouse.h"
#include "kbd.h"
#include "pixelmaps.h"
#include "char.h"
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

  int button_pressed = 0;

  xpm_image_t imageHelp;
  uint8_t *helpSprite = xpm_load(help_screen, XPM_8_8_8, &imageHelp);

  xpm_image_t imageW;
  uint8_t *WSprite = xpm_load(help_w, XPM_8_8_8, &imageW);

  xpm_image_t imageA;
  uint8_t *ASprite = xpm_load(help_a, XPM_8_8_8, &imageA);

  xpm_image_t imageS;
  uint8_t *SSprite = xpm_load(help_s, XPM_8_8_8, &imageS);

  xpm_image_t imageD;
  uint8_t *DSprite = xpm_load(help_d, XPM_8_8_8, &imageD);


  while(kbd_out_buffer_byte != BREAK_ESC){
      kbc_ih();
      
      kbd_read_scancode(&two_byte, &make, &size, scancode);

      if(two_byte)
          continue;
      if(kbd_out_buffer_byte == 0x0)
        continue;
      
      if(button_pressed != 0 && kbd_out_buffer_byte != MAKE_W &&kbd_out_buffer_byte != MAKE_A && kbd_out_buffer_byte != MAKE_S && kbd_out_buffer_byte != MAKE_D)
      {
        if(make)
          kbd_out_buffer_byte = button_pressed;
        
      }

      vg_clean_screen();
      vg_draw_image(imageHelp, helpSprite, XPM_8_8_8, 0, 0); 
      switch(kbd_out_buffer_byte){
        case MAKE_W:
          vg_draw_image(imageA, ASprite, XPM_8_8_8, 57, 381);
          vg_draw_image(imageS, SSprite, XPM_8_8_8, 177, 381);
          vg_draw_image(imageD, DSprite, XPM_8_8_8, 298, 381);

          button_pressed = MAKE_W; break;
        case MAKE_A:
          vg_draw_image(imageW, WSprite, XPM_8_8_8, 177, 248);
          vg_draw_image(imageS, SSprite, XPM_8_8_8, 177, 381);
          vg_draw_image(imageD, DSprite, XPM_8_8_8, 298, 381);
          button_pressed = MAKE_A; break;
        case MAKE_S:
          vg_draw_image(imageW, WSprite, XPM_8_8_8, 177, 248);
          vg_draw_image(imageA, ASprite, XPM_8_8_8, 57, 381);
          vg_draw_image(imageD, DSprite, XPM_8_8_8, 298, 381);
          button_pressed = MAKE_S; break;
        case MAKE_D:
          vg_draw_image(imageW, WSprite, XPM_8_8_8, 177, 248);
          vg_draw_image(imageA, ASprite, XPM_8_8_8, 57, 381);
          vg_draw_image(imageS, SSprite, XPM_8_8_8, 177, 381);
          button_pressed = MAKE_D; break;
        default:
          vg_draw_image(imageW, WSprite, XPM_8_8_8, 177, 248);
          vg_draw_image(imageA, ASprite, XPM_8_8_8, 57, 381);
          vg_draw_image(imageS, SSprite, XPM_8_8_8, 177, 381);
          vg_draw_image(imageD, DSprite, XPM_8_8_8, 298, 381);
          button_pressed = 0; break;
      }
      copy_buffer();
      tickdelay(micros_to_ticks(DELAY_US));

      
  } 
}

void leaderboard_menu(){

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  xpm_image_t imageA;
  uint8_t *aSprite = xpm_load(charb_a, XPM_8_8_8, &imageA);
  xpm_image_t imageB;
  uint8_t *bSprite = xpm_load(charb_b, XPM_8_8_8, &imageB);
  xpm_image_t imageC;
  uint8_t *cSprite = xpm_load(charb_c, XPM_8_8_8, &imageC);
  xpm_image_t imageD;
  uint8_t *dSprite = xpm_load(charb_d, XPM_8_8_8, &imageD);
  xpm_image_t imageE;
  uint8_t *eSprite = xpm_load(charb_e, XPM_8_8_8, &imageE);
  xpm_image_t imageF;
  uint8_t *fSprite = xpm_load(charb_f, XPM_8_8_8, &imageF);
  xpm_image_t imageG;
  uint8_t *gSprite = xpm_load(charb_g, XPM_8_8_8, &imageG);
  xpm_image_t imageH;
  uint8_t *hSprite = xpm_load(charb_h, XPM_8_8_8, &imageH);
  xpm_image_t imageI;
  uint8_t *iSprite = xpm_load(charb_i, XPM_8_8_8, &imageI);
  xpm_image_t imageJ;
  uint8_t *jSprite = xpm_load(charb_j, XPM_8_8_8, &imageJ);
  xpm_image_t imageK;
  uint8_t *kSprite = xpm_load(charb_k, XPM_8_8_8, &imageK);
  xpm_image_t imageL;
  uint8_t *LSprite = xpm_load(charb_l, XPM_8_8_8, &imageL);
  xpm_image_t imageM;
  uint8_t *mSprite = xpm_load(charb_m, XPM_8_8_8, &imageM);
  xpm_image_t imageN;
  uint8_t *nSprite = xpm_load(charb_n, XPM_8_8_8, &imageN);
  xpm_image_t imageO;
  uint8_t *oSprite = xpm_load(charb_o, XPM_8_8_8, &imageO);
  xpm_image_t imageP;
  uint8_t *pSprite = xpm_load(charb_p, XPM_8_8_8, &imageP);
  xpm_image_t imageQ;
  uint8_t *qSprite = xpm_load(charb_q, XPM_8_8_8, &imageQ);
  xpm_image_t imageR;
  uint8_t *rSprite = xpm_load(charb_r, XPM_8_8_8, &imageR);
  xpm_image_t imageS;
  uint8_t *sSprite = xpm_load(charb_s, XPM_8_8_8, &imageS);
  xpm_image_t imageT;
  uint8_t *tSprite = xpm_load(charb_t, XPM_8_8_8, &imageT);
  xpm_image_t imageU;
  uint8_t *uSprite = xpm_load(charb_u, XPM_8_8_8, &imageU);
  xpm_image_t imageV;
  uint8_t *vSprite = xpm_load(charb_v, XPM_8_8_8, &imageV);
  xpm_image_t imageW;
  uint8_t *wSprite = xpm_load(charb_w, XPM_8_8_8, &imageW);
  xpm_image_t imageX;
  uint8_t *xSprite = xpm_load(charb_x, XPM_8_8_8, &imageX);
  xpm_image_t imageY;
  uint8_t *ySprite = xpm_load(charb_y, XPM_8_8_8, &imageY);
  xpm_image_t imageZ;
  uint8_t *zSprite = xpm_load(charb_z, XPM_8_8_8, &imageZ);

  uint8_t c;
  FILE *fptr;
  fptr = fopen("/home/lcom/labs/proj/leaderboard.txt","r");
  vg_draw_pixmap(leaderboard_screen,XPM_8_8_8,0,0);
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 8; j++){
        c = fgetc(fptr);
        if(j > 5)
          j += 4;
        
        switch(c)
        {
          case 'a':
            vg_draw_image(imageA, aSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'b':
            vg_draw_image(imageB, bSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'c':
            vg_draw_image(imageC, cSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'd':
            vg_draw_image(imageD, dSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'e':
            vg_draw_image(imageE, eSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'f':
            vg_draw_image(imageF, fSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'g':
            vg_draw_image(imageG, gSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'h':
            vg_draw_image(imageH, hSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'i':
            vg_draw_image(imageI, iSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'j':
            vg_draw_image(imageJ, jSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'k':
            vg_draw_image(imageK, kSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'l':
            vg_draw_image(imageL, LSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'm':
            vg_draw_image(imageM, mSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'n':
            vg_draw_image(imageN, nSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'o':
            vg_draw_image(imageO, oSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'p':
            vg_draw_image(imageP, pSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'q':
            vg_draw_image(imageQ, qSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'r':
            vg_draw_image(imageR, rSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 's':
            vg_draw_image(imageS, sSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 't':
            vg_draw_image(imageT, tSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'u':
            vg_draw_image(imageU, uSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'v':
            vg_draw_image(imageV, vSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'w':
            vg_draw_image(imageW, wSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'x':
            vg_draw_image(imageX, xSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'y':
            vg_draw_image(imageY, ySprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case 'z':
            vg_draw_image(imageZ, zSprite, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '1':
            vg_draw_pixmap(charB_1, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '2':
            vg_draw_pixmap(charB_2, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '3':
            vg_draw_pixmap(charB_3, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '4':
            vg_draw_pixmap(charB_4, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '5':
            vg_draw_pixmap(charB_5, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '6':
            vg_draw_pixmap(charB_6, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '7':
            vg_draw_pixmap(charB_7, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '8':
            vg_draw_pixmap(charB_8, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '9':
            vg_draw_pixmap(charB_9, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          case '0':
            vg_draw_pixmap(charB_0, XPM_8_8_8, 350 + j*30, 290 + i * 62);
            break;
          default:
            break;
        }
      if(j > 5)
          j -= 4;
      
    }
    c = fgetc(fptr);//c = fgetc(fptr);
  }
  fclose(fptr);
  copy_buffer();
  while(kbd_out_buffer_byte != BREAK_ESC){
      kbc_ih();
      
      kbd_read_scancode(&two_byte, &make, &size, scancode);

      if(two_byte)
          continue;
      if(kbd_out_buffer_byte == 0x0)
        continue;
      vg_clean_screen();
      
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

  xpm_image_t imageLeaderboard;
  uint8_t *leaderboardSprite = xpm_load(leaderboard_button, XPM_8_8_8, &imageLeaderboard);

  struct button leaderboardButton = {.image = imageLeaderboard, .sprite = leaderboardSprite, .x = 643 - imageLeaderboard.width, .y = 580 - imageLeaderboard.height, .width = imageLeaderboard.width,  .height = imageLeaderboard.height};

  xpm_image_t imageExit;
  uint8_t *exitSprite = xpm_load(exit_button, XPM_8_8_8, &imageExit);

  struct button exitButton = {.image = imageExit, .sprite = exitSprite, .x = 560 - imageExit.width, .y = 635 - imageExit.height, .width = imageExit.width,  .height = imageExit.height};

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
                          else if(cursorOnButton(leaderboardButton, cursor_x, cursor_y) == 0){
                             vg_draw_image(leaderboardButton.image, leaderboardButton.sprite, XPM_8_8_8, leaderboardButton.x, leaderboardButton.y); 
                          }
                          else if(cursorOnButton(exitButton, cursor_x, cursor_y) == 0){
                             vg_draw_image(exitButton.image, exitButton.sprite, XPM_8_8_8, exitButton.x, exitButton.y); 
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
                            else if(cursorOnButton(leaderboardButton, cursor_x, cursor_y) == 0){
                             leaderboard_menu();
                              if(kbd_out_buffer_byte == BREAK_ESC)
                              {
                                kbd_out_buffer_byte = 0x0;
                              }
                            }
                            else if(cursorOnButton(exitButton, cursor_x, cursor_y) == 0)
                            {
                              kbd_out_buffer_byte = BREAK_ESC;
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
