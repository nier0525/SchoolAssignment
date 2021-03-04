#include "CInGameManager.h"

CInGameManager::CInGameManager()
{
	mCorrectAnswer = NODATA;
	mAnswerCount = 0;
	mCurrUser = nullptr;
}

CInGameManager::~CInGameManager()
{

}

int	CInGameManager::GetCorrectAnswer()
{
	return mCorrectAnswer;
}
int	CInGameManager::GetAnswerCount()
{
	return mAnswerCount;
}

void CInGameManager::GameStart(CUser* _user)
{
	srand(time(NULL));

	mCorrectAnswer = rand() % 100 + 1;	

	mCurrUser = _user;
}

void CInGameManager::GameOver()
{
	mCorrectAnswer = NODATA;
	mAnswerCount = 0;
	mCurrUser = nullptr;
}

void CInGameManager::GameRun()
{
	cout << "Up Down Game�� �����մϴ�~" << endl;

	while (1)
	{		
		
		cout << ++mAnswerCount << "��° �õ��Դϴ�." << endl;
		
		cout << "1~100������ ���߿� �ϳ��� �Է��ϼ��� :";

		int number;
		cin >> number;

		if (number < 1 || number > 100)
		{
			cout << "�Է¹����� Ʋ�Ƚ��ϴ� �ٽ��Է��ϼ���" << endl;
			mAnswerCount--;
			continue;
		}		

		if (number < mCorrectAnswer)
		{
			cout << "UP!" << endl;
		}
		else if (number > mCorrectAnswer)
		{
			cout << "DOWN!" << endl;
		}
		else
		{
			cout << "Correct Answer!!" << endl;	
			
			if (mCurrUser->GetHighGameScore() == NODATA)
			{
				mCurrUser->SetHighGameScore(mAnswerCount);
			}
			else if(mCurrUser->GetHighGameScore() > mAnswerCount)
			{
				mCurrUser->SetHighGameScore(mAnswerCount);
			}

			cout << "������� ����� �ְ�����:" << mCurrUser->GetHighGameScore()
				<< "�� �õ����� �����Դϴ�" << endl;

			break;
		}

		
	}
	
}