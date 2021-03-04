#ifndef _SPE_HEADER_
#define _SPE_HEADER_ 
#include "Header.h"

template<typename T, int SIZE>
class CStak<T*,SIZE>
{	
	T* mData[SIZE];
	int	  mTop;	
	int   msize;

public:

	CStak();
	~CStak();

	bool IsEmpty()
	{
		if(mTop==0)
			return true;
		else
			return false;
	}

	bool IsFull()
	{
		if(mTop==SIZE)
			return true;
		else
			return false;
	}
	bool Push(T* _data);
	bool Pop(T*& _data);	
};

template<typename T, int SIZE>
CStak<T*,SIZE>::~CStak()
{
	for(int i=0; i<mTop; i++)
	{
		if(mData!=nullptr)
		{
			delete mData[i];
		}
	}
}

template<typename T, int SIZE>
CStak<T*,SIZE>::CStak()
{
	memset(mData, 0, sizeof(mData));
	mTop=0;
	msize=0;

}

template<typename T, int SIZE>
bool CStak<T*,SIZE>::Push(T* _data)
{
	if(IsFull())
		return false;
	else
	{
		cout << "speial template push"<< endl;		
		mData[mTop++]=_data;		
		return true;
	}

}
template<typename T, int SIZE>
bool CStak<T*,SIZE>::Pop(T*& _data)
{
	if(IsEmpty())
		return false;
	else
	{
		cout << "speial template pop"<< endl;
		_data=mData[--mTop];
		return true;
	}

}


#endif