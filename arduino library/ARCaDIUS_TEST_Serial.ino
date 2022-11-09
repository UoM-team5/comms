#include "ARCaDIUS_Serial.h"
#include <TimerOne.h>

String DeviceDesc = "This is an example string";
int Device_ID = 1001;
int Sender_ID = 1000;
int Num_of_Pumps = 1;
int Num_of_Valves = 1;
int Num_of_Shutter = 1;
int Num_of_Temp = 1;
int Num_of_Bubble = 1;
int Num_of_Mixer = 1;

ASerial Test(DeviceDesc, Device_ID, Sender_ID, Num_of_Pumps, Num_of_Valves, Num_of_Shutter, Num_of_Temp, Num_of_Bubble, Num_of_Mixer);

bool run_1 = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(2), serialInterrupt, FALLING);
  Timer1.initialize(30000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Test.GotCommand()) {
    delay(10000);
    Test.process();
    Test.print2Serial();
    Test.SetState(true);
    Test.SetCommand(false);
    run_1 = false;
  }
}

void serialInterrupt() {
  detachInterrupt(digitalPinToInterrupt(2));
  noInterrupts();
  Test.SerialLoop();
  interrupts();
  Timer1.attachInterrupt(reattach);
}

void reattach() {
  attachInterrupt(digitalPinToInterrupt(2), serialInterrupt, FALLING);
  Timer1.detachInterrupt();
  if (!run_1) {
    run_1 = !run_1;
    Serial.println("[sID1001 rID1000 PK1 ACK]");
  }
  else if(!Test.GetState()){
    Serial.println("[sID1001 rID1000 PK1 BUSY]");
  }
  else {
    Serial.println("[sID1001 rID1000 PK1 ACK]");
    Test.SetState(false);
  }
  Test.SetCommand(true);
}
