#include <lcom/lcf.h>
#include "kbd.h"
#include "maze.h"
#include "video.h"
#include "i8042.h"
#include "i8254.h"
#include "pixelmaps.h"

static char *maze;
extern uint32_t counter;
extern uint8_t kbd_out_buffer_byte;
extern uint8_t irq_timer0, irq_kbd;

int disable_interrupts(){
  if(kbd_unsubscribe_int())
    return 1;

  //if(timer_unsubscribe_int())
    //return 1;
  return 0;
}

int enable_interrupts(){
  if(kbd_subscribe_int(&irq_kbd))
    return 1;

  //if(timer_subscribe_int(&irq_timer0))
    //return 1;
  return 0;
}


void draw_timer(int32_t time){
  switch(time/10){
    case 1:
      vg_draw_pixmap(char_1, XPM_8_8_8, 900, 15);
      break;
    case 2:
      vg_draw_pixmap(char_2, XPM_8_8_8, 900, 15);
      break;
    case 3:
      vg_draw_pixmap(char_3, XPM_8_8_8, 900, 15);
      break;
    case 4:
      vg_draw_pixmap(char_4, XPM_8_8_8, 900, 15);
      break;
    case 5:
      vg_draw_pixmap(char_5, XPM_8_8_8, 900, 15);
      break;
    case 6:
      vg_draw_pixmap(char_6, XPM_8_8_8, 900, 15);
      break;
    case 7:
      vg_draw_pixmap(char_7, XPM_8_8_8, 900, 15);
      break;
    case 8:
      vg_draw_pixmap(char_8, XPM_8_8_8, 900, 15);
      break;
    case 9:
      vg_draw_pixmap(char_9, XPM_8_8_8, 900, 15);
      break;
    default:
      vg_draw_pixmap(char_0, XPM_8_8_8, 900, 15);
      break;
  }
  switch(time%10){
    case 1:
      vg_draw_pixmap(char_1, XPM_8_8_8, 910, 15);
      break;
    case 2:
      vg_draw_pixmap(char_2, XPM_8_8_8, 910, 15);
      break;
    case 3:
      vg_draw_pixmap(char_3, XPM_8_8_8, 910, 15);
      break;
    case 4:
      vg_draw_pixmap(char_4, XPM_8_8_8, 910, 15);
      break;
    case 5:
      vg_draw_pixmap(char_5, XPM_8_8_8, 910, 15);
      break;
    case 6:
      vg_draw_pixmap(char_6, XPM_8_8_8, 910, 15);
      break;
    case 7:
      vg_draw_pixmap(char_7, XPM_8_8_8, 910, 15);
      break;
    case 8:
      vg_draw_pixmap(char_8, XPM_8_8_8, 910, 15);
      break;
    case 9:
      vg_draw_pixmap(char_9, XPM_8_8_8, 910, 15);
      break;
    default:
      vg_draw_pixmap(char_0, XPM_8_8_8, 910, 15);
      break;
  }

}

void draw_key_counter(uint8_t keys){
  vg_draw_pixmap(key_counter, XPM_8_8_8, 945, 0);
  switch(keys){
    case 1:
      vg_draw_pixmap(char_1, XPM_8_8_8, 990, 15);
      break;
    case 2:
      vg_draw_pixmap(char_2, XPM_8_8_8, 990, 15);
      break;
    case 3:
      vg_draw_pixmap(char_3, XPM_8_8_8, 990, 15);
      break;
    case 4:
      vg_draw_pixmap(char_4, XPM_8_8_8, 990, 15);
      break;
    case 5:
      vg_draw_pixmap(char_5, XPM_8_8_8, 990, 15);
      break;
    case 6:
      vg_draw_pixmap(char_6, XPM_8_8_8, 990, 15);
      break;
    case 7:
      vg_draw_pixmap(char_7, XPM_8_8_8, 990, 15);
      break;
    default:
      vg_draw_pixmap(char_0, XPM_8_8_8, 990, 15);
      break;
  }
}

void game_result(bool game_win, int32_t time, uint8_t keys){

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  xpm_image_t imageGameResult;
  uint8_t *gameResultSprite = xpm_load(gameResult, XPM_8_8_8, &imageGameResult); 

  xpm_image_t imageGameWin;
  uint8_t *gameWinSprite = xpm_load(gameWin, XPM_8_8_8, &imageGameWin);

  vg_draw_image(imageGameResult, gameResultSprite, XPM_8_8_8, 0, 0);
  draw_key_counter(keys);
  draw_timer(time);
  copy_buffer();
  tickdelay(micros_to_ticks(1000000));
  if(game_win)
      vg_draw_image(imageGameWin, gameWinSprite, XPM_8_8_8, 300, 650);
  copy_buffer();

  tickdelay(micros_to_ticks(1000000));

  kbd_out_buffer_byte = 0x0;
  while(kbd_out_buffer_byte != BREAK_ESC){
      kbc_ih();
      
      kbd_read_scancode(&two_byte, &make, &size, scancode);

      if(two_byte)
          continue; 
      
      tickdelay(micros_to_ticks(DELAY_US));
  } 

}

int runGame(){
  int32_t ipc_status;
  uint32_t r;
  message msg;

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  int32_t aux_counter=0, max_time = 99;

  uint8_t keys = 0;
  uint8_t spike_pos = 0;
  bool game_win = false;

  uint32_t fr_rate = 30;
  int speed = 20;
  int x_move = -4*80, y_move = 5*80 - 45;
  bool collision = false;

  xpm_image_t imagePlayerFront;
  uint8_t *playerFrontSprite = xpm_load(playerFront, XPM_8_8_8, &imagePlayerFront); 

  xpm_image_t imagePlayerFront1;
  uint8_t *playerFront1Sprite = xpm_load(playerFront1, XPM_8_8_8, &imagePlayerFront1); 

  xpm_image_t imagePlayerFront2;
  uint8_t *playerFront2Sprite = xpm_load(playerFront2, XPM_8_8_8, &imagePlayerFront2); 

  xpm_image_t imagePlayerBack;
  uint8_t *playerBackSprite = xpm_load(playerBack, XPM_8_8_8, &imagePlayerBack);

  xpm_image_t imagePlayerBack1;
  uint8_t *playerBack1Sprite = xpm_load(playerBack1, XPM_8_8_8, &imagePlayerBack1);

  xpm_image_t imagePlayerBack2;
  uint8_t *playerBack2Sprite = xpm_load(playerBack2, XPM_8_8_8, &imagePlayerBack2);

  xpm_image_t imagePlayerLeft;
  uint8_t *playerLeftSprite = xpm_load(playerLeft, XPM_8_8_8, &imagePlayerLeft); 

  xpm_image_t imagePlayerLeft1;
  uint8_t *playerLeft1Sprite = xpm_load(playerLeft1, XPM_8_8_8, &imagePlayerLeft1); 

  xpm_image_t imagePlayerLeft2;
  uint8_t *playerLeft2Sprite = xpm_load(playerLeft2, XPM_8_8_8, &imagePlayerLeft2); 

  xpm_image_t imagePlayerRight;
  uint8_t *playerRightSprite = xpm_load(playerRight, XPM_8_8_8, &imagePlayerRight); 

  xpm_image_t imagePlayerRight1;
  uint8_t *playerRight1Sprite = xpm_load(playerRight1, XPM_8_8_8, &imagePlayerRight1);

  xpm_image_t imagePlayerRight2;
  uint8_t *playerRight2Sprite = xpm_load(playerRight2, XPM_8_8_8, &imagePlayerRight2); 

  uint8_t playerOrientation = MAKE_S;
  uint8_t playerStep = 0;

  y_move -= 80;
  initialize_maze();
  maze = (char*)malloc(21 * 19 * sizeof(char));
  generateMaze(maze, 21, 19);
  addKeys(maze, 21, 19);
  addTraps(maze, 21, 19);
  drawMaze(maze, 21, 19, x_move, y_move, spike_pos); 
  vg_draw_image(imagePlayerFront, playerFrontSprite, XPM_8_8_8, 1024/2 - 15, 768/2 - 23);  
  draw_key_counter(keys);
  copy_buffer();

  uint8_t move = 0;
  while(kbd_out_buffer_byte != BREAK_ESC && max_time >= 0){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
    }
    if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 	
                  if (msg.m_notify.interrupts & irq_timer0){
                      timer_int_handler();
                      if(aux_counter % 30 == 0)
                      {
                          max_time--;
                          if(spike_pos == 2)
                            spike_pos = 0;
                          else
                            spike_pos++;
                          
                      }
                      if( 60 / fr_rate == counter){
                          counter = 0;
                          
                          vg_clean_screen();
                          drawMaze(maze, 21, 19, x_move, y_move, spike_pos);
                          switch(playerOrientation){
                            case MAKE_W:   
                              if(playerStep == 0)
                                vg_draw_image(imagePlayerBack, playerBackSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                              
                              else  if(move == 1){
                                  vg_draw_image(imagePlayerBack1, playerBack1Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                    playerStep = 1;
                                    move = 0;
                              }
                              else if(playerStep == 1)
                              {
                                  vg_draw_image(imagePlayerBack2, playerBack2Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                  playerStep = 1;
                                  move = 1;
                              }  
                              break;
                            case MAKE_A:
                              if(playerStep == 0)
                                vg_draw_image(imagePlayerLeft, playerLeftSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                              
                              else  if(move == 1){
                                  vg_draw_image(imagePlayerLeft1, playerLeft1Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                    playerStep = 1;
                                    move = 0;
                              }
                              else if(playerStep == 1)
                              {
                                  vg_draw_image(imagePlayerLeft2, playerLeft2Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                  playerStep = 1;
                                  move = 1;
                              }     
                              break;
                            case MAKE_S:
                              if(playerStep == 0)
                                vg_draw_image(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                              
                              else  if(move == 1){
                                  vg_draw_image(imagePlayerFront1, playerFront1Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                    playerStep = 1;
                                    move = 0;
                              }
                              else if(playerStep == 1)
                              {
                                  vg_draw_image(imagePlayerFront2, playerFront2Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                  playerStep = 1;
                                  move = 1;
                              }
                                 
                              break;
                            case MAKE_D:
                              if(playerStep == 0)
                                vg_draw_image(imagePlayerRight, playerRightSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                              
                              else  if(move == 1){
                                  vg_draw_image(imagePlayerRight1, playerRight1Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                    playerStep = 1;
                                    move = 0;
                              }
                              else if(playerStep == 1)
                              {
                                  vg_draw_image(imagePlayerRight2, playerRight2Sprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);
                                  playerStep = 1;
                                  move = 1;
                              }
                               
                              break;
                            default:
                              break;
                          }
                         // red_mask(0x88);
                          draw_timer(max_time);
                          draw_key_counter(keys);
                          
                          //disable_interrupts();
                          copy_buffer(); 
                          //enable_interrupts();  
                      }
                    aux_counter++;
                  } 
                  if (msg.m_notify.interrupts & KBD_IRQ) { 
                      kbc_ih();
                      kbd_read_scancode(&two_byte, &make, &size, scancode);
                      if(two_byte)
                          continue; 
                      else{
                        uint32_t aux = 0;
                        
                        /*if(playerStep != 0)
                        {
                          if((playerOrientation | 0x80) != kbd_out_buffer_byte)
                            kbd_out_buffer_byte = playerOrientation;
                        } */        

                        switch(kbd_out_buffer_byte){
                          case MAKE_W:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23 - speed, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23  - (speed-aux), &collision);

                            }
                            playerOrientation = MAKE_W;
                            playerStep = 1;
                            y_move += (speed-aux);
                            break;
                          case BREAK_W:
                            playerStep = 0;
                            break;
                          case MAKE_A:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 - speed, 768/2 - 23, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 - (speed-aux), 768/2 - 23, &collision);
                            }
                            playerOrientation = MAKE_A;
                            playerStep = 1;
                            x_move += (speed-aux);
                            break;
                          case BREAK_A:
                            playerStep = 0;
                            break;
                          case MAKE_S:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23 + speed, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23  + (speed-aux), &collision);
                            }
                            playerOrientation = MAKE_S;
                            playerStep = 1;
                            y_move -= (speed-aux);
                            break;
                          case BREAK_S:
                            playerStep = 0;
                            break;
                          case MAKE_D:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 + speed, 768/2 - 23, &collision);
                        
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 + (speed-aux), 768/2 - 23, &collision);
                            }
                            playerOrientation = MAKE_D;
                            playerStep = 1;
                            x_move -= (speed-aux);
                            break;
                          case BREAK_D:
                            playerStep = 0;
                            break;
                          default:
                            break;
                        }
                    
                      }
					        }  
                  break;
              default:
                  break;
          }
      } 
      else {
    } 
    if(maze[-(x_move-1024/2 - 15) / 80 + (- (y_move-768/2 - 23) / 80) * 21] == 3){
      maze[-(x_move-1024/2 - 15) / 80 + (- (y_move-768/2 - 23) / 80) * 21] = 0;
      keys++;
    }

    if(keys == 7)
    {
      game_win = true;
      break;
    }

    if(vg_verify_death(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23) == 0)
      break;

  }

  game_result(game_win, max_time, keys);

  return 0;
}
