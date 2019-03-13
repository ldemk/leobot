#include "leobot_hardware/robot_hardware.h"
#include <controller_manager/controller_manager.h>
#include <hardware_interface/actuator_state_interface.h>
#include <ros/callback_queue.h>

using leobot_hardware::LeobotRobotHW;

int main(int argc, char **argv)
{
  const double ACTUAL_CYCLE = 0.75;  
  std::string node_name = "leobot_hardware_node";

  ros::init(argc, argv, node_name);
  ros::NodeHandle top_level_node_handle;
  ros::NodeHandle root_node_handle("~");

  ros::NodeHandle private_node("~");

  ros::CallbackQueue generic_queue;
  top_level_node_handle.setCallbackQueue(&generic_queue);
  root_node_handle.setCallbackQueue(&generic_queue);

  ros::AsyncSpinner spinner(2, &generic_queue);
  spinner.start();

  ros::CallbackQueue read_state_queue;
  private_node.setCallbackQueue(&read_state_queue);

  LeobotRobotHW robot_hardware;
  robot_hardware.init(root_node_handle, private_node);
  controller_manager::ControllerManager controller_manager(&robot_hardware, top_level_node_handle);

  struct timespec last_time;
  struct timespec current_time;
  clock_gettime(CLOCK_MONOTONIC, &last_time);

  double loop_rate;
  private_node.param<double>("loop_rate", loop_rate, 10.0);

  ros::Duration desired_update_duration = ros::Duration(1.0 / loop_rate);
  ros::Duration communicaton_wait_duration = ros::Duration(ACTUAL_CYCLE / loop_rate);

  ros::Rate rate(loop_rate / ACTUAL_CYCLE);
  while (ros::ok())
  {
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    ros::Duration elapsed_time = ros::Duration(current_time.tv_sec - last_time.tv_sec +
                                               (current_time.tv_nsec - last_time.tv_nsec) / 1e+9);
    last_time = current_time;

    if (elapsed_time > desired_update_duration)
    {
      ROS_WARN_STREAM_THROTTLE(2.0, "Cycle time exceeded error threshold by: " <<
        (elapsed_time - desired_update_duration) << ", cycle time: " << elapsed_time <<
        ", desired_update_duration: " << desired_update_duration);
    }

    ros::Time timestamp = ros::Time::now();
    read_state_queue.clear();
    read_state_queue.callOne(ros::WallDuration(communicaton_wait_duration.toSec()));
    robot_hardware.read(timestamp, elapsed_time);
    controller_manager.update(ros::Time::now(), elapsed_time);
    robot_hardware.write(timestamp, elapsed_time);

    rate.sleep();
  }

  spinner.stop();

  return 0;
}