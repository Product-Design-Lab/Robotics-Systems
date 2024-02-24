
// TODO
/*
- Add MATLAB handshake to EstablishUSBConnection() or triage to Arduino serial port
- Add manual control mode in Arduino using Serial -> Ask which motor, then what angle ("x" to cancel)
- Make q_STS & q_dot_STS private!
- Use getters & setters
- Add switch statements to loop

*/



#include "Robotics_Systems_Library.h"

UOM_RS_Robot::UOM_RS_Robot() {
  num_ID = 0;
  
  
}


int UOM_RS_Robot::EstablishUSBConnection() {
  Serial.begin(USB_BAUD_RATE); // Set serial baud rate for USB

  int start_timer = millis();
  while (!Serial) // Wait For Serial To Connect
  {
    if (millis() - start_timer > CONNECTION_TIMEOUT) {
      return -1;
    }
  }
  connection_status = connectToMATLAB();
  if (connection_status == ERROR) {
    Serial.println("Failed to connect to MATLAB. Connecting to Arduino serial monitor instead.");          
  }

  // Serial.println("Serial Connected To Arduino Successfully.");             
  return 1;
}


int UOM_RS_Robot::EstablishMotorSerialConnection() {
  Serial1.begin(MOTOR_BAUD_RATE); // Set serial baud rate for motors

  int start_timer = millis();
  while (!Serial1) // Wait For Serial To Connect
  {
    if (millis() - start_timer > CONNECTION_TIMEOUT) {
      return -1;
    }
  }             
  motors.pSerial = &Serial1;        // Assign serial port to motor
  return 1;
}



int UOM_RS_Robot::getID() {

  // Get the number if motors connected and their IDs.

  // External Variables
  // @ ID       - Array of connected motor IDs.

  // Internal Variables
  // @ numID    - Number of connected IDs.
  // @ tempID   - Temporary value for storing pre-checked ID value.
  // @ SCSCL_ID - EPROM reference for accessing motor ID.

  // Internal Functions
  // readByte() - Read a byte of data returned by the motors.

  num_ID = 0;

  for (int i = 1; i <= MAX_ID; i++) {
    // Read ID value from motor
    int tempID = motors.readByte(i, SMS_STS_ID);

    // If returned ID is valid, store.
    if (tempID > 0 && tempID <= MAX_ID) {
      connected_ID[num_ID] = tempID;
      num_ID++;
    }

  }

  return 1;
}

void UOM_RS_Robot::UpdateMotorControlMode() {

  for (int i=0; i<MAX_ID; i++) {
    if (control_mode[i]) {
      motors.WheelMode(ID[i]);
    }
    else {
      motors.ServoMode(ID[i]);
    }
  }

}

void UOM_RS_Robot::SetMotorControlMode(bool *new_control_mode) {
  for (int i=0; i<MAX_ID; i++) {
    control_mode[i] = new_control_mode[i];
  }

  UpdateMotorControlMode();

}

void UOM_RS_Robot::DriveMotors() {
  
  for (int i=0; i<MAX_ID; i++) {
    if (control_mode[i]) {
      motors.WriteSpe(ID[i], q_dot_STS[i]);
    }
    else {
      u16 Speed = 0;
      u8 ACC = 50;
      motors.WritePosEx(ID[i], q_STS[i], Speed, ACC);
    }
  }

}

void UOM_RS_Robot::ReadFeedback() {
  for (int i=0; i<MAX_ID; i++) {
    q_FB[i]=motors.ReadPos(ID[i]);
  }
}


void UOM_RS_Robot::SerialMonitorMotorControl() {
  Serial.print("Enter Motor ID: ");
  while (Serial.available() == 0) {}     //wait for data available
  String teststr = Serial.readString();  //read until timeout
  teststr.trim();                        // remove any \r \n whitespace at the end of the String
  int motorID = teststr.toInt();
  Serial.println(motorID);

  Serial.print("Enter Value: ");
  while (Serial.available() == 0) {}     //wait for data available
  teststr = Serial.readString();  //read until timeout
  teststr.trim();                        // remove any \r \n whitespace at the end of the String
  int motorValue = teststr.toInt();
  Serial.println(motorValue);

  if (control_mode[ID[motorID-1]]-1) {
    q_dot_STS[ID[motorID-1]-1] = motorValue;
  }
  else {
    q_STS[ID[motorID-1]-1] = motorValue;
  }

  DriveMotors();
  // TO DO
  // - Drive Motor

}

void UOM_RS_Robot::InitMotorFeedback() {
  ReadFeedback();
  for (int i=0; i<MAX_ID; i++) {
    q_STS[i] = q_FB[i];
  }
}


int UOM_RS_Robot::connectToMATLAB() {

  // Establish serial connection with MATLAB by sending a single char,
  // waiting for a single char response, and then sending a final char
  // to acknoledge one was received.

  char c = 'b';
  Serial.println("a");

  int start_timer = millis();
  while ( c != 'a' ) {
    if (millis() - start_timer > CONNECTION_TIMEOUT) {
      return -1;
    }
    c = Serial.read();
  }
  Serial.println("b");

  return 1;
}


