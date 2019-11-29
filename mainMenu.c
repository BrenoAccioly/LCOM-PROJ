#include <lcom/lcf.h>
#include "video.h"
#include "mouse.h"

#include "pixelmaps.h"

#include "i8042.h"

extern uint8_t mouse_out_buf_byte;

void showInitialScreen(){
  vg_draw_pixmap(initialScreen, XPM_8_8_8, 0, 0); 
}

int startMenu(){
  int32_t irq_mouse;
  int32_t ipc_status;
  uint32_t r;
  message msg;
  struct packet pp;

  uint8_t byte_index = 0;
  uint8_t bytes[3];

  uint32_t cursor_x = 500, cursor_y = 400;

  struct mouse_ev mouse_event;

  mouse_write_cmd(EN_DATA_REPORT);

  if(mouse_subscribe_int(&irq_mouse))
    return 1;

  showInitialScreen();

  while(1){
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0){ 
          printf("driver_receive failed with: %d", r);
          continue;
      }

      if (is_ipc_notify(ipc_status)) { 

            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: 	
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
                            
                            break;
                          case LB_RELEASED:
                            
                            break;
                          case MOUSE_MOV:
                              // verificar limites
                              if(cursor_x + mouse_event.delta_x <= 1024 && cursor_y -  mouse_event.delta_y <= 768 ){
                                cursor_x += mouse_event.delta_x;
                                cursor_y -= mouse_event.delta_y;
                              }
                              else{
                                //cursor_x += 1(mouse_event.delta_x + cursor_x) % (h_res);
                                //cursor_y -= mouse_event.delta_y;
                              }
                              //vg_clean_screen();
                              vg_draw_pixmap(cursor, XPM_8_8_8, cursor_x, cursor_y); 
                              //showInitialScreen();
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
  
  mouse_disable_data_report();

  return 0;
}


