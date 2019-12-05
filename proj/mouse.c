#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"


int32_t mouse_hook_id = MOUSE_IRQ;
uint8_t mouse_out_buf_byte = 0;
uint8_t mouse_st_byte;
bool rb_pressed = 0, lb_pressed = 0;
struct mouse_ev *ev = NULL;
struct mouse_state mouse_states;


int mouse_subscribe_int(int32_t *bit_no){
  *bit_no = BIT(mouse_hook_id);
  if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, & mouse_hook_id) != OK) 
    return 1;
  return 0;
}

int mouse_unsubscribe_int(){
  if (sys_irqdisable(& mouse_hook_id) != OK) {
		return 1;
	}
  if(sys_irqrmpolicy(& mouse_hook_id) != OK)
    return 1;
  return 0;
}


int mouse_enable_data_report(){
  if(mouse_get_status(& mouse_st_byte))
      return 1;

  if(sys_outb(KBC_CMD_REG, WRITE_TO_MOUSE) == OK){
    if(sys_outb(MOUSE_OUT_BUFFER, EN_DATA_REPORT)== OK)
      return 0;
  }

  return 1;
}

int mouse_disable_data_report(){
  if(mouse_get_status(& mouse_st_byte))
      return 1;

  if(mouse_st_byte & FULL_BUFFER){
    mouse_read_out_buffer(&mouse_out_buf_byte);
  }

  return mouse_write_cmd(DIS_DATA_REPORT);
}

int mouse_get_status(uint8_t *st){
  return util_sys_inb(MOUSE_STATUS_REG, st);
}

int mouse_read_out_buffer(uint8_t *out_buf){
  return util_sys_inb(MOUSE_OUT_BUFFER, out_buf);
}

int mouse_write_cmd(uint32_t cmd){
  /*The acknowledgment byte for each byte written as argument of
command 0xD4 is put in the KBC’s OUT_BUF and should be
read via port 0x60*/
  do{
    if(mouse_get_status(& mouse_st_byte))
      return 1;
    
    if(mouse_st_byte & FULL_BUFFER)
      continue;
    else
    {
      if(sys_outb(KBC_CMD_REG, WRITE_TO_MOUSE) != OK)
        return 1;
    }
    
    if(mouse_get_status(& mouse_st_byte))
      return 1;
    
    if(mouse_st_byte & FULL_BUFFER)
      continue;
    else
    {
      if(sys_outb(MOUSE_OUT_BUFFER, cmd) != OK)
        return 1;
    }

    mouse_read_out_buffer(&mouse_out_buf_byte);

  } while(ACK != mouse_out_buf_byte);

  return 0;
}

int parse_packet(uint8_t bytes[3], struct packet *pp){
  
  pp->bytes[0] = bytes[0];
  pp->bytes[1] = bytes[1];
  pp->bytes[2] = bytes[2];
  
  if(bytes[0] & 0x01) pp->lb = true;
  else pp->lb = false;

  if(bytes[0] & 0x02) pp->rb = true;
  else pp->rb = false;

  if(bytes[0] & 0x04) pp->mb = true;
  else pp->mb = false;

  if(bytes[0] & 0x40) pp->x_ov = true;
  else pp->x_ov = false;
  
  if(bytes[0] & 0x80) pp->y_ov = true;
  else pp->y_ov = false;

  if(bytes[0] & 0x10)
    pp->delta_x = (bytes[1]-256) & 0xFFFF;
  else
    pp->delta_x = bytes[1] & 0xFFFF;
  if(bytes[0] & 0x20)
    pp->delta_y = (bytes[2]-256) & 0xFFFF;
  else
    pp->delta_y = bytes[2] & 0xFFFF;
  return 0;
}

void (mouse_ih)() {

  if(mouse_get_status(& mouse_st_byte))
  {
    mouse_out_buf_byte = 0x0; return;
  } 

  if( (mouse_st_byte & PARITY_ERROR))
  {
    printf("IH Failed: Parity Error \n");
    mouse_out_buf_byte = 0x0;
    return;
  }

  if( (mouse_st_byte & TIME_OUT_ERROR))
  {
    printf("IH Failed: Time Out Error \n");
    mouse_out_buf_byte = 0x0;
    return;
  }

  if(mouse_st_byte & FULL_BUFFER){
    if(mouse_read_out_buffer(&  mouse_out_buf_byte) == 0)
      return;
    mouse_out_buf_byte = 0x0;
  }
  else
    mouse_out_buf_byte = 0x0;

}

void mouse_remote_operation(uint8_t bytes[3]){

  mouse_write_cmd(READ_DATA);
  if(mouse_get_status(& mouse_st_byte))
    return;

  for(int i = 0; i < 3; i++){
    if(mouse_st_byte & FULL_BUFFER){  
        if(mouse_read_out_buffer(& mouse_out_buf_byte) == 0){
          if(i == 0){
            if(mouse_out_buf_byte & BIT_3)
              bytes[i] = mouse_out_buf_byte;
          }
          else
            bytes[i] = mouse_out_buf_byte;
        }
    }
  }
}

struct mouse_ev mouse_event_detect	(struct packet * pp){
  struct mouse_ev ev;

  if(lb_pressed && pp->lb == 0){
    ev.type = LB_RELEASED;
    lb_pressed = false;
  }
  else if(rb_pressed && pp->rb == 0){
    ev.type = RB_RELEASED;
    rb_pressed = false;
  }
  else if(pp->lb == 1 && pp->mb == 0 && pp->rb == 0){
    ev.type = LB_PRESSED;
    lb_pressed = true;
  }
  else if(pp->lb == 0 && pp->mb == 0 && pp->rb == 1){
    ev.type = RB_PRESSED;
    rb_pressed = true;
  }
  else if(pp->delta_x || pp->delta_y)
  {
    ev.type = MOUSE_MOV;
    ev.delta_x = pp->delta_x;
    ev.delta_y = pp->delta_y;
  }
  else
  {
    ev.type = BUTTON_EV;
  }

  return ev;
}

void mouse_update_state(struct mouse_ev mouse_event, uint8_t x_len, uint8_t tolerance){

  if(mouse_states.initial == 1)
  {
    switch (mouse_event.type)
    {
    case LB_PRESSED:
      mouse_states.initial = 0;
      mouse_states.drawing_left = 1;
      mouse_states.x = 0;
      mouse_states.y = 0;
      break;

    default:
      break;
    }
  }
  else if(mouse_states.drawing_left == 1){
    switch (mouse_event.type)
    {
    case LB_RELEASED:
      mouse_states.initial = 1;
      mouse_states.drawing_left = 0;
      break;
    case MOUSE_MOV:
      mouse_states.x += mouse_event.delta_x;
      mouse_states.y += mouse_event.delta_y;
      
      if(mouse_states.y > mouse_states.x + tolerance || mouse_states.y < mouse_states.x - tolerance){
        printf("Tolerance exceeded\n");
        mouse_states.initial = 1;
        mouse_states.drawing_left = 0;
      }
      

      if(mouse_states.x >= x_len){
        mouse_states.drawing_left = 0;
        mouse_states.transition = 1;
      }
    
      break;  
    default:
      mouse_states.initial = 1;
      mouse_states.drawing_left = 0;
      break;
    }
  }
  else if(mouse_states.transition == 1){
    switch (mouse_event.type)
    {
    case 2:
      mouse_states.transition = 0; 
      mouse_states.drawing_right = 1;
      mouse_states.x = 0;
      mouse_states.y = 0;
      break;  
    case 5:

      if(mouse_event.delta_x > tolerance || -mouse_event.delta_x > tolerance || mouse_event.delta_y > tolerance || -mouse_event.delta_y > tolerance)
      {
        printf("Tolerance exceeded\n");
        mouse_states.initial = 1;
        mouse_states.transition = 0;
      }  
    default:

      break;
    }
  }
  else if(mouse_states.drawing_right == 1){
    switch (mouse_event.type)
    {
    case 3:
      if(mouse_states.x >= x_len){
        mouse_states.drawing_right  = 0;
        mouse_states.final = 1;
      }
      else{
      mouse_states.initial = 1;
      mouse_states.drawing_right = 0;
      }
      break;
    case 5:
      mouse_states.x += mouse_event.delta_x;
      mouse_states.y += mouse_event.delta_y;
      
      if(mouse_states.y > -mouse_states.x + tolerance || mouse_states.y < -mouse_states.x - tolerance){
        printf("Tolerance exceeded\n");
        mouse_states.initial = 1;
        mouse_states.drawing_right = 0;
      }
    
      break;  
    default:
      break;
    }

  }

  else
  {
    mouse_states.initial = 1;
  }
 
}
