#include "ARCaDIUS_Serial.h"

String DeviceDesc = "This is an example string";
int Device_ID = 1001;
int Sender_ID = 1000;
int Num_of_Pumps = 1;
int Num_of_Valves = 1;
int Num_of_Shutter = 1;
int Num_of_Temp = 1;
int Num_of_Bubble = 1;
int Num_of_Mixer = 1;
int intPin = 2;

ASerial Device(DeviceDesc, Device_ID, Sender_ID, Num_of_Pumps, Num_of_Valves, Num_of_Shutter, Num_of_Temp, Num_of_Bubble, Num_of_Mixer, intPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(1000);
  Device.FinishedCommand();
}

void loop() {
  // Enter the subsystem code in the switch statement, delete all the serial prints.
  if (Device.GotCommand()) {
    switch (Device.GetCommand()) {
      case PUMP: // Enter code for pumping here
        Serial.println("The pump number is: "+ (String)Device.getPump());
        Serial.println("The pump volume in mls is: "+ (String)Device.getPumpMls());
        Serial.println("The pump direction is: "+ (String)Device.getPumpDir());
        break;
      case MIXER: // Enter code for mixer here
        Serial.println("The mixer number is: "+ (String)Device.getMixer());
        Serial.println("The mixer speed is: "+ (String)Device.getMixerSpeed());
        Serial.println("The mixer direction is: "+ (String)Device.getMixerDir());
        break;
      case VALVE: // Enter code for Valve here
        Serial.println("The valve number is: "+ (String)Device.getValve());
        Serial.println("The valve state  is: "+ (String)Device.getValveState());
        break;
      case SHUTTER: // Enter code for shutter here
        Serial.println("The shutter number is: "+ (String)Device.getShutter());
        Serial.println("The shutter position  is: "+ (String)Device.getShutterPos());
        break;
      default: // Leave this, its just the default
        break;
    }
    Device.FinishedCommand();
  }
}
