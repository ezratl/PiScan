/*
 * standard_gpio.h
 *
 *  Created on: Jul 12, 2018
 *      Author: kidsr
 */

#ifndef INCLUDE_INTERFACE_STANDARD_GPIO_H_
#define INCLUDE_INTERFACE_STANDARD_GPIO_H_

#include <stdlib.h>
#include "gpio.h"

#define PIN_BUTTON_1	0
#define PIN_BUTTON_2	0
#define PIN_BUTTON_3	0
#define PIN_BUTTON_4	0
#define	PIN_BUTTON_5	0

#define PIN_ROTENC_1	0
#define PIN_ROTENC_2	0
#define PIN_ROTENC_BTN	0

#define PIN_AUDIO_CTRL	0
#define PIN_I2C_CLK		0
#define PIN_I2C_DATA	0
#define PIN_SPI_MOSI	0
#define PIN_SPI_MISO	0

const GPIO_InputPin_t gpio_default_button_1 = {
		.config = {
				.pinNumber = PIN_BUTTON_1,
				.pinMode = GPIO_INPUT,
				.pullUpDnControl = GPIO_PUD_DOWN,
		},
		.onFallingEdge = NULL,
		.onRisingEdge = NULL
};

const GPIO_InputPin_t gpio_default_button_2 = {
		.config = {
				.pinNumber = PIN_BUTTON_2,
				.pinMode = GPIO_INPUT,
				.pullUpDnControl = GPIO_PUD_DOWN,
		},
		.onFallingEdge = NULL,
		.onRisingEdge = NULL
};

const GPIO_InputPin_t gpio_default_button_3 = {
		.config = {
				.pinNumber = PIN_BUTTON_3,
				.pinMode = GPIO_INPUT,
				.pullUpDnControl = GPIO_PUD_DOWN,
		},
		.onFallingEdge = NULL,
		.onRisingEdge = NULL
};

const GPIO_InputPin_t gpio_default_button_4 = {
		.config = {
				.pinNumber = PIN_BUTTON_4,
				.pinMode = GPIO_INPUT,
				.pullUpDnControl = GPIO_PUD_DOWN,
		},
		.onFallingEdge = NULL,
		.onRisingEdge = NULL
};

const GPIO_InputPin_t gpio_default_button_5 = {
		.config = {
				.pinNumber = PIN_BUTTON_5,
				.pinMode = GPIO_INPUT,
				.pullUpDnControl = GPIO_PUD_DOWN,
		},
		.onFallingEdge = NULL,
		.onRisingEdge = NULL
};

const GPIO_OutputPin_t gpio_default_audio_control = {
		.config = {
				.pinNumber = PIN_AUDIO_CTRL,
				.pinMode = GPIO_OUTPUT
		}
};

#endif /* INCLUDE_INTERFACE_STANDARD_GPIO_H_ */
