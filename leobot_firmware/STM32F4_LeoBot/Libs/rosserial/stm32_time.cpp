/*
 * stm32_time.cpp
 *
 *  Created on: 30 ���. 2018 �.
 *      Author: Nemo
 */


#include "stm32_time.h"

//To-DO: Add defines for critical sections
//example: https://mcuoneclipse.com/2014/01/26/entercritical-and-exitcritical-why-things-are-failing-badly/

extern uint32_t tick;

void stm32_time_init(void)
{
	/* cofigure systimer for 1ms interrupt */
	if (SysTick_Config(SystemCoreClock/1000))
	{
		//Function failed.
		while(1);
	}
}

// Get the current time in milliseconds
uint32_t stm32_time_now(void)
{
	uint32_t now;

	//EnterCritical();
	__disable_irq();
	now = tick;
	__enable_irq();
	//ExitCritical();


	return now;
}
