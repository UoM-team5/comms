#include "ARCaDIUS_Serial.h"

int Device_ID = 1001;
int Sender_ID = 1000;

int SerialrID = 0;
int SerialsID = 0;
int PK_Size = 0;
String Command = "NaN";
bool isWaiting = true;
bool busy = false;

String rID = "NaN";
String sID = "NaN";
String rPK_Size = "NaN";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!isWaiting) {
    Serial.print("The received packet: ");
    Serial.println(sID + ' '+ rID + ' ' + rPK_Size + ' ' + Command + ']');
    Serial.print("The decoded sender ID is: ");
    Serial.println(SerialsID);
    Serial.print("The decoded receiver ID is: ");
    Serial.println(SerialrID);
    Serial.print("The decoded Packet Size is: ");
    Serial.println(PK_Size);
    Serial.print("The decoded Command is: ");
    Serial.println(Command);
    Serial.println();
    isWaiting = true;
  }
  delay(100);
}

void serialEvent() {
  while (Serial.available()) {
    sID = Serial.readStringUntil(' ');
    if (sID[0] == '[') {
      SerialsID = ChecksID(sID,Sender_ID);
      rID = Serial.readStringUntil(' ');
      SerialrID = CheckrID(rID,Device_ID);
      rPK_Size = Serial.readStringUntil(' ');
      PK_Size = getPKSize(rPK_Size);
      Command = Serial.readStringUntil(']');
      if(SerialsID != -1 && SerialrID != -1){
        if(busy){
          Serial.println("[sID1001 rID1000 PK1 BUSY]");
        }
        else{
          Serial.println("[sID1001 rID1000 PK1 ACK]");
        }
      }
      Serial.flush();
      isWaiting = false;
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
