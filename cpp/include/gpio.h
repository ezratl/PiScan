/*
 * GPIO.h
 *
 *  Created on: Jun 10, 2018
 *      Author: kidsr
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

#include <stdbool.h>

#define PIN_NUMBERING_BCM
#define PIN_NUMBERING_PHYSICAL
#define PIN_NUMBERING_WPI

#ifdef PIN_NUMBERING_BCM

#else /* PIN_NUMBERING_BCM */
#ifdef PIN_NUMBERING_PHYSICAL

#else /* PIN_NUMBERING_PHYSICAL */

#endif /* PIN_NUMBERING_PHYSICAL */
#endif /* PIN_NUMBERING_BCM */

typedef enum {
	GPIO_2_I2C1_SDA,
	GPIO_3_I2C1_SCL,
	GPIO_4_GPCLK0,
	GPIO_5_GPCLK1,
	GPIO_6_GPCLK2,
	GPIO_7_SPI0_CS1,
	GPIO_8_SPIO_CS0,
	GPIO_9_SPI0_MISO,
	GPIO_10_SPIO_MOSI,
	GPIO_11_SPI0_SCLK,
	GPIO_12,
	GPIO_13,
	GPIO_14_UART0_TXD,
	GPIO_15_UART0_RXD,
	GPIO_16_SPI1_CS0,
	GPIO_17,
	GPIO_18_PWM,
	GPIO_19_SPI1_MISO,
	GPIO_20_SPI1_MOSI,
	GPIO_21_SPI1_SCLK,
	GPIO_22,
	GPIO_23,
	GPIO_24,
	GPIO_25,
	GPIO_26,
} GPIO_Pin_t;

/* GPIO status codes */
typedef enum {
	GPIO_SUCCESS,
	GPIO_ERROR,
	GPIO_INVALID_PIN,
	GPIO_BUSY,
	GPIO_PIN_UNAVAILABLE,
	GPIO_LISTENER_ALREADY_RUNNING,
	GPIO_PIN_INVALID_MODE,
} GPIO_Status_t;

/* GPIO pin modes */
typedef enum {
	GPIO_INPUT,
	GPIO_OUTPUT,
	GPIO_PWM_OUTPUT,
	GPIO_GPIO_CLOCK,
	GPIO_SOFT_PWM_OUTPUT,
	GPIO_SOFT_TONE_OUTPUT,
	GPIO_PWM_TONE_OUTPUT
} GPIO_PinMode_t;

/* GPIO internal pull up/down modes */
typedef enum {
	GPIO_PUD_OFF,
	GPIO_PUD_DOWN,
	GPIO_PUD_UP
} GPIO_PinPUD_t;

typedef enum {
	GPIO_I2C,
	GPIO_SPI,
	GPIO_UART
} GPIO_SerialProtocol_t;

/* Configuration struct for a GPIO pin */
typedef struct {
	const GPIO_Pin_t	pinNumber;
	bool				disable;
	bool				useInterrupt;
	GPIO_PinMode_t		pinMode;
	GPIO_PinPUD_t		pullUpDnControl;
} GPIO_PinConfig_t;

typedef struct {
	GPIO_PinConfig_t	config;
} GPIO_OutputPin_t;

typedef struct {
	GPIO_PinConfig_t	config;
	void				(*onRisingEdge)(void);
	void				(*onFallingEdge)(void);
} GPIO_InputPin_t;

typedef struct {
	GPIO_PinConfig_t	pinA;
	GPIO_PinConfig_t	pinB;
	void				(*onAntiClockwiseTurn)(void);
	void				(*onClockwiseTurn)(void);
} GPIO_RotaryEncoder_t;

typedef struct {
	GPIO_SerialProtocol_t	protcol;
	GPIO_Pin_t				pinA;
	GPIO_Pin_t				pinB;
	GPIO_Pin_t				pinC;
} GPIO_SerialPort_t;

#ifdef __cplusplus
extern "C" {
#endif

extern GPIO_Status_t GPIO_setupPin(GPIO_PinConfig_t* pin);

extern GPIO_Status_t GPIO_setupRotaryEncoder(GPIO_RotaryEncoder_t* dev);

extern GPIO_Status_t GPIO_setupSerialPort(GPIO_SerialPort_t* port);

extern GPIO_Status_t GPIO_writePin(GPIO_OutputPin_t* pin, int val);

extern uint8_t GPIO_readPin(GPIO_InputPin_t* pin);

extern GPIO_Status_t GPIO_attachPinInputListener(GPIO_InputPin_t* pin);

extern GPIO_Status_t GPIO_startInputListener();

extern GPIO_Status_t GPIO_stopInputListener();

#ifdef __cplusplus
}
#endif

#endif /* SRC_GPIO_H_ */
