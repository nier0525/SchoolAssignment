# 유저 데이터
class UserData:    
    def __init__(self, _id = '', _pw = '', _state = False):
        self.id = _id
        self.pw = _pw
        self.state = _state
       
# 로그인 기능 매니저
class CLoginManager:    
    UserList = [UserData('1234','1234'), UserData('kja0204','1234')]
    NowUser = UserData()

    def __init__(self):
        pass

    @classmethod
    def UserSearch(cls):
        count = len(cls.UserList)

        if count == 0:
            return

        current = cls.UserList[0]
        while current != cls.UserList[count - 1]:
            yield current           

    @classmethod
    def UserSearch(cls):
        size = len(cls.UserList)
        if size == 0:
            return

        for index in range(0, size):
            current = cls.UserList[index]
            yield current
        return   

    @classmethod
    def Join(cls):
        try:
            user = UserData()              

            print('ID 를 입력하세요')
            user.id = input()
         
            for data in cls.UserSearch():                
                if user.id == data.id:                
                    raise Exception('사용 할 수 없는 ID 입니다')
                    break

            print('PW 를 입력하세요')
            user.pw = input()

            cls.UserList.append(user)            
            print('회원가입에 성공했습니다')     

        except Exception as err:
            print(err)

    @classmethod
    def Login(cls):
        try:
            user = UserData()

            print('ID 를 입력하세요')
            user.id = input()
            print('PW 를 입력하세요')
            user.pw = input()

            for data in cls.UserSearch():
                if user.id == data.id:
                    if user.pw == data.pw:
                        if data.state == False:
                            cls.NowUser = data
                            return True
                        else:
                            raise Exception('이미 접속중인 계정으로 로그인 할 수 없습니다.')
                    else:
                        raise Exception('PW 가 일치하지 않습니다.')

            raise Exception('없는 계정입니다.')

        except Exception as err:
            print(err)
            return False

    @classmethod
    def Logout(cls):
        cls.NowUser.state = False            

    @classmethod
    def Remove(cls):
        cls.UserList.remove(cls.NowUser)

        

        

        
