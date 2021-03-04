from Global import *
from ServerPackage import ClientInfo
import struct

class CLoginManager:
    def __init__(self):
        pass

    # 프로토콜 송신 데이터 패킹
    def protocol_packing(self, protocol):
        struct_fmt = str.format('=2i')
        struct_len = struct.calcsize(struct_fmt) - 4

        pack = struct.pack(struct_fmt, struct_len, protocol)
        return pack

    # 유저 정보 송신 데이터 패킹
    def user_packing(self, protocol, id, pw):    
        struct_fmt = str.format('=2i2i{0}s'.format(len(id) + len(pw)))
        struct_len = struct.calcsize(struct_fmt) - 4

        # 전체사이즈, 프로토콜, ID 사이즈, PW 사이즈, ID,PW 데이터 로 구성
        pack = struct.pack(struct_fmt, struct_len, protocol, len(id), len(pw), (id.encode('utf-8') + pw.encode('utf-8')))
        return pack

    # 메세지 수신 데이터 언패킹
    def msg_unpacking(self, buffer):
        struct_fmt = str.format('={0}s' , len(buffer))
        unpack = struct.unpack(struct_fmt, buffer)
        return unpack    

    # 결과값, 메세지 수신 데이터 언패킹
    def result_unpacking(self, buffer):
        struct_fmt = str.format('=i{0}s' , len(buffer) - 4)
        unpack = struct.unpack(struct_fmt, buffer)
        return unpack    

    # 회원 가입
    def Join(self, client):
        # ID, PW 입력
        print("")
        id = input("ID : ")
        pw = input("PW : ")
        
        # ID,PW 정보 패킹 후 송신
        pack = self.user_packing(Protocol.JOIN_INFO, id, pw)
        client.Complete_Send(pack)

        # 데이터 수신
        if client.Complete_Recv() == False:
            print('Recv Error')
            return None

        # 결과값과 메세지 언패킹
        buffer = self.result_unpacking(client.recvied)
        result = buffer[0]
        msg = buffer[1].decode('utf-8')

        # 출력 후 결과값 반환
        print(msg)
        return result

    # 로그인
    def Login(self, client):
        # ID, PW 입력
        print("")
        id = input("ID : ")
        pw = input("PW : ")
        
        # ID,PW 정보 패킹 후 송신
        pack = self.user_packing(Protocol.LOGIN_INFO, id, pw)
        client.Complete_Send(pack)

        # 데이터 수신
        if client.Complete_Recv() == False:
            print('Recv Error')
            return None

        # 결과값과 메세지 언패킹
        buffer = self.result_unpacking(client.recvied)
        result = buffer[0]
        msg = buffer[1].decode('utf-8')

        # 출력 후 결과값 반환
        print(msg)
        return result

    # 로그아웃
    def Logout(self, client):
        # 로그아웃 프로토콜 송신
        pack = self.protocol_packing(Protocol.LOGOUT)
        client.Complete_Send(pack)

        # 데이터 수신
        if client.Complete_Recv() == False:
            print('Recv Error')
            return None

        # 메시지 언패킹 후 출력
        buffer = self.msg_unpacking(client.recvied)
        msg = buffer[0].decode('utf-8')
        print(msg)

    # 회원탈퇴
    def Remove(self, client):
        # 회원탈퇴 프로토콜 송신
        pack = self.protocol_packing(Protocol.REMOVE)
        client.Complete_Send(pack)

        # 데이터 수신
        if client.Complete_Recv() == False:
            print('Recv Error')
            return None

        # 메세지 언패킹 후 출력
        buffer = self.msg_unpacking(client.recvied)
        msg = buffer[0].decode('utf-8')
        print(msg)
