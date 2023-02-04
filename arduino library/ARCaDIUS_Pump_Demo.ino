#include "ARCaDIUS_Serial.h"
#include "Valve.h"
#include "Pump.h"

String DeviceDesc = "This is an example string";
int Device_ID = 1001;
int Sender_ID = 1000;
int Num_of_Pumps = 2;
int Num_of_Valves = 2;
int Num_of_Shutter = 2;
int Num_of_Mixer = 1;
int Num_of_Temp = 4;
int Num_of_Bubble = 5;
int ResetPin = 3;

ASerial Device(DeviceDesc, Device_ID, Sender_ID, Num_of_Pumps, Num_of_Valves, Num_of_Shutter, Num_of_Temp, Num_of_Bubble, Num_of_Mixer, ResetPin);
Valve Valve1(9);
Pump P1(5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(1000);
  Device.Start();
  Valve1.setUp();
  P1.setUp();
}

void loop() {
  // Enter the subsystem code in the switch statement, delete all the serial prints.
  if (Device.GotCommand()) {
    switch (Device.GetCommand()) {
      case PUMP: // Enter code for pumping here
        Serial.println("The pump number is: " + (String)Device.getPump());
        Serial.println("The pump volume in mls is: " + (String)Device.getPumpMls());
        Serial.println("The pump direction is: " + (String)Device.getPumpDir());
        switch (Device.getPump()) {
          case 1:
            //P1.set_vol(Device.getPumpMls(),Device.getPumpDir());
            break;
          default:
            break;
        }
        break;
      case MIXER: // Enter code for mixer here
        Serial.println("The mixer number is: " + (String)Device.getMixer());
        Serial.println("The mixer speed is: " + (String)Device.getMixerSpeed());
        Serial.println("The mixer direction is: " + (String)Device.getMixerDir());
        break;
      case VALVE: // Enter code for Valve here
        Serial.println("The valve number is: " + (String)Device.getValve());
        switch (Device.getValve()) {
          case 1:
            if (Device.getValveState()) {
              //Valve1.set_pos1();
              Serial.println("The valve state  is: " + (String)Valve1.get_pos_digital());
            }
            else {
              //Valve1.set_pos0();
              Serial.println("The valve state  is: " + (String)Valve1.get_pos_digital());
            }
            break;
          default:
            break;
        }
        break;
      case SHUTTER: // Enter code for shutter here
        Serial.println("The shutter number is: " + (String)Device.getShutter());
        Serial.println("The shutter position  is: " + (String)Device.getShutterPos());
        break;
      default: // Leave this, its just the default
        break;
    }
    delay(100);
    Device.FinishedCommand();

    // This is how you update sensor values 1 to N, TEMP or BUBBLE 
    Device.updateSensors(TEMP, 1, 123.456);  
  }
}
