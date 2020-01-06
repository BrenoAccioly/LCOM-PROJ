#include <lcom/lcf.h>
#include "kbd.h"
#include "i8042.h"


uint32_t sys_inb_calls;
int32_t kbd_hook_id = KBD_IRQ;
static uint8_t kbd_status_byte;
uint8_t kbd_out_buffer_byte = 0;


int kbd_subscribe_int(uint8_t *bit_no){

  int32_t aux_hook_id = kbd_hook_id;

  if(sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, & kbd_hook_id) != OK)
    return 1;

  *bit_no = BIT(aux_hook_id);
  return 0;
}

int kbd_unsubscribe_int(){

  if (sys_irqdisable(& kbd_hook_id) != OK) {
		return 1;
	}
  if(sys_irqrmpolicy(& kbd_hook_id) != OK)
    return 1;

  return 0;
}

int kbd_get_status(uint8_t *st){

  #ifdef LAB3
  sys_inb_calls++;
  #endif

  return util_sys_inb(KBD_STATUS_REG, st);
}

int kbd_read_out_buffer(uint8_t *out_buf){

  #ifdef LAB3
  sys_inb_calls++;
  #endif

  return util_sys_inb(KBD_OUT_BUFFER, out_buf);

}

void kbd_read_scancode(bool *two_byte, bool *make, uint8_t *size, uint8_t scancode[]){

    if(*two_byte){
      *two_byte = 0;

      scancode[*size-2] = kbd_out_buffer_byte;
      if((kbd_out_buffer_byte & 0x80) >> 7)
          *make = 0;
      else
          *make = 1;   
      return;          
    }

    if(kbd_out_buffer_byte == 0xE0){
      *two_byte = 1;
      *size = 2; 
      scancode[*size-1] = kbd_out_buffer_byte;
      return;
    }

    else{    
      *size = 1;
      scancode[*size-1] = kbd_out_buffer_byte;
      if((kbd_out_buffer_byte & 0x80) >> 7)
          *make = 0;
      else
          *make = 1;   
      return;   
    }
             
}

int kbc_read(){
    if(sys_outb(KBC_CMD_REG, KBC_READ_CMD) == OK){
      kbd_read_out_buffer(&kbd_out_buffer_byte);
      return 0;
    }
    return 1;
}

int kbc_write(uint8_t args)
  {
    if(kbd_get_status(& kbd_status_byte))
      return 1;

    if(kbd_status_byte & FULL_BUFFER){
      printf("kbc_write Failed: Buffer Full");
      return 1;
    }

    if(sys_outb(KBC_CMD_REG, KBC_WRITE_CMD) == OK){
      sys_outb(KBD_OUT_BUFFER, args);
      return 0;
    }
    return 1;
  }


void (kbc_ih)() {
  
  if(kbd_get_status(& kbd_status_byte))
  {
    kbd_out_buffer_byte = 0x0; return;
  } 

  if( (kbd_status_byte & PARITY_ERROR))
  {
    printf("IH Failed: Parity Error \n");
    kbd_out_buffer_byte = 0x0;
    return;
  } 
  if( (kbd_status_byte & TIME_OUT_ERROR))
  {
    printf("IH Failed: Time Out Error \n");
    kbd_out_buffer_byte = 0x0;
    return;
  }   

  if(kbd_status_byte & FULL_BUFFER){
    if(kbd_read_out_buffer(& kbd_out_buffer_byte) == 0)
      return;
    kbd_out_buffer_byte = 0x0;
  }
  else
    kbd_out_buffer_byte = 0x0;

}
