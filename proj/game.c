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
extern uint8_t irq_timer0;

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

int runGame(){
  int32_t ipc_status;
  uint32_t r;
  message msg;

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  uint32_t aux_counter=0, elapsed_time = 0, max_time = 60;

  uint8_t keys = 0;
  bool game_win = false;

  uint32_t fr_rate = 30;
  int speed = 20;
  int x_move = -4*80, y_move = 5*80 - 45;
  bool collision = false;

  xpm_image_t imagePlayerFront;
  uint8_t *playerFrontSprite = xpm_load(playerFront, XPM_8_8_8, &imagePlayerFront); 

  xpm_image_t imagePlayerBack;
  uint8_t *playerBackSprite = xpm_load(playerBack, XPM_8_8_8, &imagePlayerBack); 

  xpm_image_t imagePlayerLeft;
  uint8_t *playerLeftSprite = xpm_load(playerLeft, XPM_8_8_8, &imagePlayerLeft); 

  xpm_image_t imagePlayerRight;
  uint8_t *playerRightSprite = xpm_load(playerRight, XPM_8_8_8, &imagePlayerRight);  

  uint8_t playerOrientation = MAKE_S;

  y_move -= 80;
  initialize_maze();
  maze = (char*)malloc(21 * 19 * sizeof(char));
  generateMaze(maze, 21, 19);
  addKeys(maze, 21, 19);
  drawMaze(maze, 21, 19, x_move, y_move); 
  vg_draw_image(imagePlayerFront, playerFrontSprite, XPM_8_8_8, 1024/2 - 15, 768/2 - 23);  
  draw_key_counter(keys);
  copy_buffer();

  while(kbd_out_buffer_byte != BREAK_ESC && elapsed_time <= max_time){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
    }
    if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 	
                  if (msg.m_notify.interrupts & irq_timer0){
                      timer_int_handler();
                      if(aux_counter % 60 == 0)
                      {
                          elapsed_time++;
                      }
                      if( 60 / fr_rate == counter){
                          counter = 0;
                          vg_clean_screen();
                          drawMaze(maze, 21, 19, x_move, y_move);
                          switch(playerOrientation){
                            case MAKE_W:
                              vg_draw_image(imagePlayerBack, playerBackSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);   
                              break;
                            case MAKE_A:
                              vg_draw_image(imagePlayerLeft, playerLeftSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);   
                              break;
                            case MAKE_S:
                              vg_draw_image(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23);   
                              break;
                            case MAKE_D:
                              vg_draw_image(imagePlayerRight, playerRightSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23); 
                              break;
                            default:
                              break;
                          }
                          draw_key_counter(keys);
                          copy_buffer();   
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
                        switch(kbd_out_buffer_byte){
                          case MAKE_W:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23 - speed, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23  - (speed-aux), &collision);

                            }
                            playerOrientation = MAKE_W;
                            y_move += (speed-aux);
                            break;
                          case MAKE_A:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 - speed, 768/2 - 23, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 - (speed-aux), 768/2 - 23, &collision);
                            }
                            playerOrientation = MAKE_A;
                            x_move += (speed-aux);
                            break;
                          case MAKE_S:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23 + speed, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15, 768/2 - 23  + (speed-aux), &collision);
                            }
                            playerOrientation = MAKE_S;
                            y_move -= (speed-aux);
                            break;
                          case MAKE_D:
                            vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 + speed, 768/2 - 23, &collision);
                            while(collision){
                              aux++;
                              collision = false;
                              vg_verify_collision(imagePlayerFront, playerFrontSprite, XPM_8_8_8,1024/2 - 15 + (speed-aux), 768/2 - 23, &collision);
                            }
                            playerOrientation = MAKE_D;
                            x_move -= (speed-aux);
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

  }

  if(game_win)
  {

  }
  else{

  }

  return 0;
}
