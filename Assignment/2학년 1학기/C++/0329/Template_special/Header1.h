
#include <iostream>
using namespace std;

template <typename T>
class temp
{
	T mData;
public:
	temp()
	{		
	}
	~temp()
	{	
	}
	void show()
	{
		cout << mData << " base template!! " << endl;
	}
};

//template <>
//void temp<char*>::show()
//{
//	cout << mData << " full specialized template " << endl;
//}

//============================================================

template <>
class temp<char*>
{
	char* mStr;
public:
	temp(char* _str)
	{
		mStr=new char[strlen(_str)+1];
		strcpy(mStr, _str);
	}
	~temp()
	{
		delete mStr;
	}
	void show()
	{
		cout << mStr << endl;
	}
};

//================================================================
template <typename T>
class temp<T*>
{
	T* mData;
public:
	temp(T* _data)
	{
		mData=new T();
		memcpy(mData, _data, sizeof(T));
	}
	~temp()
	{
		delete mData;
		cout << "memory return" << endl;
	}
	void show()
	{
		cout << "Pointer specialization" << endl;
	}
};