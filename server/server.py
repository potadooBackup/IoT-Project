import socket
import time

host = '192.168.137.1'
port = 12345

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((host, port))
s.listen(5)

print('server start at: %s:%s' % (host, port))
print('wait for connection...')

while True:
    conn, addr = s.accept()
    print('connected by ' + str(addr))
    while True:
        indata = conn.recv(1024)
        if len(indata) == 0: # connection closed
           conn.close()
           print('client closed connection.')
           break
        echo_data = indata.decode().replace('\r',' ')
        echo_data = indata.decode().split()
        print('---------------------------')
        print('Light sensor:', echo_data[1])
        print('Temperature sensor:', echo_data[2])
        print('Smoke sensor:', echo_data[3])

        #time.sleep(5)
        conn.send('RED'.encode())