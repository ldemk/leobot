/*
 * ros.h
 *
 *  Created on: 29 ���. 2018 �.
 *      Author: Nemo
 */

#ifndef _ROS_H_
#define _ROS_H_

#include "ros/node_handle.h"
#include "stm32f4hardware.h"

namespace ros
{
	typedef ros::NodeHandle_<STM32F4Hardware> NodeHandle;
}


#endif /* _ROS_H_ */
