from Global import *
from ServerPackage import PackingManager

class CClientInfo(PackingManager.CPackingManager):
    # 상속 받은 객체 초기화, 현재 자신이 접속중인 유저 정보 변수
    def __init__(self, _sock, _addr):
        PackingManager.CPackingManager.__init__(self, _sock, _addr)
        self.user = UserData()    
        
