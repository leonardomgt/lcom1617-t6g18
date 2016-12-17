#ifndef _LCOM_MACROS_H_
#define _LCOM_MACROS_H_

#define TIMER_HOOK_ID  1
#define KEYBOARD_HOOK_ID 5

typedef unsigned char scode;

#define PATH_TO_LOG 		"/home/lcom/lcom1617-t6g18/proj/log.txt"
#define PATH_TO_DEBUG 		"/home/lcom/lcom1617-t6g18/proj/debug.txt"


/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define TIMER_FREQ  1193182    /**< @brief clock frequency for timer in PC and AT */

#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	        0    /**< @brief Timer 0 IRQ line */

/* I/O port addresses */

#define TIMER_0			0x40 /**< @brief Timer 0 count register */
#define TIMER_1			0x41 /**< @brief Timer 1 count register */
#define TIMER_2			0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL		0x43 /**< @brief Control register */

#define SPEAKER_CTRL		0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0		0x00     /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1		BIT(6)   /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2		BIT(7)   /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD		(BIT(7)|BIT(6))  /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB		BIT(4)  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB		BIT(5)  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB		(TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE		(BIT(2)|BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN		BIT(2)          /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD			0x01   /**< @brief Count in BCD */
#define TIMER_BIN			0x00   /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_         BIT(5)
#define TIMER_RB_STATUS_        BIT(4)
#define TIMER_RB_SEL(n)         BIT((n)+1)


// KEYBOARD MACROS

#define KBD_IRQ	        1    /**< @brief KEYBOARD IRQ line */

/* I/O port addresses */

#define STATUS_REG		0x64 /**< @brief Keyboard status register register */
#define OUT_REG			0x60 /**< @brief Keyboard out buffer register */
#define IN_REGa			0x60 /**< @brief Keyboard in buffer register */
#define IN_REGb			0x64 /**< @brief Keyboard in buffer register */

#define ESC_MAKECODE	0x1 /**< @brief ESC scancode */
#define ENTER_MAKECODE	0x1C /**< @brief ENTER scancode */
#define SPACE_MAKECODE	0x39 /**< @brief SPACE scancode */
#define ONE_MAKECODE	0x2 /**< @brief ONE scancode */
#define TWO_MAKECODE	0x3 /**< @brief TWO scancode */
#define THREE_MAKECODE	0x4 /**< @brief THREE scancode */
#define FOUR_MAKECODE	0x5 /**< @brief FOUR scancode */

#define ESC_BREAKCODE	0x81 /**< @brief ESC scancode */
#define ENTER_BREAKCODE	0x9C /**< @brief ENTER scancode */
#define SPACE_BREAKCODE	0xB9 /**< @brief SPACE scancode */
#define ONE_BREAKCODE	0x82 /**< @brief ONE scancode */
#define TWO_BREAKCODE	0x83 /**< @brief TWO scancode */
#define THREE_BREAKCODE	0x84 /**< @brief THREE scancode */
#define FOUR_BREAKCODE	0x85 /**< @brief FOUR scancode */

#define BACKSPACE_SCANCODE	0xE /**< @brief BACKSPACE scancode */

#define TWO_BYTES_SC    0xE0 /**< @brief Scancode with more than one byte */

#define KBC_CMD_REG 	0x64 /**< @brief Keyboard Controller Port */
#define KBD_CMD_REG 	0x60 /**< @brief Keyboard Port */


#define RESEND 			0xFE /**< @brief Keyboard Resend Response */
#define ERROR		 	0xFC /**< @brief Keyboard Error Response */
#define ACK		 		0xFA /**< @brief Keyboard Ack */

#define LEDSCOMMAND 	0xED /**< @brief Toogle Leds */
#define CLEARBUFFER		0xF4 /**< @brief Clear Keyboard buffer */

#define IBF				BIT(1) /**< @brief Input buffer bit */

#define OBF				BIT(0) /**< @brief Input buffer bit */


#define WAIT_KBC 		20000 /**< @brief Keyboard delay */

#define PAR_ERR			BIT(7)  /**< @brief Parity Error */
#define TO_ERR			BIT(6)  /**< @brief Timeout Error */

// VIDEO CARD

#define WAIT_TIME_TEST_INIT			30000000
#define GRAPHICS_MODE_105			0x105
#define VBE_MODE_GET				0x4F01
#define VBE_MODE_SET				0x4F02
#define LINEAR_MODE					BIT(14)

#define	HRES						1024
#define VRES						768

// colors


#define GREEN						2
#define BLUE						9
#define SKY_BLUE					31
#define PINK						45
#define RED							36
#define GREY						7
#define EGG							55
#define BLACK						0
#define PURPLE						29
#define LIME						22
#define CORAL						60
#define CYAN						19
#define ORANGE						52
#define BROWN						32
#define YELLOW						54
#define WHITE						63
#define MILITAR						48


#define TRANSPARENT					8

#define BACKGROUND_COLOR			SKY_BLUE






/**@}*/

#endif /* _LCOM_MACROS_H_ */
