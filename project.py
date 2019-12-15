import struct
f = open("/dev/input/mice", "rb");

def project(data):
	while 1:	
		data = f.read(3)
		i = 0

		mouse,x,y = struct.unpack('3b',data)

		if(y > 10): 
			#print("N")
			return 1
			#print struct.unpack('3b',data)
		elif(y < -10): 
			#print("S")
			return 2
			#print struct.unpack('3b',data)
		elif(x > 10): 
			#print("E")
			return 3
			#print struct.unpack('3b',data)
		elif(x < -10): 
			#print("W")
			return 4
			#print struct.unpack('3b',data)
	
