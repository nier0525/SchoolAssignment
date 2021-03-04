from package import LoginManager

class CMainManager:
	LM = LoginManager.CLoginManager()

	def __init__(self):
		pass

	@classmethod
	def LoginMenu(cls):
		if cls.LM.Login() == False:
			return

		Menu = {1 : cls.LM.Logout, 2 : cls.LM.Remove}

		while True:
			print('[ 메뉴 ]')
			print('1. 로그아웃')
			print('2. 탈퇴')
			print('')
			print('입력')

			num = int(input())
			
			try:
				Menu[num]()
			except Exception:
				print('잘못된 선택입니다.')
			
			if num == 1 or num == 2:
				return


	@classmethod
	def Run(cls):
		Menu = {1 : cls.LM.Join, 2 : cls.LoginMenu}

		while True:
			print('[ 메뉴 ]')
			print('1. 회원가입')
			print('2. 로그인')
			print('3. 종료')
			print('')
			print('입력')

			num = int(input())

			if num == 3:
				return
			else:
				try:
					Menu[num]()
				except Exception:
					print('잘못된 선택입니다.')

	def __call__(self):
		self.Run()

