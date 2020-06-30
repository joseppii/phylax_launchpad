#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// TivaC specific includes
extern "C"
{
  #include <driverlib/sysctl.h>
  #include <driverlib/gpio.h>
}
// ROS includes
#include <ros.h>
#include <std_msgs/String.h>

#include "ICM_20948.h"

// ROS nodehandle
ros::NodeHandle nh;

std_msgs::String str_msg;
ros::Publisher launchpad("imu", &str_msg);
char sensorID[13] = "";

int main(void)
{
  uint8_t i2c_data;
  // TivaC application specific code
  MAP_FPUEnable();
  MAP_FPULazyStackingEnable();
  // TivaC system clock configuration. Set to 80MHz.
  MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
   
  ICM20948 *imu = new ICM20948(0x69);
  imu->init();

  i2c_data = imu->getAccelerometer_ID();
  
  // ROS nodehandle initialization and topic registration
  nh.initNode();
  nh.advertise(launchpad);

  sprintf(sensorID, "SensorID %02x:", i2c_data);
  str_msg.data = sensorID;
  while (1)
  {
    // Publish message to be transmitted.
    launchpad.publish(&str_msg);

    // Handle all communications and callbacks.
    nh.spinOnce();

    // Delay for a bit.
    nh.getHardware()->delay(100);
  }
  delete imu;
}

