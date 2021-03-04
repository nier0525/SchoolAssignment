from Global import *
from FileIOStream import FileIOStreamManager

from ServerPackage import ClientInfo
import struct

class CLoginManager:
    def __init__(self):
        pass

    # 메세지 송신 패킹 함수
    def msg_packing(self, protocol, msg):        
        struct_fmt = str.format('=2i{0}s'.format(len(msg) * 3))
        struct_len = struct.calcsize(struct_fmt) - 4

        pack = struct.pack(struct_fmt, struct_len, protocol, msg.encode('utf-8'))
        return pack        

    # 결과값과 메세지 송신 패킹 함수
    def result_packing(self, protocol, result, msg):        
        struct_fmt = str.format('=3i{0}s'.format(len(msg) * 3))
        struct_len = struct.calcsize(struct_fmt) - 4

        pack = struct.pack(struct_fmt, struct_len, protocol, result, msg.encode('utf-8'))
        return pack        

    # 유저 ID, PW 정보 수신 언패킹 함수
    def user_unpacking(self, buffer):
        struct_fmt = str.format('=2i{0}s', len(buffer) - 8)
        unpack = struct.unpack(struct_fmt, buffer)
        return unpack

    # 언패킹한 정보를 유저 데이터에 맞게 변환
    def BufferToUserData(self, buffer):
        id_len = buffer[0]
        pw_len = buffer[0] + buffer[1]
        data = buffer[2].decode('utf-8')

        id = data[0:id_len]
        pw = data[id_len:pw_len]
        
        return UserData(id, pw)

    # 유저 찾기
    def UserSearch(self):        
        size = len(UserList)
        if size == 0:
            return

        for index in range(0, size):
            current = UserList[index]            
            yield current
        return

    # 회원가입
    def Join(self, client):                
        # 수신 받은 데이터를 유저 데이터로 변환하여 변수에 저장
        user = self.BufferToUserData(self.user_unpacking(client.recvied))

        # 같은 ID 가 이미 존재하는 지 검사
        for data in self.UserSearch():
            if user.id == data.id:
                return LoginResult.JOIN_ERROR

        # 회원가입이 가능한 ID 라면 유저 리스트에 추가
        UserList.append(user)
        return LoginResult.JOIN_SUCCESS

    # 로그인
    def Login(self, client):
        # 수신 받은 데이터를 유저 데이터로 변환하여 변수에 저장
        user = self.BufferToUserData(self.user_unpacking(client.recvied))

        # 로그인 할 계정과 정보가 일치하는 지, 그 계정이 현재 접속중인 지 검사
        for data in self.UserSearch():
            if user.id == data.id:
                if user.pw == data.pw:
                    if data.state == False:
                        # 모든 조건이 충족하면 로그인 성공 결과값 송신 및 계정 접속 상태 변환
                        client.user = data
                        client.user.state = True
                        return LoginResult.LOGIN_SUCCESS
                    else:
                        return LoginResult.LOGIN_OVERLAP_ERROR
                else:

                    return LoginResult.LOGIN_PW_ERROR
        return LoginResult.LOGIN_ID_ERROR

    # 로그아웃
    def Logout(self, client):
        client.user.state = False

    # 회원탈퇴
    def Remove(self, client):
        UserList.remove(client.user)

        