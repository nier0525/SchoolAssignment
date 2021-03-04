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
	cout << "Up Down Game을 시작합니다~" << endl;

	while (1)
	{		
		
		cout << ++mAnswerCount << "번째 시도입니다." << endl;
		
		cout << "1~100까지의 수중에 하나를 입력하세요 :";

		int number;
		cin >> number;

		if (number < 1 || number > 100)
		{
			cout << "입력범위가 틀렸습니다 다시입력하세요" << endl;
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

			cout << "현재까지 당신의 최고성적은:" << mCurrUser->GetHighGameScore()
				<< "번 시도만에 정답입니다" << endl;

			break;
		}

		
	}
	
}