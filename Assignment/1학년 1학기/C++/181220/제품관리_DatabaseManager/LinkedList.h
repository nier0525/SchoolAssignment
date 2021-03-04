#ifndef _CLINKEDLIST_H
#define _CLINKEDLIST_H
#include "Global.h"

template <typename T>
class CNode
{
	T	mData;
	CNode<T>*	mNext;

public:

	CNode<T>():mData(0), mNext(nullptr){}
	CNode(T _data):mData(_data), mNext(0){}
	~CNode(){ delete mData;}

	T GetmData()const{return mData;}
	CNode<T>*  GetmNext()const{return mNext;}

	void SetmData(T _data){ mData=_data;}
	void SetmNext(CNode<T>* _next){ mNext=_next;}
	
};

template <typename T>
class CLinkedList
{


	CNode<T>* mHead;
	CNode<T>* mTail;
	int    mNodeCount;


public:

	CLinkedList<T>();
	~CLinkedList<T>();

	CNode<T>* GetmHead()const{return mHead;}
	CNode<T>* GetmTail()const{return mTail;}
	int GetmNodeCount() const{return mNodeCount;}


	bool Insert(T _data);
	bool Delete(T _data);

};

template <typename T>
CLinkedList<T>::CLinkedList<T>()
{
	mHead=new CNode<T>();
	mTail=mHead;
	mNodeCount=0;
}

template <typename T>
CLinkedList<T>::~CLinkedList<T>()
{
	const CNode<T>* ptr=mHead->GetmNext();
	delete mHead;

	while(ptr!=NULL)
	{
		mHead=const_cast<CNode<T>*>(ptr);
		ptr=ptr->GetmNext();
		delete mHead;
	}
}

template <typename T>
bool CLinkedList<T>::Insert(T _data)
{
	CNode<T>* ptr=new CNode<T>(_data);
	mTail->SetmNext(ptr);
	mTail=ptr;
	return true;
}

template <typename T>
bool CLinkedList<T>::Delete(T _data)
{
	CNode<T>* pre=mHead;
	CNode<T>* ptr=const_cast<CNode<T>*>(pre->GetmNext());

	while(ptr!=NULL)
	{		
		if(ptr->GetmData()==_data)
		{
			pre->SetmNext(const_cast<CNode<T>*>(ptr->GetmNext()));
			if(ptr->GetmNext()==NULL)
			{
				mTail=pre;
			}
			delete ptr;
			break;
		}

		pre=ptr;
		ptr=const_cast<CNode<T>*>(ptr->GetmNext());
		
	}

	return true;

}

#endif