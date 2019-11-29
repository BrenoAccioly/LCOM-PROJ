#ifndef KBD_H_INCLUDED
#define KBD_H_INCLUDED

/*
  Subscribes and enables keyboard interrupts
*/
int kbd_subscribe_int(uint8_t *bit_no);

/*
  Unsubscribes keyboard interrupts
*/
int kbd_unsubscribe_int();

/*
  Reads output buffer byte
*/
int kbd_read_out_buffer(uint8_t *out_buf);

/*
  Reads keyboard status byte
*/
int kbd_get_status(uint8_t *st);

/*
  reads scancode (just accepts codes with 1 or 2 bytes)
  returns by parameters if it is a make/break code, two byte
  its size and the array with the scancode 
*/
void kbd_read_scancode(bool *two_byte, bool *make, uint8_t *size, uint8_t scancode[]);

/*
  reads kbc command byte
*/
int kbc_read();

/*
  write kbc command byte
*/
int kbc_write(uint8_t args);

#endif // KBD_H_INCLUDED
