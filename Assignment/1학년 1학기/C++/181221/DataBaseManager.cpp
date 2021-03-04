#include "DataBaseManager.h"

CLinkedList<CProduct*>* CDataBaseManager::mFoodLst = nullptr;
CLinkedList<CProduct*>* CDataBaseManager::mWearLst = nullptr;
CLinkedList<CProduct*>* CDataBaseManager::mApplianceLst = nullptr;
CNode<CProduct*>*		CDataBaseManager::mSearchNode = nullptr;

CDataBaseManager::CDataBaseManager()
{	
}

CDataBaseManager::~CDataBaseManager()
{
		
}

void CDataBaseManager::End()
{
	if (mFoodLst != nullptr)
	{
		delete mFoodLst;
	}
	if (mWearLst != nullptr)
	{
		delete mWearLst;
	}
	if (mApplianceLst != nullptr)
	{
		delete mApplianceLst;
	}
}

void CDataBaseManager::SearchEnd()
{ 
	mSearchNode = nullptr; 
}

void CDataBaseManager::Initialize()
{
	mFoodLst = new CLinkedList<CProduct*>();
	mWearLst = new CLinkedList<CProduct*>();
	mApplianceLst = new CLinkedList<CProduct*>();
}

bool CDataBaseManager::InputData(CProduct* _ptr, PRODUCT_CLASS _class)
{
	bool result = false;
	switch (_class)
	{
	case FOOD:		
		result=mFoodLst->Insert(_ptr);
		break;
	case WEAR:
		result=mWearLst->Insert(_ptr);
		break;
	case APPLIANCE:
		result=mApplianceLst->Insert(_ptr);
		break;
	}

	return result;

}


CProduct* CDataBaseManager::SearchData(PRODUCT_CLASS _class)
{
	if (!mSearchNode)
	{
		switch (_class)
		{
		case FOOD:
			mSearchNode = mFoodLst->GetmHead()->GetmNext();
			break;
		case WEAR:
			mSearchNode = mWearLst->GetmHead()->GetmNext();
			break;
		case APPLIANCE:
			mSearchNode = mApplianceLst->GetmHead()->GetmNext();
			break;
		}		
	}
	else
	{
		mSearchNode = mSearchNode->GetmNext();
	}
	
	
	if (mSearchNode == nullptr)
	{
		return nullptr;
	}

	return mSearchNode->GetmData();	
}


bool CDataBaseManager::DeleteData(CProduct* _ptr, PRODUCT_CLASS _class)
{
	CLinkedList<CProduct*>* lstptr = nullptr;

	switch (_class)
	{
	case FOOD:
		lstptr = mFoodLst;
		break;
	case WEAR:
		lstptr = mWearLst;
		break;
	case APPLIANCE:
		lstptr = mApplianceLst;
		break;
	}

	if (mSearchNode!=nullptr &&
		_ptr == mSearchNode->GetmData())
	{
		mSearchNode = mSearchNode->GetmNext();
	}

	return lstptr->Delete(_ptr);
}