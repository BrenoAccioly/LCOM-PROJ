#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = 0xFF & val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (val >> 8) & 0xFF;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
    uint32_t aux;
    if(sys_inb(port, &aux) == OK){
        *value = aux & 0xFF; 
        return 0;
    }
    return 1;
}

