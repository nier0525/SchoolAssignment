#ifndef _HEADER2_H
#define _HEADER2_H


template <typename T1, typename T2 , int a>
class temp
{
public:
	void show()
	{
		cout << " base template!! " << endl;
	}
};

template <typename T>
class temp<T, T, 3>
{
	public:
	void show()
	{
		cout << " T,T template!! " << endl;
	}
};

template <typename T>
class temp<T,int, 3>
{
	public:
	void show()
	{
		cout << " T,int template!! " << endl;
	}
};

template <typename T>
class temp<int,T, 3>
{
	public:
	void show()
	{
		cout << " int,T template!! " << endl;
	}
};


template <int a>
class temp<double, int, a>
{
	public:
	void show()
	{
		cout << " double, int, non-type template!! " << endl;
	}
};


template <typename T,int a>
class temp<double, T, a>
{
	public:
	void show()
	{
		cout << " double, T, non-type template!! " << endl;
	}
};



#endif