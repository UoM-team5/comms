# communication
These programs facilitates communication between python and arduino

## Python comm

Set of functions that may be called from the UI.
| Function name  | Description | return |
| ------------- | ------------- | ------------- |
| `ID_PORTS_AVAILABLE()` | get port ID connected to arduinos | devices |
| `OPEN_SERIAL_PORTS(DEVS)` | open Serial comm | DEVICES |
| `SERIAL_READ_LINE(DEV)` | ard 2 py, read line via Serial | Incoming_Data |
| `SERIAL_WRITE_LINE(DEV,COMMAND)` | py 2 ard, write line via Serial  |  |
| `WRITE(DEV,COMMAND)` | py 2 ard, write to ard IF not occupied (10 tries) | |
| `FLUSH_PORT(DEV)` | terminate communiaction | |

#### example code of a function: 
```
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
```
