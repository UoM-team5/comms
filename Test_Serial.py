import serial
import serial.tools.list_ports
import time

def ID_PORTS_AVAILABLE():
    devices = []
    for port in ['COM%s' % (i + 1) for i in range(256)]:
        try:
            s = serial.Serial(port)
            s.close()
            devices.append(port)
        except (OSError, serial.SerialException):
            pass
    
    return devices

def OPEN_SERIAL_PORTS(DEVS):
    DEVICES = []
    try:
        for i in range(len(DEVS)):
            try:
                Dev = serial.Serial(DEVS[i],baudrate=115200)
                Dev.isOpen()
                DEVICES.append(Dev)
            except IOError:
                Dev.close()
                ev = serial.Serial(DEVS[i],baudrate=115200)
                Dev.isOpen()
                DEVICES.append(Dev)
            except (OSError, serial.SerialException,ValueError):
                pass
        
        return DEVICES
    except:
        return []

def SERIAL_READ_LINE(DEV):
    try:
        Incoming_Data = DEV.readline().decode('UTF-8').replace('\r\n','') 
        return Incoming_Data
    except (NameError,IOError,ValueError):
            pass
    return -1

def SERIAL_WRITE_LINE(DEV,COMMAND):
    try:
        DEV.write(COMMAND.encode('UTF-8'))
        return 1
    except:
        return -1

def WRITE(DEV,COMMAND):
    STATE = -1
    TRY = 0
    while(STATE == -1):
        STATE = SERIAL_WRITE_LINE(DEV,COMMAND)
        TRY = TRY + 1
        if(TRY>10):
            return -1
    STATE = -1
    TRY = 0
    while(STATE == -1):
        STATE = SERIAL_READ_LINE(DEV)
        TRY = TRY + 1
        if(TRY>10):
            return -1
    return STATE

def FLUSH_PORT(DEV):
    try:
        for i in range(len(DEV)):
            DEV[i].flushInput()
    except:
        pass

Current_No = 0

while True:
    OP_Devs = OPEN_SERIAL_PORTS(ID_PORTS_AVAILABLE())

    if len(OP_Devs)>Current_No:
        Current_No = len(OP_Devs)
        while True:
            print("There is ", len(OP_Devs)," devices connected")
            user_input = input("Which device would you like to send a command to: ")
            if user_input == "rescan":
                break
            else:
                dev_num = int(user_input)
            Command = int(input("1 = Device ID, 2 = Temperature reading (enter 1 or 2): "))
            if Command == 1:
                print(WRITE(OP_Devs[dev_num-1],"ID"),'\n')
            if Command == 2:
                print(WRITE(OP_Devs[dev_num-1],"T"),'\n')
    FLUSH_PORT(OP_Devs)