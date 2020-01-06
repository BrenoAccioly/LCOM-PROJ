#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

uint32_t counter = 0;
int32_t hook_id = TIMER0_IRQ; 

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  
  // clock/div = freq

  uint16_t div = TIMER_FREQ/freq;
  uint8_t msb, lsb;
  uint8_t st;
  uint8_t auxSt;
  
  if(freq < 19){
    printf("timer_set_frequency failed: freq too low \n");
    return 1;
  }
    
  if(freq > TIMER_FREQ){
    printf("timer_set_frequency failed: freq too high \n");
    return 1;
  }

  if(timer < 0 || timer > 2){
    printf("timer_set_frequency failed: not a valid timer \n");
    return 1;
  }

  if(timer_get_conf(timer, &st) != 0){
    return 1;
  }

  if(util_get_LSB(div, &lsb) != 0)
    return 1;
  if(util_get_MSB(div, &msb) != 0)
    return 1;

  auxSt = st & 0x0F;

  if(timer == 0){
    if(sys_outb(TIMER_CTRL, TIMER_SEL0 | TIMER_LSB_MSB | auxSt) != OK)
      return 1;
    if(sys_outb(TIMER_0, lsb) != OK)
      return 1; 
    if(sys_outb(TIMER_0, msb) != OK)
      return 1; 

  }
  else if(timer == 1){
    if(sys_outb(TIMER_CTRL, (TIMER_SEL1 | TIMER_LSB_MSB | auxSt)) != OK)
      return 1;
    if(sys_outb(TIMER_1, lsb) != OK)
      return 1; 
    if(sys_outb(TIMER_1, msb) != OK)
      return 1;  
  }
  else if(timer == 2){
    if(sys_outb(TIMER_CTRL, (TIMER_SEL2 | TIMER_LSB_MSB | auxSt)) != OK)
      return 1;
    if(sys_outb(TIMER_2, lsb) != OK)
      return 1; 
    if(sys_outb(TIMER_2, msb) != OK)
      return 1;  
  }

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {

  int32_t aux_hook_id = hook_id;
 
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK)
    return 1;

  //there is no need to call sys_irqenable()

  *bit_no = BIT(aux_hook_id);
  return 0;
}

int (timer_unsubscribe_int)() {

  if (sys_irqdisable(&hook_id) != OK) {
		return 1;
	}
  if(sys_irqrmpolicy(&hook_id) != OK)
    return 1;

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {

  if(sys_outb(TIMER_CTRL, TIMER_RB_COUNT_ | TIMER_RB_CMD | TIMER_RB_SEL(timer)) == OK) {

    switch (timer)
    {
      case 0:
          return util_sys_inb(TIMER_0, st);
      case 1:
          return util_sys_inb(TIMER_1, st);
      case 2:
          return util_sys_inb(TIMER_2, st);

      default: printf("timer_get_conf failed: not a valid timer \n");
          break;
    }
  }
  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val value;

  if(timer < 0 || timer > 2){
    printf("timer_display_conf failed: not a valid timer \n");
    return 1;
  }
    

  if(field == tsf_all){
    value.byte = st;
  }

  else if(field == tsf_initial){
    if((st & 0x30) == 16)
      value.in_mode = LSB_only;
    else if((st & 0x30) == 32)
      value.in_mode = MSB_only;
    else if((st & 0x30) == 48)
      value.in_mode = MSB_after_LSB;
    else
      value.in_mode = INVAL_val;
    
  }

  else if(field == tsf_mode){
    if(((st >> 1) & 0x07) == 0)
      value.count_mode = 0;
    else if(((st >> 1) & 0x07) == 1)
      value.count_mode = 1;
    else if(((st >> 1) & 0x07) == 2 || ((st >> 1) & 0x07) == 6)
      value.count_mode = 2;
    else if(((st >> 1) & 0x07) == 3 || ((st >> 1) & 0x07) == 7)
      value.count_mode = 3;
    else if(((st >> 1) & 0x07) == 4)
      value.count_mode = 4;
    else if(((st >> 1) & 0x07) == 5)
      value.count_mode = 5;
  }

  else if(field == tsf_base){
    if(st & 0x01)
      value.bcd = 1; //true if bcd
    else
      value.bcd = 0;

  }
    
  timer_print_config(timer, field, value);

  return 0;
}
