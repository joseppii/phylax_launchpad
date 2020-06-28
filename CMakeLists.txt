cmake_minimum_required(VERSION 3.0.2)
project(phylax_launchpad)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++17)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  rosserial_client
  rosserial_tivac
  std_msgs
)

generate_messages(
  DEPENDENCIES std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime std_msgs
)
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

rosserial_generate_ros_lib(
  PACKAGE rosserial_tivac
  SCRIPT make_libraries_tiva
)

# Because we need to generate our custom messages before exporting our rosserial ros_lib
add_dependencies(${PROJECT_NAME}_ros_lib ${${PROJECT_NAME}_EXPORTED_TARGETS})

# Chatter tutorial
rosserial_configure_client(
  DIRECTORY launcher
  TOOLCHAIN_FILE ${ROSSERIAL_TIVAC_TOOLCHAIN}
)

rosserial_add_client_target(launcher launcher.axf ALL)
rosserial_add_client_target(launcher flash)
rosserial_add_client_target(launcher size)
rosserial_add_client_target(launcher dump)
