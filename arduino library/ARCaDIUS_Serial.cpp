#include "ARCaDIUS_Serial.h"
#include <TimerOne.h>

ASerial::ASerial(String DD, int rID, int sID, int P, int V, int I, int T, int B, int M, int Res) {
  ResPin = Res;
  digitalWrite(ResPin, HIGH);
  pinMode(ResPin, OUTPUT);
  DeviceDesc = DD;
  Device_ID = rID;
  Sender_ID = sID;
  sACK = "[sID" + (String)rID + " rID" + (String)sID + " PK1 ACK]";
  sBUSY = "[sID" + (String)rID + " rID" + (String)sID + " PK1 BUSY]";
  NumPump = P;
  NumValve = V;
  NumIrr = I;
  NumTemp = T;
  NumBubble = B;
  NumMixer = M;
  isWaiting = true;
  instance0_ = this;
  TempVal = new float[NumTemp];
  BubbleVal = new float[NumBubble];
  //attachInterrupt(digitalPinToInterrupt(intPin), serialInterrupt, FALLING);
  //Timer1.initialize(300000);
  for (int i = 0; i < NumTemp; i++) {
    TempVal[i] = 0;
  }
  for (int i = 0; i < NumBubble; i++) {
    BubbleVal[i] = 0;
  }
}

ASerial * ASerial::instance0_;

void ASerial::Start() {
  //  while (!GotCommand()) {}
  //  if (GetCommand() != DETAIL) {
  //    Error(5);
  //    while(1){
  //      int R = GetCommand();
  //    }
  //  }
  FinishedCommand();
}

void ASerial::serialFlush() {
  int i = 0;
  while (Serial.available() > 0 && i < 500) {
    char t = Serial.read();
    i++;
  }
}

void ASerial::ReturnDetails() {
  Serial.println("[sID" + (String)Device_ID +
                 " rID" + (String)Sender_ID +
                 " PK6" +
                 " P" + (String)NumPump +
                 " V" + (String)NumValve +
                 " I" + (String)NumIrr +
                 " M" + (String)NumMixer +
                 " T" + (String)NumTemp +
                 " B" + (String)NumBubble +
                 " (" + DeviceDesc + ")]");

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
  Serial.println("[sID" + (String)Device_ID + " rID" + (String)Sender_ID + " PK1 ERR" + (String)code + "]");
}

bool ASerial::GotCommand(){
  if(Serial.available()>0){
    Serial.println(instance0_->sACK);
    if (Serial.peek() == 'K') {
      Serial.println("KILL");
      digitalWrite(instance0_->ResPin, LOW);
    }
    return true;
  }
  else{
    return false;
  }
}

int ASerial::process() {
  if (Serial.available() > 0) {
    sID = Serial.readStringUntil(' ');
    if (sID[0] == '[') {
      //Serial.println(sID);
      SerialsID = ChecksID(sID, Sender_ID);
      if (SerialsID != -1) {
        rID = Serial.readStringUntil(' ');
        //Serial.println(rID);
        SerialrID = CheckrID(rID, Device_ID);
        if (SerialrID != -1) {
          rPK_Size = Serial.readStringUntil(' ');
          //Serial.println(rPK_Size);
          PK_Size = getPKSize(rPK_Size);
          Command = Serial.readStringUntil(']');
          //Serial.println(Command);
          Data = Command;
          Serial.println("[sID" + (String)Device_ID + " rID" + (String)Sender_ID + " PK1 VALID]");
          Serial.flush();
          serialFlush();
          return 1;
        }
      }
    }
    else {
      Error(0);
      serialFlush();
      return -1;
    }
    return -1;
  }
}

void ASerial::analyse() {
  switch (Command[0]) {
    case 'P':
      op = PUMP;
      Pump();
      break;
    case 'M':
      op = MIXER;
      Mixer();
      break;
    case 'V':
      op = VALVE;
      Valve();
      break;
    case 'I':
      op = SHUTTER;
      Shutter();
      break;
    case 'R':
      readSensors();
      break;
    case 'D':
      op = DETAIL;
      ReturnDetails();
      break;
    default:
      break;
  }
}

String readStringuntil(String s, char c) {
  int i = 0;
  String r = "";
  //Serial.println(sizeof(s));

  while (i < s.length()) {
    if (s[i - 1] == c) {
      //Serial.print(r);
      return r;
    }
    else {
      r = r + s[i];
      //Serial.println(r[i]);
    }
    i++;
  }
  return r;
}

void ASerial::Pump() {
  String rubbish;
  pump = Command[1] - '0';
  //Serial.println(pump);
  rubbish = readStringuntil(Command, 'm');
  Command.remove(0, rubbish.length());
  pumpValue = readStringuntil(Command, ' ').toFloat();
  //Serial.println(pumpValue);
  rubbish = readStringuntil(Command, 'D');
  Command.remove(0, rubbish.length());
  if (pumpValue > 0) {
    pumpDir = 1;
  }
  else {
    pumpDir = 0;
  }
  //pumpDir = Command[0] - '0';
  //Serial.println(pumpDir);
}

void ASerial::Valve() {
  String rubbish;
  valve = Command[1] - '0';
  //Serial.println(valve);
  rubbish = readStringuntil(Command, 'S');
  Command.remove(0, rubbish.length());
  valveState = Command[0] - '0';
  //Serial.println(valveState);
}

void ASerial::Mixer() {
  String rubbish;
  mixer = Command[1] - '0';
  //Serial.println(mixer);
  rubbish = readStringuntil(Command, 'S');
  Command.remove(0, rubbish.length());
  mixerSpeed = readStringuntil(Command, ' ').toFloat();
  //Serial.println(mixerSpeed);
  rubbish = readStringuntil(Command, 'D');
  Command.remove(0, rubbish.length());
  mixerDir = Command[0] - '0';
  //Serial.println(mixerDir);
}

void ASerial::Shutter() {
  String rubbish;
  shutter = Command[1] - '0';
  //Serial.println(shutter);
  rubbish = readStringuntil(Command, 'S');
  Command.remove(0, rubbish.length());
  shutterPos = readStringuntil(Command, ' ').toFloat();
  //Serial.println(shutterPos);
}

void ASerial::readSensors() {
  int PK_size = (NumBubble + NumTemp) * 2;
  String SenVal = "[sID" + (String)Device_ID + " rID" + (String)Sender_ID + " PK" + (String)PK_size + " SEN";
  for (int i = 0; i < NumTemp; i++) {
    SenVal = SenVal + " T" + (String)(i + 1) + " S" + (String)TempVal[i];
  }
  for (int i = 0; i < NumBubble; i++) {
    SenVal = SenVal + " B" + (String)(i + 1) + " S" + (String)BubbleVal[i];
  }
  SenVal = SenVal + "]";
  Serial.println(SenVal);
}

void ASerial::updateSensors(SENSOR s, int Num, float Val) {
  switch (s) {
    case TEMP:
      TempVal[Num-1] = Val;
      break;
    case BUBBLE:
      BubbleVal[Num-1] = Val;
      break;
    default:
      break;
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
  Serial.println((String)GetData());
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
int ASerial::getPump() {
  return pump;
}
float ASerial::getPumpMls() {
  return pumpValue;
}
bool ASerial::getPumpDir() {
  return pumpDir;
}
int ASerial::getValve() {
  return (int)valve;
}
bool ASerial::getValveState() {
  return valveState;
}
int ASerial::getMixer() {
  return mixer;
}
float ASerial::getMixerSpeed() {
  return mixerSpeed;
}
bool ASerial::getMixerDir() {
  return mixerDir;
}
int ASerial::getShutter() {
  return shutter;
}
float ASerial::getShutterPos() {
  return shutterPos;
}
int ASerial::GetCommand() {
  int S = process();
  serialFlush();
  if (S == 1) {
    analyse();
    return op;
  }
  return -1;
}

void ASerial::FinishedCommand() {
  isWaiting = true;
  CMD = false;
  Serial.println("[sID" + (String)Device_ID + " rID" + (String)Sender_ID + " PK1 FREE]");
  serialFlush();
}

//void ASerial::serialInterrupt() {
//  detachInterrupt(digitalPinToInterrupt(instance0_->intPin));
//  //noInterrupts();
//  if (instance0_->isWaiting) {
//    instance0_->isWaiting = false;
//  }
//  //interrupts();
//  Timer1.attachInterrupt(reattach);
//}

//void ASerial::reattach() {
//  attachInterrupt(digitalPinToInterrupt(instance0_->intPin), serialInterrupt, FALLING);
//  Timer1.detachInterrupt();
//  //noInterrupts();
//  if (Serial.peek() == 'K') {
//    Serial.println("KILL");
//    digitalWrite(instance0_->ResPin, LOW);
//  }
//  if (instance0_->CMD == false) {
//    instance0_->CMD = !instance0_->CMD;
//    Serial.println(instance0_->sACK);
//    //Serial.println("[sID1001 rID1000 PK1 ACK]");
//  }
//  else if (instance0_->isWaiting == false) {
//    //Serial.println("[sID1001 rID1000 PK1 BUSY]");
//    Serial.println(instance0_->sBUSY);
//  }
//
//  //interrupts();
//  //instance0_->SetCommand(true);
//}
