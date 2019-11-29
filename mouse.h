#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

int mouse_subscribe_int(int32_t *bit_no);

int mouse_unsubscribe_int();

int mouse_enable_data_report();

int mouse_disable_data_report();

int mouse_get_status(uint8_t *st);

int mouse_read_out_buffer(uint8_t *out_buf);

int mouse_write_cmd(uint32_t cmd);

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
