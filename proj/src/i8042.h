#ifndef I8042_H_INCLUDED
#define I8042_H_INCLUDED

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042.
 */



#define DELAY_US        20000 /**< @brief  Time interval for the keyboard to respond */


#define KBD_IRQ         0x01 /**< @brief Keyboard IRQ line */

#define BREAK_ESC       0x81 /**< @brief  Break Code (Release) of ESC key */

#define MAKE_BACKSPACE  0x0E /**< @brief @brief  Make Code of BACKSPACE */

#define MAKE_ENTER      0x1C /**< @brief  Make Code of ENTER */

#define MAKE_W          0x11
#define BREAK_W         0x91

#define MAKE_A          0x1E
#define BREAK_A         0x9E

#define MAKE_S          0x1F
#define BREAK_S         0x9F

#define MAKE_D          0x20
#define BREAK_D         0xA0

#define MAKE_B          0x30
#define MAKE_C          0x2E
#define MAKE_E          0x12
#define MAKE_F          0x21
#define MAKE_G          0x22
#define MAKE_H          0x23
#define MAKE_I          0x17
#define MAKE_J          0x24
#define MAKE_K          0x25
#define MAKE_L          0x26
#define MAKE_M          0x32
#define MAKE_N          0x31
#define MAKE_O          0x18
#define MAKE_P          0x19
#define MAKE_Q          0x10
#define MAKE_R          0x13
#define MAKE_T          0x14
#define MAKE_U          0x16
#define MAKE_V          0x2F
#define MAKE_X          0x2D
#define MAKE_Y          0x15
#define MAKE_Z          0x2C

/* addresses */

#define KBD_OUT_BUFFER  0x60  /**< @brief  Output Buffer */

#define KBD_STATUS_REG  0x64  /**< @brief  Status Register */

#define KBC_CMD_REG     0x64  /**< @brief  KBC commands register */

/* useful masks */

#define FULL_BUFFER     0x01  /**< @brief  Used to check if the buffer is full */

#define PARITY_ERROR    0x80  /**< @brief  Indicates parity error */

#define TIME_OUT_ERROR  0x40  /**< @brief Indicates timeout error */

#define BIT_3           0x08

/* KBC commands */

#define KBC_READ_CMD    0x20

#define KBC_WRITE_CMD   0x60

#define KBC_EN_IR       0X01  /**< @brief Enable Output Buffer Full Interrupt */


/////MOUSE//////

#define WRITE_TO_MOUSE    0xD4

/* MOUSE commands */

#define SET_STREAM_MODE   0xEA
#define READ_DATA         0xEB
#define EN_DATA_REPORT    0xF4
#define DIS_DATA_REPORT   0xF5

/*---------------*/
#define MOUSE_IRQ         12  /**< @brief Mouse IRQ line */

#define MOUSE_STATUS_REG  0x64 

#define MOUSE_OUT_BUFFER  0x60

#define ACK               0xFA  

#define NACK              0xFE

#define ERROR             0xFC

#define SLOPE             1.7320508

#endif // I8042_H_INCLUDED
