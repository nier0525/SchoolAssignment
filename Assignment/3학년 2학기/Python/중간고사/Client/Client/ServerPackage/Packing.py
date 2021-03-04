from ServerPackage import Socket
from Global import *
import struct

class CPacking(Socket.CSocket):
    # 상속 받은 객체 초기화, 프로토콜, 수신 데이터 변수
    def __init__(self):
        Socket.CSocket.__init__(self)
        self.protocol = 0
        self.recvied = 0

    # 전체 사이즈 언패킹
    def size_unpacking(self, buffer):
        unpack = struct.unpack('=i', buffer)
        return unpack[0]

    # 프로토콜 언패킹
    def protocol_unpacking(self, buffer):
        unpack = struct.unpack('=i', buffer)
        return unpack[0]

    #def unpacking(self, buffer):
    #    struct_fmt = str.format('={0}s', (len(buffer)))
    #    unpack = struct.unpack(struct_fmt, buffer)
    #    return unpack   

    # 송신
    def Complete_Send(self, data):
        self.Send(data)       

    # 수신
    def Complete_Recv(self):
        # Size 받기
        self.recvied = self.Recv(4)
        if not self.recvied:
            return False        
        size = self.size_unpacking(self.recvied)

        # Protocol 받기
        self.recvied = self.Recv(4)
        if not self.recvied:
            return False

        self.protocol = self.protocol_unpacking(self.recvied)
        size = size - 4

        # Protocol 만 받은 경우 데이터가 없기 때문에 예외 처리
        if size == 0:
            return True

        # 데이터 받기
        self.recvied = self.Recv(size)
        if not self.recvied:
            return False

        return True