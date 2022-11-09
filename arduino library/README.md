# Communication “Protocol”



The communications is done on the low lever between the python and Arduino sub modules using serial communications. Each device will be given an ID used to send messages between the modules upon request from the master. The serial communication will be interrupt driven with its replies only, the commands will be stored in the devices serial buffer. Once the device has received a command, it will either acknowledge the packet or send a busy response if it is not ready for the packet.

## Packet Composition:

### __Sent__:

Packet sent from the python script to each Arduino. It consists to 4 main elements, the sender ID, the receiver ID, packet size and then a single command. The packet size is used to determine the length of the command. 

![comms](https://user-images.githubusercontent.com/78451671/200846901-ec4742d7-5fc5-411e-bc36-9bb239b59c6f.png)

Each part of the command is delineated with a space character to make them easier to read as well as easier to decode on the Arduino hardware.

### __Received__:

The received packets are of similar format to that of the sent packet however with a few more additional operators such as __ACK__, __BUSY__ and __ERR___. All of these packets are sent as a reply to a request from the main python script and are only sent as such, there will be no constant writing to the serial lines as this will fill the python buffer. When the Arduino is busy with other operations it will only ever respond with a ACK or BUSY which will be driven by a timer interrupt 40ms after the receiving of a packet. If the Arduino is BUSY it will ignore the latest packet sent and simply reply with a BUSY.

The only time when the Arduino is proactively send a message to the python is when the device encounters an error, then the device will send an error message to the python for it to handle and deal with.

![comms1](https://user-images.githubusercontent.com/78451671/200846973-452f3ce9-af27-4bbf-b198-fa694cbc40d3.png)

### __Other Packets__:

•	[sID1001 rID1000 PK1 ACK]

•	[sID1001 rID1000 PK1 BUSY]

•	[sID1001 rID1000 PK1 ERR0]

  | __ERROR NUMBER__ |	__FAULT__ | 
  | :------------: | -------- | 
  | ERR0	| Incorrect packet format |
  | ERR1	| Packet missing items | 
  | ERR2	| Incorrect Device ID | 
  | ERR3	| Incorrect Sender ID | 
  | ERR4	| System is in error state. | 

## Command Operators:

| __OPERATOR__ | __INFO__ |
| :------------ | -------- | 
| sID____ | 	Sender ID, this has to match on both devices, has to be unique + ID number |
| rID____ |	Receiver ID, this has to match on both devices, has to be unique + ID number |
| PK_ |	Packet size of the number of command operators, not length |
| ACK |	Acknowledgement response |
| BUSY |	Busy response when the device is in operation |
| ERR_ |	Error message based on the devices state |
| R |	Read sensor operator, sent by the python to indicate the reading of all the sensors, all sent back as a packet as shown above. |
| P_ |	Pump operator followed by the number of the pump |
| m__.__ |	number of millilitres to pump, always follows pump command, ml in integer or floating point notation. |
| D_ |	Direction of the pump, 0 pumping away from reactor, 1 pumping into reactor or used for the mixer as 0 clockwise and 1 anticlockwise |
| S_____  |	State operator, This follows after all the following operators to denote the value that they are or the state in which they will take. 0 or 1 for on and off and either a floating point or integer value for all other devices and sensors. |
| V_ |	Valve operator, followed by the number of the valve and then state. |
| I___ |	Shutter operator, followed by the number of the shutter and then state. |
| T_ |	Temperature operator, followed by the number of the sensor and then state which is followed by the temperature reading. |
| B_ |	Bubble sensor operator, followed by the number of the sensor and then state which is followed by the value of the sensor. |
| M_ |	Mixer operator, followed by the number of the mixer, State to specify the speed of the mixer and Direction of the mixer |
| L_ |	Liquid level sensor operator, followed by the number of the sensor and then state which is followed by the value of the sensor. |
