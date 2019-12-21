#include <lcom/lcf.h>
#include "kbd.h"
#include "maze.h"
#include "video.h"
#include "i8042.h"
#include "i8254.h"

static char *maze;
extern uint32_t counter;
extern uint8_t kbd_out_buffer_byte;
extern uint8_t irq_timer0;

int runGame(){
  //uint8_t irq_timer = TIMER0_IRQ ;
  int32_t ipc_status;
  uint32_t r;
  message msg;

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;

  uint32_t fr_rate = 30;
  int speed = 10;
  int x_move = 0, y_move = 0;
  
  initialize_maze();
  maze = (char*)malloc(21 * 19 * sizeof(char));
  generateMaze(maze, 21, 19);
  addKeys(maze, 21, 19);
  drawMaze(maze, 21, 19, x_move, -y_move); copy_buffer();


  while(kbd_out_buffer_byte != BREAK_ESC){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
    }
    if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 	
                  if (msg.m_notify.interrupts & irq_timer0){
                      timer_int_handler();
                      if( 60 / fr_rate == counter){
                          counter = 0;
                          vg_clean_screen();
                          drawMaze(maze, 21, 19, x_move, -y_move); 
                          copy_buffer();   
                      }

                  } 
                  if (msg.m_notify.interrupts & KBD_IRQ) { 
                      kbc_ih();
                      kbd_read_scancode(&two_byte, &make, &size, scancode);
                      if(two_byte)
                          continue; 
                      else{
                        switch(kbd_out_buffer_byte){
                          case MAKE_W:
                            y_move += speed;
                            break;
                          case MAKE_A:
                            x_move -= speed;
                            break;
                          case MAKE_S:
                            y_move -= speed;
                            break;
                          case MAKE_D:
                            x_move += speed;
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
  }

  return 0;
}
