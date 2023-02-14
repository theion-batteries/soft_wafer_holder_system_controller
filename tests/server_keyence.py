import socket

def run_server():
    s = socket.socket()
    print("Socket successfully created")
    val = "MS,01,100\r".encode('utf-8')
    hex_val = ''.join('\\x{:02x}'.format(b) for b in val)

    port = 24687
    s.bind(('', port))
    print("socket binded to %s" % (port))
    s.listen(5)
    print("socket is listening")

    while True:
        try:
            c, addr = s.accept()
            print('Got connection from', addr)

            while True:
                data = c.recv(5012)
                if not data:
                    # Connection lost, break out of inner loop and reconnect
                    print("Connection lost, reconnecting...")
                    c.close()
                    break

                if data == b'MS,01\r':
                    print(f"received: {data}")
                    print(f"sent: {val}")
                    c.send(val)
                if data == b'\x07\x00\x07\x14\x00\x00\x00':
                    print(f"received: {data}")
                    print(f"sent: {hex_val}")
                    #c.send(bytes.fromhex(hex_val.replace('\\x', '')))
                    c.send(b'\x07\x00\x07\x14\x00\x00\x00')
                else:
                    print(f"new data coming: {data}")


            
        except KeyboardInterrupt:
            print("Program stopped by user.")
            break
        except Exception as e:
            print("Error occurred:", e)

    s.close()

if __name__ == "__main__":
    run_server()
