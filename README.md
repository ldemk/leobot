| Service  | Status |
|---------------|--------|
| Docker Image | [![Docker Hub](https://img.shields.io/docker/build/rosukraine/leobot.svg)](https://hub.docker.com/r/rosukraine/leobot/) |
| Code Style Checks | [![CircleCI](https://circleci.com/gh/ros-ukraine/leobot.svg?style=shield)](https://circleci.com/gh/ros-ukraine/leobot) |
| Unit Tests | [![Build Status](https://img.shields.io/shippable/5a1348e66301ee07008ca23f.svg)](https://app.shippable.com/github/ros-ukraine/leobot) |
| Install Tests | [![Build Status](https://travis-ci.org/ros-ukraine/leobot.svg?branch=kinetic-devel)](https://travis-ci.org/ros-ukraine/leobot) |
| Code Coverage | [![Coverage Badge](https://api.shippable.com/projects/5a1348e66301ee07008ca23f/coverageBadge?branch=kinetic-devel)](https://app.shippable.com/github/ros-ukraine/leobot) |

# leobot
![LeoBot](https://raw.githubusercontent.com/ros-ukraine/leobot/kinetic-devel/docs/design/LeoBotLogo.jpg "LeoBot telepresence robot")

LeoBot telepresence robot 

[![License](https://img.shields.io/github/license/ros-ukraine/leobot.svg)](https://github.com/ros-ukraine/leobot/blob/kinetic-devel/LICENSE)


## Docker
For convenience it is recommended to use Docker containers.
Please follow these steps to run Docker container on your machine.

 1. Install Desktop OS Ubuntu Trusty or Xenial on your machine or in virtual machine
 2. Install Docker-CE using these [instructions](https://docs.docker.com/engine/installation/linux/docker-ce/ubuntu/)
 3. In order to executed Docker without sudo please execute
```bash
sudo usermod -aG docker $USER
```
 4. Logout and login to your machine again :)
 5. In case if you have NVidia graphic card customized Docker could be installed which will utilize your GPU. Please follow [these extra steps](https://github.com/NVIDIA/nvidia-docker/wiki/Installation-(version-1.0)).
 6. For development [the following](https://hub.docker.com/r/rosukraine/leobot/) docker image will be used for NVidia Docker [this one](https://hub.docker.com/r/rosukraine/leobot-dev-nvidia/).
 7. Use the following command to start ordinary Docker container
```bash
docker run -it --name leobot_dev -p 8080:8080 -p 8090:8090 -p 9090:9090 -e DISPLAY -e LOCAL_USER_ID=$(id -u) -v /tmp/.X11-unix:/tmp/.X11-unix:rw rosukraine/leobot:latest
```
for NVidia Docker please use
```bash
nvidia-docker run -it --name leobot_dev -p 8080:8080 -p 8090:8090 -p 9090:9090 -e DISPLAY -e LOCAL_USER_ID=$(id -u) -v /tmp/.X11-unix:/tmp/.X11-unix:rw rosukraine/leobot-dev-nvidia:latest
```
 8. Black window of [Terminator](https://gnometerminator.blogspot.com/p/introduction.html) UI console will appear after some time.
 9. You can use it's features to [split terminal window](https://linux.die.net/man/1/terminator) into smaller terminals and run few commands in parallel (Ctrl+Shift+E).
 10. If you want to run real robot add user to dialout group and restart Docker container
```bash
sudo usermod -a -G dialout user
```

In order to relaunch docker container after you closed Terminator window or rebooted machine please run
```bash
docker start leobot_dev
```
and for NVidia Docker
```bash
nvidia-docker start leobot_dev
```

After some time Terminator window will reappear.

## IDEs

In case if you want to run PyCharm in Docker container please run

```bash
pycharm
```

To launch QtCreator please run

```bash
qtcreator
```

For VSCode type

```bash
vscode
```

## URDF and RViz
In order to debug URDF please launch

```bash
roslaunch leobot_launch view_urdf.launch
```

To have a look on the state of the robot in RViz run

```bash
roslaunch leobot_launch rviz.launch
```

## Navigating on known map

Start office simulation

```bash
roslaunch leobot_launch simulation.launch
```

If you want to reduce usage of machine's resources and increase simulation speed you could run it without GUI in headless mode.
For these purposes you could use the following command
```bash
roslaunch leobot_launch simulation.launch headless:=true gui:=false
```

Start art gallery simulation
```bash
roslaunch leobot_launch simulation.launch world_file:=artgallery
```

Launch navigation stack (in order to launch second command split Terminator window by two using Ctrl-Shift-E. More information on Terminator shortcuts can be found [here](https://dmaricic.wordpress.com/2011/01/28/terminator-keyboard-shortcuts/))
```bash
roslaunch leobot_launch navigation.launch
```

In RViz which appear after some time select "2D Nav Goal" and robot will travel to it.
Like it is shown in [this video](https://www.youtube.com/watch?v=xSdHlC2ISq8).

## Building the map

Start simulation

```bash
roslaunch leobot_launch simulation.launch
```

Launch gmapping node

```bash
roslaunch leobot_launch gmapping.launch
```

Drive arround environment to build map using [the following keys](http://wiki.ros.org/stdr_simulator/Tutorials/Teleop%20with%20teleop_twist_keyboard#Teleoperate_your_robot.21).
*Please note that console window with gmapping launch file should be active in order to teleoperate robot using keys*

Save map to file
```bash
rosrun map_server map_saver -f <map_file_name>
```

## Starting the web server

Once you install all project dependencies, you can start the web server with such command
```bash
roslaunch leobot_launch web_server.launch
```

Additionally you can specify a custom port for the web server in docker container
```bash
roslaunch leobot_launch web_server.launch port:=1234
```
In this case you'll need to re-build the docker container
to publish the specified port to your host machine (see `docker run -p` command at [Docker](#docker) section).

If everything goes well, you'll see the message
```
Web server started at port 8080
```
After that the web server will become available on your host Ubuntu OS at http://localhost:8080 as well as from LAN.


## Using a _USB_ joystick
To use a _USB_ joystick you need to rebuild the docker container.
See item 7 in [Docker](#docker).

Add the following parameter right after `docker run`
```
--device=/dev/input/js0
```
Notice: in this case you must have the joystick plugged in when you
trigger `docker run` and every time you `docker start` the corresponding container.
Otherwise these commands will fail with an error
```
docker: Error response from daemon: linux runtime spec devices: error gathering device information while adding custom device "/dev/input/js0": no such file or directory.
```
To avoid this, you can create one container that supports USB joystick
and another default one (using different `--name` parameters).

You can test that joistick is available from the container with a command
```
cat /dev/input/js0
```
It should print strange symbols in the console when you press joystick
buttons. Press Ctrl+C to exit.

When joystick is available in the container, trigger the `simulation.launch`
and then start the tepeoperation with joystick support using command
```
roslaunch leobot_control teleop.launch joy_enabled:=true
```

If your USB joystick is connected to something different than `/dev/input/js0`
you can configure it adding such parameter
```
joy_device:=/dev/input/js1
```

To operate the robot you need to press the so-called
[deadman button](https://discourse.ros.org/t/teleop-twist-joy-inconvenient-button-settings/1035/2)
_simultaneously_ with the arrow buttons or manipulating the thumbstick.
Most often that is one of the main buttons at the right side of joystick.
Just experiment to find the right one.
If you don't press the deadman button the robot won't move.

The joystick will be working even if the console window is minimized,
unlike the keyboard teleoperation.
