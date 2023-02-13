import socket

def run_server():
    s = socket.socket()
    print("Socket successfully created")
    settings = "$0=10 \
                $1=255 \
                $2=0\
                $3=0\
                $4=0\
                $5=0\
                $6=0\
                $10=1 \
                $11=0.010\
                $12=0.002\
                $13=0\
                $20=1\
                $21=0\
                $22=1\
                $23=0\
                $24=25.000\
                $25=500.000\
                $26=250 \
                $27=2.000 \
                $30=1000 \
                $31=0 \
                $32=0 \
                $100=400.000 \
                $101=200.000 \
                $102=10.000\
                $110=800.000\
                $111=500.000\
                $112=10.000\
                $120=50.000\
                $121=10.000\
                $122=10.000\
                $130=130.000\
                $131=10.000\
                $132=10.000\
                ok".encode('utf-8')
    positions = "<Idle|MPos:-100.000,0.000,0.000|FS:0,0|WCO:0.000,0.000,0.000>\
                ok".encode('utf-8')
    port = 8882
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

                if data == b'$$\r\n':
                    print(f"received: {data}")
                    print(f"sent: {settings}")
                    c.send(settings)
                elif data == b'?\r\n':         
                    print(f"received: {data}")
                    print(f"sent: {positions}")
                    c.send(positions)
                elif data == b'$X\r\n':         
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')
                elif data == b'$H\r\n':         
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')
                elif data == b'X130\r\n':         
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')
                elif data == b'X100\r\n':         
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')
                elif data == b'-X100\r\n':         
                    print(f"received: {data}")
                    print(f"sent: ok")
                    c.send(b'ok')
                 
        except KeyboardInterrupt:
            print("Program stopped by user.")
            break
        except Exception as e:
            print("Error occurred:", e)

    s.close()

if __name__ == "__main__":
    run_server()
