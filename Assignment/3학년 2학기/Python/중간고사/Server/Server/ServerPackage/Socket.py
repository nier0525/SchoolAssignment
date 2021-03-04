import socket

class CSocket:
    # 소켓, 주소 초기화
    def __init__(self, _sock, _addr):        
        self.sock = _sock
        self.addr = _addr        

    # 송신
    def Send(self, data):
        self.sock.sendall(data)

    # 수신
    def Recv(self, size):
        return self.sock.recv(size)    

    def GetSocket(self):
        return self.sock

    def GetAddr(self):
        return self.addr



