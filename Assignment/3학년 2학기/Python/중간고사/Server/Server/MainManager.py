from Global import *
from LoginPackage import LoginManager
from ServerPackage import ListenSocket
from ServerPackage import ClientInfo
from FileIOStream import FileIOStreamManager

from _thread import *
import atexit

class CMainManager:
    # 파일 불러오기, Listen 서버 변수
    def __init__(self):
        FileIOStreamManager.FileManager.DataLoad()
        self.server = ListenSocket.CListenSocket()        
        # 로그인 매니저 객체 생성
        self.login = LoginManager.CLoginManager()

    # 스레드 프로세스 함수
    def ProcessThread(self, sock, addr):               
        # 클라이언트 객체 생성
        client = ClientInfo.CClientInfo(sock, addr)
        print("클라이언트 연결 : " , client.addr[0] , ' : ' , client.addr[1])
        login = self.login

        # 메세지 변수
        msg = ''

        while True:
            try:
                # 데이터 수신
                if client.Complete_Recv() == False:
                    print("Recv Error")
                    break;

                # 프로토콜 변수
                protocol = client.protocol                

                # 회원가입
                if protocol == Protocol.JOIN_INFO:
                    result = login.Join(client)                    

                    # 결과값에 맞는 메세지 준비
                    if result == LoginResult.JOIN_ERROR:
                        msg = JOIN_ERROR_MSG
                    elif result == LoginResult.JOIN_SUCCESS:
                        msg = JOIN_SUCCESS_MSG

                    # 결과값과 메세지 송신
                    pack = login.result_packing(Protocol.JOIN_RESULT, result, msg)
                    client.Complete_Send(pack)

                # 로그인
                elif protocol == Protocol.LOGIN_INFO:
                    result = login.Login(client)

                    # 결과값에 맞는 메세지 준비
                    if result == LoginResult.LOGIN_ID_ERROR:
                        msg = LOGIN_ID_ERROR_MSG
                    elif result == LoginResult.LOGIN_PW_ERROR:
                        msg = LOGIN_PW_ERROR_MSG
                    elif result == LoginResult.LOGIN_OVERLAP_ERROR:
                        msg = LOGIN_OVERLAP_ERROR_MSG
                    elif result == LoginResult.LOGIN_SUCCESS:
                        msg = LOGIN_SUCCESS_MSG

                    # 결과값과 메세지 송신
                    pack = login.result_packing(Protocol.LOGIN_RESULT, result, msg)
                    client.Complete_Send(pack)

                # 종료
                elif protocol == Protocol.EXIT:
                    break

                # 로그아웃
                elif protocol == Protocol.LOGOUT:
                    # 로그아웃 처리
                    login.Logout(client)

                    # 로그아웃 메세지 송신
                    msg = ACCOUNT_LOGOUT_MSG
                    pack = login.msg_packing(Protocol.LOGOUT, msg)
                    client.Complete_Send(pack)

                # 회원탈퇴
                elif protocol == Protocol.REMOVE:
                    # 회원탈퇴 처리
                    login.Remove(client)

                    # 회원탈퇴 메세지 송신
                    msg = ACCOUNT_REMOVE_MSG
                    pack = login.msg_packing(Protocol.REMOVE, msg)
                    client.Complete_Send(pack)

            except:
                print('Error : ', client.addr[0], " : " , client.addr[1])                
                self.Close()
                return

        # 클라이언트가 정상 종료했을 경우 데이터 저장
        FileIOStreamManager.FileManager.DataSave()
        print("클라이언트 종료 : " , client.addr[0] , ' : ' , client.addr[1])    

        # 클라이언트가 사용한 유저 계정 비 로그인 처리
        client.user.state = False
        # 클라이언트 소켓 종료
        client.sock.close()
       
    # 실행 함수
    def Run(self):
        try:
            # Listen 소켓 초기화 검사
            if self.server.Setting() == True:
                print("Server Setting OK")
                while True:
                    print("Accept Wait . . .")
                    # Accept 대기
                    client_sock, client_addr = self.server.Accept()                                
                    # 스레드 생성
                    start_new_thread(self.ProcessThread, (client_sock, client_addr))
        except:
            print("클라이언트 중 강제 종료한 클라이언트가 있어 서버를 종료합니다.")
        finally:
            # 프로그램이 비정상 종료 시에도 파일 저장
            FileIOStreamManager.FileManager.DataSave()

    # 서버 소켓 닫기
    def Close(self):
        self.server.Close()


