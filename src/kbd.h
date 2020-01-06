#ifndef KBD_H_INCLUDED
#define KBD_H_INCLUDED

/** @defgroup KBD KBD
 * @{
 *
 * Functions used for programming the keyboard.
 */

/**
 * @brief Subscribes and enables keyboard interrupts
 * 
 * @param bit_no 
 * @return int 
 */
int kbd_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * 
 * @return int 
 */
int kbd_unsubscribe_int();

/**
 * @brief Reads output buffer byte 
 * 
 * @param out_buf 
 * @return int 
 */
int kbd_read_out_buffer(uint8_t *out_buf);

/**
 * @brief Reads keyboard status byte
 * 
 * @param st 
 * @return int 
 */
int kbd_get_status(uint8_t *st);

/**
 * @brief   reads scancode (just accepts codes with 1 or 2 bytes)
  returns by parameters if it is a make/break code, two byte
  its size and the array with the scancode 
 * 
 * @param two_byte 
 * @param make 
 * @param size 
 * @param scancode 
 */
void kbd_read_scancode(bool *two_byte, bool *make, uint8_t *size, uint8_t scancode[]);

/**
 * @brief Reads kbc command byte.
 * 
 * @return int 
 */
int kbc_read();

/**
 * @brief Write kbc command byte.
 * 
 * @param args 
 * @return int 
 */
int kbc_write(uint8_t args);

#endif // KBD_H_INCLUDED
