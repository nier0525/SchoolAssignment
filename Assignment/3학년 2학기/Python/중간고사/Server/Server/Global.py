from enum import IntEnum

# 유저 데이터
class UserData:
    def __init__(self, _id = '', _pw = '', _state = False):
        # self.account = {_id : _pw}

        self.id = _id;
        self.pw = _pw;
        self.state = _state;
             
# ClientList = []
# 유저 리스트
UserList = []

# 호스트 IP
HOST = '127.0.0.1'
# 포트 번호
PORT = 9050

#PROTOCOL
class Protocol(IntEnum):
    JOIN_INFO = 1
    JOIN_RESULT = 2
    LOGIN_INFO = 3
    LOGIN_RESULT = 4
    EXIT = 5

    LOGOUT = 6
    REMOVE = 7


# LOGINRESULT
class LoginResult(IntEnum):
    JOIN_ERROR = 1
    JOIN_SUCCESS = 2

    LOGIN_ID_ERROR = 1
    LOGIN_PW_ERROR = 2
    LOGIN_OVERLAP_ERROR = 3
    LOGIN_SUCCESS = 4

    ACCOUNT_LOGOUT = 1
    ACCOUNT_REMOVE = 2

# MSG DEFINE
JOIN_ERROR_MSG = '사용 할 수 없는 ID 입니다'
JOIN_SUCCESS_MSG = '회원가입에 성공했습니다'

LOGIN_ID_ERROR_MSG = '없는 ID 입니다'
LOGIN_PW_ERROR_MSG = 'PW 가 일치하지 않습니다.'
LOGIN_OVERLAP_ERROR_MSG = '이미 접속중인 계정 입니다.'
LOGIN_SUCCESS_MSG = '로그인에 성공했습니다.'

ACCOUNT_LOGOUT_MSG = '로그아웃 했습니다.'
ACCOUNT_REMOVE_MSG = '회원탈퇴 했습니다.'