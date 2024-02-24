#ifndef ROBOTICS_SYSTEMS_LIBRARY_H
#define ROBOTICS_SYSTEMS_LIBRARY_H

#include <Arduino.h>
#include <SMS_STS.h>
#include <math.h>


// SMS_STS st;                                       // Motor Object used to send & receive motor data
#define MOTOR_BAUD_RATE 1000000                       // BaudRate used between Arduino and Motors
#define USB_BAUD_RATE 115200                          // BaudRate used between MATLAB and Arduino
#define CONNECTION_TIMEOUT 2000                       // Timeout for connecting to MATLAB
#define MAX_ID 8                                      // MAX Number of motors
#define POSITION 0
#define VELOCITY 1
#define ERROR -1


class UOM_RS_Robot
{
public:
  SMS_STS motors;                                       // Motor Object used to send & receive motor data
  u8 num_ID;
  int connection_status;
  u8 ID[MAX_ID] = {1, 2, 3, 4, 5, 6, 7, 8};
  u8 connected_ID[MAX_ID] = {255, 255, 255, 255, 255, 255, 255, 255};
  s16 q_FB[MAX_ID];
  s16 q_STS[MAX_ID];
  s16 q_dot_STS[MAX_ID] = {0, 0, 0, 0, 0, 0, 0, 0};
  
  

  UOM_RS_Robot();
  int EstablishUSBConnection();
  int EstablishMotorSerialConnection();
  int getID();
  int connectToMATLAB();

  void SetMotorControlMode(bool *new_control_mode);
  void SerialMonitorMotorControl();
  
  void DriveMotors();
  void ReadFeedback();
  void InitMotorFeedback();

private:
  bool control_mode[MAX_ID] = {0,0,0,0,0,0,0,0};
  void UpdateMotorControlMode();
};




#endif