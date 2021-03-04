#include "GraphicManager.h"
#include "../Manager.h"

const char* CGraphicManager::mSubjectName[] = { "2D", "3D", "드로잉" };

CGraphicManager::CGraphicManager()
{
	
}

CGraphicManager::~CGraphicManager()
{

}

void CGraphicManager::Input()
{
	char name[NAMESIZE];
	char id[IDSIZE];
	int  score;

	cout << "이름:";
	cin >> name;

	sprintf(id, "2017%s%03d", GRAPHIC_CODE, mStuCount + 1);

	CStudent* ptr = new CStudent(name, id);

	for (int i = 0; i < sizeof(mSubjectName) / sizeof(char*); i++)
	{
		cout << mSubjectName[i] << ":";
		cin >> score;
		ptr->SetSubject(score, i);
	}

	mStudentList[mStuCount++] = ptr;
}

void CGraphicManager::Display()
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

void CGraphicManager::Search()
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
