
#ifndef _HEADER_
#define _HEADER_ 
template <typename T>
class CStak
{	
	T     mData[10];
	int	  mTop;	

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
		if(mTop==10)
			return true;
		else
			return false;
	}
	bool Push(T _data);
	bool Pop(T& _data);	
};


template <typename T>
CStak<T>::CStak()
{
	memset(mData, 0, sizeof(mData));
	mTop=0;
}

template <typename T>
CStak<T>::~CStak()
{//	
}

template <typename T>
bool CStak<T>::Push(T _data)
{
	if(IsFull())
		return false;
	else
	{
		mData[mTop++]=_data;
		return true;
	}

}
//
//
template <typename T>
bool CStak<T>::Pop(T& _data)
{
	if(IsEmpty())
		return false;
	else
	{
		_data=mData[--mTop];
		return true;
	}

}

//=================================================================================

template <>
bool CStak<char*>::Pop(char*& _data)
{
	if(IsEmpty()) 
		return false;
	else
	{
		_data=mData[--mTop];
		cout << "Special Template" << endl;
		return true;
	}
}


//=======================================================================================
//=======================================================================================

//
//template <typename T, int SIZE>
//class CStak
//{	
//	T     mData[SIZE];
//	int	  mTop;	
//
//public:
//	
//	CStak();
//	~CStak();
//	bool IsEmpty()
//	{
//		if(mTop==0)
//			return true;
//		else
//			return false;
//	}
//
//	bool IsFull()
//	{
//		if(mTop==SIZE)
//			return true;
//		else
//			return false;
//	}
//	bool Push(T _data);
//	bool Pop(T& _data);	
//};
//
//
//template <typename T, int SIZE>
//CStak<T, SIZE>::CStak()
//{
//	memset(mData, 0, sizeof(mData));
//	mTop=0;
//
//}
//
//template <typename T, int SIZE>
//CStak<T, SIZE>::~CStak()
//{
//}
//
//template <typename T, int SIZE>
//bool CStak<T, SIZE>::Push(T _data)
//{
//	if(IsFull())
//		return false;
//	else
//	{
//		mData[mTop++]=_data;
//		return true;
//	}
//
//}
//
//
//template <typename T, int SIZE>
//bool CStak<T, SIZE>::Pop(T& _data)
//{
//	if(IsEmpty())
//		return false;
//	else
//	{
//		_data=mData[--mTop];
//		return true;
//	}
//
//}

#endif







