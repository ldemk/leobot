/*
 * stm32_uart.h
 *
 *  Created on: 30 ���. 2018 �.
 *      Author: Nemo
 */

#ifndef ROS_LIB_STM32_UART_H_
#define ROS_LIB_STM32_UART_H_

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"

#define UART_BAUDRATE (57600)


void stm32_uart_init(void);
void stm32_uart_send_byte(uint8_t tx_byte);
int16_t stm32_uart_receive_byte(void);


#endif /* ROS_LIB_STM32_UART_H_ */
