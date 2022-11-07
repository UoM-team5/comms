#ifndef ARCaDIUS_Serial_H
#define ARCaDIUS_Serial_H

#include <Arduino.h>




class ASerial {
  private:
    //Flushes the serial port of any unwanted data
    void serialFlush();
    //Checks to see if the receiver ID matches that of the device
    int CheckrID(String rID, int Device_ID);
    //Checks to see if the sender ID matches that of the listener
    int ChecksID(String sID, int Sender_ID);
    //Calculates the command packet size from the serial comms.
    int getPKSize(String rPK_Size);
    //Error handler
    void Error(int code);
  public:


};
#endif
