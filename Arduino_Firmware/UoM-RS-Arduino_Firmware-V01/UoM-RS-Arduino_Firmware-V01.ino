
#include "Robotics_Systems_Library.h"



UOM_RS_Robot robot;


bool control_mode[MAX_ID] = {
                  POSITION, // Motor ID 1
                  VELOCITY, // Motor ID 2
                  POSITION, // Motor ID 3
                  POSITION, // Motor ID 4
                  POSITION, // Motor ID 5
                  VELOCITY, // Motor ID 6
                  POSITION, // Motor ID 7
                  POSITION  // Motor ID 8
                  };


void setup() {
  
  // int a = robot.EstablishMotorSerialConnection();
  robot.EstablishUSBConnection();
  robot.EstablishMotorSerialConnection();
  
  robot.getID();
  
  if (robot.connection_status != 1) {
    Serial.println("Number of Connected Motors: " + String(robot.num_ID));
  }
  

  robot.InitMotorFeedback();

  robot.SetMotorControlMode(control_mode);

  // EstablishMotorSerialConnection
  // EstablishComputerSerialConnection
  // Get Control Type
  // BeginTimer

}

void loop() {

  
  if (robot.connection_status != 1) {
    robot.SerialMonitorMotorControl();
  }
  

  // ReadFromMATLAB
  // ChangeMotorParam
  // DriveMotors



}
