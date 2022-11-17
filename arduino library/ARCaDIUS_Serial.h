#ifndef ARCaDIUS_Serial_H
#define ARCaDIUS_Serial_H

#include <Arduino.h>
#include <TimerOne.h>

struct DeviceInfo {
  int test;
};

enum operation {
  PUMP,
  SHUTTER,
  VALVE,
  MIXER
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
    String sACK;
    String sBUSY;
    String DeviceDescription;

    int NumPump;
    int NumValve;
    int NumIrr;
    int NumTemp;
    int NumBubble;
    int NumMixer;
    int intPin;

    enum operation op;

    int pump;
    float pumpValue;
    int pumpDir;

    int valve;
    int valveState;

    int mixer;
    float mixerSpeed;
    bool mixerDir;

    int shutter;
    float shutterPos;

    void Pump();
    void Mixer();
    void Valve();
    void Shutter();
    void readSensors();
    void updateSensors();

    static ASerial * instance0_;

    int SerialrID;
    int SerialsID;
    int PK_Size = 0;
    String Command = "NaN";
    bool isWaiting;
    bool CMD = false;

    String rID = "NaN";
    String sID = "NaN";
    String rPK_Size = "NaN";

    String Data;

    static void serialInterrupt();
    static void reattach();
    void SerialLoop();

    void process();
    void analyse();

  public:
    ASerial(String DeviceDesc, int rID, int sID, int NumPump, int NumValve, int NumIrr, int NumTemp, int NumBubble, int NumMixer, int intPin);

    void print2Serial();
    void FinishedCommand();

    void SetState(bool s);
    bool GetState();

    void SetsID(int ID);
    int GetsID();
    int GetSerialsID();

    void SetrID(int ID);
    int GetrID();
    int GetSerialrID();

    int GetPKSize();

    int GetCommand();
    String GetData() {
      return Data;
    }
    bool GotCommand() {
      delay(1000);
      return CMD;
    }
    void SetCommand(bool v) {
      CMD = v;
    }

    int getPump();
    float getPumpMls();
    bool getPumpDir();

    int getValve();
    bool getValveState();

    int getMixer();
    float getMixerSpeed();
    bool getMixerDir();
    
    int getShutter();
    float getShutterPos();
};
#endif
