#include "DesignManager.h"
#include "../Manager.h"

const char* CDesignManager::mSubjectName[] = {"게임기획기초", "게임시나리오", "레벨디자인"};

CDesignManager::CDesignManager()
{

}

CDesignManager::~CDesignManager()
{
	
}

void CDesignManager::Input()
{
	char name[NAMESIZE];
	char id[IDSIZE];
	int  score;

	cout << "이름:";
	cin >> name;

	sprintf(id, "2017%s%03d", DESIGN_CODE, mStuCount + 1);

	CStudent* ptr = new CStudent(name, id);

	for (int i = 0; i < sizeof(mSubjectName) / sizeof(char*); i++)
	{
		cout << mSubjectName[i] << ":";
		cin >> score;
		ptr->SetSubject(score, i);
	}

	mStudentList[mStuCount++] = ptr;
}

void CDesignManager::Display()
{

	for (int i = 0; i < mStuCount; i++)
	{
		cout << "이름:" << mStudentList[i]->GetName() << endl;
		cout << "학번:" << mStudentList[i]->GetId() << endl;

		for (int j = 0; j < sizeof(mSubjectName) / sizeof(char*); j++)
		{
			cout << mSubjectName[j] << ":";
			cout << mStudentList[i]->GetSubject(j) << endl;
		}

		mStudentList[i]->CalculateGrade();

		cout << "평점:";
		cout << mStudentList[i]->GetGrade() << endl;

	}
}

void CDesignManager::Search()
{
	char id[IDSIZE];
	cout << "학번:";
	cin >> id;

	for (int i = 0; i < mStuCount; i++)
	{
		if (!strcmp(mStudentList[i]->GetId(), id))
		{
			cout << "이름:" << mStudentList[i]->GetName() << endl;
			cout << "학번:" << mStudentList[i]->GetId() << endl;

			for (int j = 0; j < sizeof(mSubjectName) / sizeof(char*); j++)
			{
				cout << mSubjectName[j] << ":";
				cout << mStudentList[i]->GetSubject(j) << endl;
			}

			cout << "평점:";
			cout << mStudentList[i]->GetGrade() << endl;
			break;
		}
	}
}
