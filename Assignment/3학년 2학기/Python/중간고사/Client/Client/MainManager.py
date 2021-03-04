from LoginPackage  import LoginManager
from ServerPackage import ClientInfo

from Global import *
from enum import IntEnum

# MAIN MENU VALUE
class MainMenuValue(IntEnum):
    JOIN = 1
    LOGIN = 2
    EXIT = 3

# LOGIN MENU VALUE
class LoginMenuValue(IntEnum):
    LOGOUT = 1
    REMOVE = 2

# STATE
class MenuState(IntEnum):
    MAIN = 0
    LOGIN = 1
    EXIT = 2

class CMainManager:
    # 클리어언트 정보, 로그인 매니저, 메뉴 상태
    def __init__(self):
        self.client = ClientInfo.CClientInfo()
        self.login = LoginManager.CLoginManager()
        self.state = MenuState.MAIN

    # 메인 메뉴
    def MainMenu(self):
            # 메뉴 선택
            print('')
            print('[ 메뉴 ]')
            print('1. 회원가입')
            print('2. 로그인')
            print('3. 종료')

            num = int(input('선택 : '))
            
            # 회원가입
            if num == MainMenuValue.JOIN:
                # 회원가입 처리
                # 회원가입에서 오류가 발생 시 프로그램 종료
                if self.login.Join(self.client) == None:
                    self.state = MenuState.EXIT

            # 로그인
            elif num == MainMenuValue.LOGIN:
                # 로그인 처리 및 결과값 반환
                result = self.login.Login(self.client)

                # 로그인 성공시 메뉴 상태 전환
                if result == LoginResult.LOGIN_SUCCESS:
                    self.state = MenuState.LOGIN
                # 로그인 처리 중 에러 발생 시 프로그램 종료
                elif result == None:
                    self.state = MenuState.EXIT

            # 종료
            elif num == MainMenuValue.EXIT:
                # 종료 프로토콜 송신
                pack = self.login.protocol_packing(Protocol.EXIT)
                self.client.Complete_Send(pack)

                # 프로그램 종료
                self.state = MenuState.EXIT

    # 로그인 메뉴
    def LoginMenu(self):
            # 메뉴 선택
            print('')
            print('[ 메뉴 ]')
            print('1. 로그아웃')
            print('2. 회원탈퇴')

            num = int(input('선택 : '))

            # 로그아웃
            if num == LoginMenuValue.LOGOUT:
                # 로그아웃 처리 후 메뉴 상태 전환
                self.login.Logout(self.client)
                self.state = MenuState.MAIN

            # 회원탈퇴
            elif num == LoginMenuValue.REMOVE:
                # 회원탈퇴 처리 후 메뉴 상태 전환
                self.login.Remove(self.client)
                self.state = MenuState.MAIN

    # 실행
    def Run(self):
        # 소켓 초기화
        if self.client.SocketSetting() == False:
            print("Socket Setting Error")
            self.client.Close()
            return

        try:
            while True:
                # 현재 메뉴 상태에 맞게 함수 호출
                if self.state == MenuState.MAIN:
                    self.MainMenu()
                elif self.state == MenuState.LOGIN:
                    self.LoginMenu()

                # 메뉴 상태가 종료라면 프로그램 종료
                if self.state == MenuState.EXIT:
                    break
        except:
            print("서버가 비정상적으로 종료되어 프로그램을 종료합니다.")
        finally:                    
            print("Quit")
            self.client.Close()
