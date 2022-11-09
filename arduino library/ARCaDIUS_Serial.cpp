#include "ARCaDIUS_Serial.h"


ASerial::ASerial(String DeviceDesc, int rID, int sID, int NumPump, int NumValve, int NumIrr, int NumTemp, int NumBubble, int NumMixer) {
  DeviceDesc = DeviceDesc;
  Device_ID = rID;
  Sender_ID = sID;
  NumPump = NumPump;
  NumValve = NumValve;
  NumIrr = NumIrr;
  NumTemp = NumTemp;
  NumBubble = NumBubble;
  NumMixer = NumMixer;
}

void ASerial::serialFlush() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}
int ASerial::CheckrID(String rID, int Device_ID) {
  int ID = 0;
  for (int i = 3; i < rID.length(); i++) {
    ID = ID * 10 + (rID[i] - '0');
  }
  if (ID != Device_ID) {
    Error(3);
    return -1;
  }
  else {
    return ID;
  }
}
int ASerial::ChecksID(String sID, int Sender_ID) {
  int ID = 0;
  for (int i = 4; i < sID.length(); i++) {
    ID = ID * 10 + (sID[i] - '0');
  }
  if (ID != Sender_ID) {
    Error(4);
    return -1;
  }
  else {
    return ID;
  }
}
int ASerial::getPKSize(String rPK_Size) {
  int PK_Size = 0;
  if (rPK_Size.length() == 3) {
    PK_Size = (rPK_Size[2] - '0');
  }
  else {
    PK_Size = (rPK_Size[2] - '0') * 10;
    PK_Size = PK_Size + (rPK_Size[3] - '0');
  }
  return PK_Size;
}
void ASerial::Error(int code) {
  Serial.println("[sID1001 rID1000 PK1 ERR" + (String)code + "]");
}

void ASerial::SerialLoop() {
  if (!isWaiting) {
  }
  else {
    isWaiting = false;
  }
}

void ASerial::process() {
  while (Serial.available()) {
    sID = Serial.readStringUntil(' ');
    if (sID[0] == '[') {
      //Serial.println(sID);
      SerialsID = ChecksID(sID, Sender_ID);
      rID = Serial.readStringUntil(' ');
      //Serial.println(rID);
      SerialrID = CheckrID(rID, Device_ID);
      rPK_Size = Serial.readStringUntil(' ');
      //Serial.println(rPK_Size);
      PK_Size = getPKSize(rPK_Size);
      Command = Serial.readStringUntil(']');
      Serial.flush();
      serialFlush();
      break;
    }
    else {
      Error(0);
      serialFlush();
      break;
    }
  }
}

void ASerial::print2Serial() {
  Serial.print("The decoded sender ID is: ");
  Serial.println((String)GetsID());
  Serial.print("The decoded receiver ID is: ");
  Serial.println((String)GetrID());
  Serial.print("The decoded Packet Size is: ");
  Serial.println((String)GetPKSize());
  Serial.print("The decoded Command is: ");
  Serial.println((String)GetCommand());
  Serial.println();
}
bool ASerial::GetState() {
  return isWaiting;
}
void ASerial::SetState(bool s) {
  isWaiting = s;
}
void ASerial::SetsID(int ID) {
  Sender_ID = ID;
}
int ASerial::GetsID() {
  return Sender_ID;
}
int ASerial::GetSerialsID() {
  return SerialsID;
}
void ASerial::SetrID(int ID) {
  Device_ID = ID;
}
int ASerial::GetrID() {
  return Device_ID;
}
int ASerial::GetSerialrID() {
  return SerialrID;
}
int ASerial::GetPKSize() {
  return PK_Size;
}
String ASerial::GetCommand() {
  return Command;
}
