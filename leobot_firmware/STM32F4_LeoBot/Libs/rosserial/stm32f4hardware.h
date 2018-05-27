/*
 * STM32Hardware.h
 *
 *  Created on: 29 ���. 2018 �.
 *      Author: Nemo
 */

#ifndef STM32F4HARDWARE_H_
#define STM32F4HARDWARE_H_

#include "stm32f4xx.h"
#include "stm32_time.h"
#include "stm32_uart.h"


class STM32F4Hardware
{
public:
	STM32F4Hardware();

	// Initialize the STM32
	void init();

	//
	int read();

	// Send a byte of data to ROS connection
	void write(uint8_t* data, uint32_t length);

	// Returns milliseconds since start of program
	unsigned long time();

};

#endif /* STM32F4HARDWARE_H_ */
