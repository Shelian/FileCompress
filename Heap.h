#ifndef __HEAP_H__
#define __HEAP_H__
#include<iostream>
using namespace std;
#include<assert.h>
int K = 5;
int N = 30;
#include <vector>

template<typename T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& l, const T& r)
	{
		return l>r;
	}
};



template <class T,class compare = less<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(T* a,size_t size)
	{
		size_t index = 0;
		while (index < size)
		{
			_a.push_back(a[index]);
			index++;
		}

		for (int i = (_a.size() - 2) / 2; i >= 0; i--)
			_AdjustDown(i);
	}

	void push(const T& x)
	{
		_a.push_back(x);
		_AdjustUp(_a.size() -1);
	}

	void pop()
	{
		size_t size = _a.size();
		assert(size > 0);
		swap(_a[0], _a[size - 1]);
		_a.pop_back();
		size = _a.size();
		_AdjustDown(0);
	}

	T& top()
	{
		assert(!_a.empty());

		return _a[0];
	}

	bool empty()
	{
		return _a.size() == 0;
	}

	size_t Size()
	{
		return _a.size();
	}

	void Print()
	{
		for (int i = 0; i < _a.size(); i++)
		{
			cout << _a[i] << " ";
		}
		cout << endl;
	}

protected:
	void _AdjustUp(int child)
	{
		int parent = (child - 1) / 2;
		compare com;
		while (child > 0)
		{
			//找出孩子中的最大孩子
			if (com(_a[child] , _a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}

	}

	void _AdjustDown(size_t parent)
	{
		size_t child = 2 * parent + 1;
		compare com;
		while (child < _a.size())
		{
			//找出孩子中的最大孩子
			if (child + 1 < _a.size() && com(_a[child+1] ,_a[child]))
			{
				++child;
			}
			//把
			if (com(_a[child] , _a[parent]))
			{
				swap(_a[parent], _a[child]);
				parent = child;
				child = child * 2 + 1;
			}
			else
			{
				break;
			}
		}

	}
protected:
	vector<T> _a;
};

template<class T, template<class> class compare = Less>
class priorityQueue
{
private:
	Heap<T, compare> _hp; 
public:
	void push(const T& x)
	{
		_hp.push(x);
	}

	void pop()
	{
		_hp.pop();
	}

	T& Top()
	{
		return _hp.top();
	}

	void Print()
	{
		_hp.Print();
	}

};

//redpacked问题
void AdjustDown(int* _a, size_t size,int i)
{
	int parent = i;
	int child = 2 * parent + 1;
	while (child < size)
	{
		//找出孩子中的最小值
		if (child + 1 < size && _a[child + 1] < _a[child])
		{
			++child;
		}
		//与父节点做比较
		if (_a[parent] > _a[child])
		{
			swap(_a[parent], _a[child]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
		{
			break;
		}
	}
}

//找30个数据中的最大五个
int* GetKTop(int* a,size_t size,size_t n)
{
	int* _a = new int[size];
	for (int i = 0; i < size; i++)
	{
		_a[i] = a[i];
	}

	//建堆
	for (int i = (size - 2) / 2; i >= 0; i--)
	{
		AdjustDown(_a, size,i);
	}

	for (int i = 0; i < n - size; i++)
	{
		if (_a[0] < a[size + i])
		{
			_a[0] = a[size + i];
			AdjustDown(_a, size, 0);
		}
	}
	return _a;
}

void _AdjustDown(int* a, size_t size, int i)
{
	int parent = i;
	int child = 2 * parent + 1;
	while (child < size)
	{
		//找出孩子中的最大值
		if (child + 1 < size && a[child] < a[child + 1])
		{
			++child;
		}
		//拿父节点与最大子节点做比较
		if (a[parent] < a[child])
		{
			swap(a[parent], a[child]);
			parent = child;
			child = 2 * parent + 1;
		}
		else
		{
			break;
		}
	}
}
////堆排序->升序排序
//int*& HeapSort(int* a, size_t size)
//{
//	for (int i = (size - 2) / 2; i >= 0; i--)
//	{
//		_AdjustDown(a, size, i);
//	}
//
//	for (int i = 0; i < size; i++)
//	{
//		swap(a[0], a[size - i - 1]);
//		_AdjustDown(a, size - i - 1, 0);
//	}
//
//	return a;
//}




#endif //__HEAP_H__