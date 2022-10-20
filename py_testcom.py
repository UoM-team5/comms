import serial
 
port = serial.Serial()
port.baudrate = 115200
port.port = 'COM4'
port.open()
MAX_BUFF_LEN = 255

# read one char (default)
def read_ser(num_char = 1):
	string = port.read(num_char)
	return string.decode('utf-8')

# Write whole strings
def write_ser(cmd):
	port.write(cmd.encode('utf-8'))

# Super loop
while(1):
	print("Enter python input: ")
	cmd = input() # Blocking, there're solutions for this ;)
	if(cmd):
		write_ser(cmd)
	
		
	#string = read_ser(MAX_BUFF_LEN)
	print(string)
	if(len(string)):
		print(string)