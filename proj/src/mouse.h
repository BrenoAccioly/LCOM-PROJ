#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

/** @defgroup Mouse Mouse
 * @{
 *
 * Functions related to the mouse.
 */

/**
 * @brief Subscribes and enables mouse interrupts
 * 
 * @param bit_no 
 * @return 0 on success
 */
int mouse_subscribe_int(int32_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * 
 * @return 0 on success
 */
int mouse_unsubscribe_int();

/**
 * @brief Enable data report
 * 
 * @return 0 on success
 */
int mouse_enable_data_report();

/**
 * @brief Disable data report
 * 
 * @return 0 on success
 */
int mouse_disable_data_report();

/**
 * @brief Reads the status byte
 * 
 * @param st Address of memory position to be filled with the mouse status

 * @return 0 on success
 */
int mouse_get_status(uint8_t *st);

/**
 * @brief Reads the mouse output buffer.
 * 
 * @param out_buf 
 * @return 0 on success 
 */
int mouse_read_out_buffer(uint8_t *out_buf);

/**
 * @brief Write the given command
 * 
 * @param cmd 
 * @return 0 on success
 */
int mouse_write_cmd(uint32_t cmd);

/**
 * @brief
 * 
 * @param bytes 
 * @param pp 
 * @return 0 on success 
 */
int parse_packet(uint8_t bytes[3], struct packet *pp);

void mouse_remote_operation(uint8_t bytes[3]);

struct mouse_ev mouse_event_detect	(struct packet * pp);	

void mouse_update_state(struct mouse_ev mouse_event, uint8_t x_len, uint8_t tolerance);

struct mouse_state{
  int x;
  int y;
  bool initial;
  bool drawing_left;
  bool transition;
  bool drawing_right;
  bool final;
};

#endif // MOUSE_H_INCLUDED
