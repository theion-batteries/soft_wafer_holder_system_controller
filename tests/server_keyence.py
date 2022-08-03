# first of all import the socket library
import socket
from struct import *
# next create a socket object
s = socket.socket()
print("Socket successfully created")

# reserve a port on your computer in our
# case it is 12345 but it can be anything
port = 6555

# Next bind to the port
# we have not typed any ip in the ip field
# instead we have inputted an empty string
# this makes the server listen to requests
# coming from other computers on the network
s.bind(('', port))
print("socket binded to %s" % (port))

# put the socket into listening mode
s.listen(5)
print("socket is listening")

# a forever loop until we interrupt it or
# an error occurs
c, addr = s.accept()
print('Got connection from', addr)
while True:

        # Establish connection with client.

    data = c.recv(1024)
    print(f"received: {data}")
    if data == b'get1':
        a="123.456"
        c.sendall(bytes(a,encoding="utf-8"))
        print(f"sent: {a}")   
        continue
    else:         
        c.send(data)
        print(f"sent: {data}")
        continue
        
    
