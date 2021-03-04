from Global import *
import struct

class CFileIOStreamManager:
    def __init__(self):
        # 패키지 포맷 형식
        self.struct_fmt = '=20s20s'

    # 데이터 불러오기
    def DataLoad(self):
        # 포맷 형식 문자열 크기
        data_len = struct.calcsize(self.struct_fmt)

        print("[ Account Data List]")        
        # 파일 바이너리 읽기 모드로 열기
        with open('UserData.dat', 'rb') as file:
            while True:
                # 포맷 형식의 문자열 만큼 읽기
                buffer = file.read(data_len)
                # 읽을 데이터가 없으면 종료
                if not buffer:
                    break
                else:
                    # 포맷 형식에 맞게 파일 내에 데이터 언패킹
                    data = struct.unpack(self.struct_fmt, buffer)

                    # 공백 제거
                    id = str(data[0].decode()).strip('\x00')
                    pw = str(data[1].decode()).strip('\x00')

                    print("ID : {0} , PW : {1}".format(id, pw))
                    # 유저 리스트에 추가
                    UserList.append(UserData(id, pw))
        print('Data Load Complete')

    # 데이터 저장하기
    def DataSave(self):
        # 파일을 바이너리 쓰기 모드로 열기
        with open('UserData.dat', 'wb') as file:
            for user in UserList:
                # 유저 리스트 내에 있는 유저 데이터를 모두 파일에 쓰기
                packed = struct.pack(self.struct_fmt, user.id.encode(), user.pw.encode())
                file.write(packed)
        print("Data Save Complete")

# 어디에서든 쓸 수 있도록 전역 객체 생성                    
FileManager = CFileIOStreamManager()        
