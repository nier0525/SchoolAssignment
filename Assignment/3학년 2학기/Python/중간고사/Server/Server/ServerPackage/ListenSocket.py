from Global import *
import socket

class CListenSocket:       
    def __init__(self):
        pass

    # 소켓 초기화 후, 바인딩, Listen 소켓 준비
    def Setting(self):
        try:            
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.bind((HOST, PORT))            

            self.sock.listen(100)
            return True
        except:
            print('Socket Setting Error')
            return False
    # Accept 
    def Accept(self):
        return self.sock.accept()

    # 소켓 닫기
    def Close(self):
        self.sock.close()


