#include "ARCaDIUS_Serial.h"


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

void ASerial::Error(int code){
  Serial.println("[sID1001 rID1000 PK1 ERR" + (String)code + "]");
}
