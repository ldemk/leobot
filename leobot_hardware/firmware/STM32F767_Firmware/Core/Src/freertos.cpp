/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
// ToDo: rename this file to app
/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "gpio.h"
#include "tim.h"
#include "rtos.h"

#include "arm_math.h"


#include "ros.h"
#include "std_msgs/UInt16.h"
//#include "std_msgs/String.h"

#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Choose PID parameters */
#define PID_PARAM_KP        100            /* Proporcional */
#define PID_PARAM_KI        0.025        /* Integral */
#define PID_PARAM_KD        20            /* Derivative */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static ros::NodeHandle nh;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId LedBlinkTaskHandle;
osThreadId EncoderTaskHandle;
osThreadId RosSpinTaskHandle;
osThreadId RosSubscriberTaHandle;
osThreadId RosPublisherTasHandle;
osMessageQId RosSubsriberQueueHandle;
osMutexId rosPublishMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void motor_cb(const std_msgs::UInt16& cmd_msg);
void pid_cb(const std_msgs::UInt16& cmd_msg);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void LedBlinkTaskHandler(void const * argument);
void EncoderTaskHandler(void const * argument);
void RosSpinTaskHandler(void const * argument);
void RosSubscriberTaskHandler(void const * argument);
void RosPublisherTaskHandler(void const * argument);

//void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	nh.initNode();
  /* USER CODE END Init */

  /* Create the mutex(es) */
  /* definition and creation of rosPublishMutex */
  osMutexDef(rosPublishMutex);
  rosPublishMutexHandle = osMutexCreate(osMutex(rosPublishMutex));

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of RosSubsriberQueue */
  osMessageQDef(RosSubsriberQueue, 16, uint16_t);
  RosSubsriberQueueHandle = osMessageCreate(osMessageQ(RosSubsriberQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityIdle, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of LedBlinkTask */
  osThreadDef(LedBlinkTask, LedBlinkTaskHandler, osPriorityLow, 0, 128);
  LedBlinkTaskHandle = osThreadCreate(osThread(LedBlinkTask), NULL);

  /* definition and creation of EncoderTask */
  osThreadDef(EncoderTask, EncoderTaskHandler, osPriorityNormal, 0, 128);
  EncoderTaskHandle = osThreadCreate(osThread(EncoderTask), NULL);

  /* definition and creation of RosSpinTask */
  osThreadDef(RosSpinTask, RosSpinTaskHandler, osPriorityIdle, 0, 128);
  RosSpinTaskHandle = osThreadCreate(osThread(RosSpinTask), NULL);

  /* definition and creation of RosSubscriberTa */
  osThreadDef(RosSubscriberTa, RosSubscriberTaskHandler, osPriorityNormal, 0, 128);
  RosSubscriberTaHandle = osThreadCreate(osThread(RosSubscriberTa), NULL);

  /* definition and creation of RosPublisherTas */
  osThreadDef(RosPublisherTas, RosPublisherTaskHandler, osPriorityNormal, 0, 128);
  RosPublisherTasHandle = osThreadCreate(osThread(RosPublisherTas), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */

  //ros::Subscriber<std_msgs::UInt16> sub("motor", motor_cb);
  //nh.subscribe(sub);

  //char hello[15] = "hello world!\r\n";
  //str_msg.data = hello;
  //nh.advertise(chatter);

  /* Infinite loop */
  for(;;)
  {
	  osDelay(100);


  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LedBlinkTaskHandler */
/**
* @brief Function implementing the LedBlinkTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LedBlinkTaskHandler */
void LedBlinkTaskHandler(void const * argument)
{
	/* USER CODE BEGIN LedBlinkTaskHandler */
	MX_GPIO_Init();

  /* Infinite loop */
  for(;;)
  {
	  LL_GPIO_SetOutputPin(LD3_GPIO_Port, LD3_Pin);
	  LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin);
	  osDelay(300);

	  LL_GPIO_ResetOutputPin(LD3_GPIO_Port, LD3_Pin);
	  LL_GPIO_SetOutputPin(LD2_GPIO_Port, LD2_Pin);
	  osDelay(100);
  }
  /* USER CODE END LedBlinkTaskHandler */
}

/* USER CODE BEGIN Header_EncoderTaskHandler */
/**
* @brief Function implementing the EncoderTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_EncoderTaskHandler */
void EncoderTaskHandler(void const * argument)
{
  /* USER CODE BEGIN EncoderTaskHandler */
  // https://www.vexforum.com/t/calculating-rpms/31506/4

  uint32_t EncoderCurr = 0;
  uint32_t EncoderLast = 0;
  uint32_t RPM;

  uint32_t pmw;

  MX_TIM1_Init();

  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
  LL_TIM_EnableCounter(TIM1);

  /* ARM PID Instance, float_32 format */
  arm_pid_instance_f32 PID;
  float32_t pid_error;


  /* Set PID parameters */
  /* Set this for your needs */
  PID.Kp = PID_PARAM_KP;        /* Proporcional */
  PID.Ki = PID_PARAM_KI;        /* Integral */
  PID.Kd = PID_PARAM_KD;        /* Derivative */

  /* Initialize PID system, float32_t format */
  arm_pid_init_f32(&PID, 1);

  /* Infinite loop */
  for(;;)
  {
	  EncoderCurr = LL_TIM_GetCounter(TIM1);
	  uint32_t dir = LL_TIM_GetDirection(TIM1);
	  RPM = (EncoderCurr - EncoderLast) / 360 * 480 * 30;
	  EncoderLast = EncoderCurr;

      //ITM_SendChar('0' + RPM);
	  //ITM_SendChar('0' + EncoderCurr);
	  //printf("EncoderCurr: %d \r\n", (int)EncoderCurr);
	  printf("dir: %d \r\n", (int)dir);

	  /* Calculate error */
	  //pid_error = TEMP_CURRENT - TEMP_WANT;
	  pid_error = 1.0;


	  /* Calculate PID here, argument is error */
	  /* Output data will be returned, we will use it as duty cycle parameter */
	  //pmw = (uint32_t)arm_pid_f32(&PID, pid_error);

	  osDelay(500);
  }
  /* USER CODE END EncoderTaskHandler */
}

/* USER CODE BEGIN Header_RosSpinTaskHandler */
/**
* @brief Function implementing the RosSpinTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RosSpinTaskHandler */
void RosSpinTaskHandler(void const * argument)
{
  /* USER CODE BEGIN RosSpinTaskHandler */
  /* Infinite loop */
  for(;;)
  {
	  nh.spinOnce();
  }
  /* USER CODE END RosSpinTaskHandler */
}

/* USER CODE BEGIN Header_RosSubscriberTaskHandler */
/**
* @brief Function implementing the RosSubscriberTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RosSubscriberTaskHandler */
void RosSubscriberTaskHandler(void const * argument)
{
  /* USER CODE BEGIN RosSubscriberTaskHandler */
  ros::Subscriber<std_msgs::UInt16> sub_motor("motor", motor_cb);
  ros::Subscriber<std_msgs::UInt16> sub_pid("pid", pid_cb);
  nh.subscribe(sub_motor);
  nh.subscribe(sub_pid);
  /* Infinite loop */
  for(;;)
  {
    osDelay(200);

	  //osStatus evt  = osMessageGet (RosSubsriberQueue, 0);

  }
  /* USER CODE END RosSubscriberTaskHandler */
}

/* USER CODE BEGIN Header_RosPublisherTaskHandler */
/**
* @brief Function implementing the RosPublisherTas thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RosPublisherTaskHandler */
void RosPublisherTaskHandler(void const * argument)
{
  /* USER CODE BEGIN RosPublisherTaskHandler */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END RosPublisherTaskHandler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void motor_cb(const std_msgs::UInt16& cmd_msg)
{
	//cmd_msg.data should be in range 0 - 100
	//nh.logdebug("motor_cb\r\n");

	//osMessagePut(RosSubsriberQueue, cmd_msg, 0);

    /* debug code: */
	switch(cmd_msg.data)
	{
		case 0: LL_GPIO_ResetOutputPin(LD2_GPIO_Port, LD2_Pin); break;
		case 1: LL_GPIO_SetOutputPin(LD2_GPIO_Port, LD2_Pin); break;
	}


	//ITM_SendChar('B');
}

void pid_cb(const std_msgs::UInt16& cmd_msg)
{

}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
