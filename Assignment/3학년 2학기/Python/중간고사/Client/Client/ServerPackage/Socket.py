from Global import *
import socket

class CSocket:
    def __init__(self):
        pass

    # 소켓 정보 초기화 후, 커넥팅
    def SocketSetting(self):
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.connect((HOST, PORT))
            return True
        except:
            print('Socket Setting Error')
            return False

    def GetSocket(self):
        return self.sock

    # 송신
    def Send(self, data):
        self.sock.sendall(data)

    # 수신
    def Recv(self, size):
        return self.sock.recv(size)

    # 소켓 닫기
    def Close(self):
         self.sock.close()

