#ifndef ARCaDIUS_Serial_H
#define ARCaDIUS_Serial_H

#include <Arduino.h>

struct DeviceInfo {
  int test;
};


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

    int Device_ID;
    int Sender_ID;
    String DeviceDescription;

    int NumPump;
    int NumValve;
    int NumIrr;
    int NumTemp;
    int NumBubble;
    int NumMixer;


    int SerialrID;
    int SerialsID;
    int PK_Size = 0;
    String Command = "NaN";
    bool isWaiting = true;
    bool CMD = false;

    String rID = "NaN";
    String sID = "NaN";
    String rPK_Size = "NaN";

    String Data;
    
  public:
    ASerial(String DeviceDesc, int rID, int sID, int NumPump, int NumValve, int NumIrr, int NumTemp, int NumBubble, int NumMixer);

    void print2Serial();

    void SerialLoop();

    void process();

    void SetState(bool s);
    bool GetState();

    void SetsID(int ID);
    int GetsID();
    int GetSerialsID();

    void SetrID(int ID);
    int GetrID();
    int GetSerialrID();

    int GetPKSize();

    String GetCommand();
    String GetData(){return Data;}
    bool GotCommand(){return CMD;}
    void SetCommand(bool v){CMD = v;}
};
#endif
