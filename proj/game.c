#include <lcom/lcf.h>
#include "kbd.h"
#include "maze.h"
#include "i8042.h"

static char *maze;
extern uint8_t kbd_out_buffer_byte;

int runGame(){
  
  uint8_t irq_kbd;
  int32_t ipc_status;
  uint32_t r;
  message msg;

  uint8_t size = 1;
  uint8_t scancode[2];
  bool make = 0, two_byte = 0;


  
  if(kbd_subscribe_int(&irq_kbd))
    return 1;
  
  
  maze = (char*)malloc(12 * 9 * sizeof(char));
  generateMaze(maze, 12, 9);
  addKeys(maze, 12, 9);
  drawMaze(maze, 12, 9);

  while(kbd_out_buffer_byte != BREAK_ESC){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
          printf("driver_receive failed with: %d", r);
          continue;
    }
    if (is_ipc_notify(ipc_status)) { 
          switch (_ENDPOINT_P(msg.m_source)) {
              case HARDWARE: 	
                  if (msg.m_notify.interrupts & irq_kbd) { 
                      kbc_ih();
                      kbd_read_scancode(&two_byte, &make, &size, scancode);

                      if(two_byte)
                          continue; 
					            }  
                  break;
              default:
                  break;
          }
      } 
      else {
    } 
  }

  if(kbd_unsubscribe_int())
    return 1;

  return 0;
}
