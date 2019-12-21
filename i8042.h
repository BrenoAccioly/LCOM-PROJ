#ifndef I8042_H_INCLUDED
#define I8042_H_INCLUDED

#include <lcom/lcf.h>

#define DELAY_US        20000 /* Time interval for the keyboard to respond */

#define KBD_IRQ         0x01  /* Keyboard IRQ line */

#define BREAK_ESC       0x81  /* Break Code (Release) of ESC key */

#define MAKE_W          0x11

#define MAKE_A          0x1E

#define MAKE_S          0x1F

#define MAKE_D          0x20

/* addresses */

#define KBD_OUT_BUFFER  0x60  /* Output Buffer */

#define KBD_STATUS_REG  0x64  /* Status Register */

#define KBC_CMD_REG     0x64  /* KBC commands register */

/* useful masks */

#define FULL_BUFFER     0x01  /* Used to check if the buffer is full */

#define PARITY_ERROR    0x80  /* Indicates parity error */

#define TIME_OUT_ERROR  0x40  /* Indicates timeout error */

#define BIT_3           0x08

/* KBC commands */

#define KBC_READ_CMD    0x20

#define KBC_WRITE_CMD   0x60

#define KBC_EN_IR       0X01  /* Enable Output Buffer Full Interrupt */


/////MOUSE//////

#define WRITE_TO_MOUSE    0xD4

/* MOUSE commands */

#define SET_STREAM_MODE   0xEA
#define READ_DATA         0xEB
#define EN_DATA_REPORT    0xF4
#define DIS_DATA_REPORT   0xF5

/*---------------*/
#define MOUSE_IRQ         12  /* Mouse IRQ line */

#define MOUSE_STATUS_REG  0x64 

#define MOUSE_OUT_BUFFER  0x60

#define ACK               0xFA  

#define NACK              0xFE

#define ERROR             0xFC

#define SLOPE             1.7320508

#endif // I8042_H_INCLUDED
