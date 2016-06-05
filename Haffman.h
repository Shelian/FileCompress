#ifndef __HAFFMAN_H__
#define __HAFFMAN_H__
#include <iostream>
using namespace std;
#include "Heap.h"
#include "Compress.h"



template<class T>
class Haffman
{
public:
	typedef BinaryTreeNode<T> Node;
	struct Compare
	{
		bool operator ()( Node*& l, Node*& r)
		{
			return l->_value < r->_value;
		}
	};
public:
	Haffman(T* a,int size,T invalid)
	{
		_root = Create(a,size,invalid);
	}

	Node* GetRoot()
	{
		return _root;
	}

protected:
	Node* Create(T* a, int size,T invalid)
	{
		Heap<Node*,Compare> hp;

		for (int i = 0; i < size; i++)
		{
			if (a[i] != invalid)
			{
				Node* node = new Node(a[i]);
				hp.push(node);
			}
		}

		Node* parent = NULL;
		while (hp.Size()>1)
		{
			Node* left = hp.top();
			hp.pop();
			Node* right = hp.top();
			hp.pop();
			parent = new Node(left->_value + right->_value);
			parent->_left = left;
			parent->_right = right;

			hp.push(parent);
		}

		return parent;
	}


	

protected:
	Node* _root;
};


void testHaffman()
{
	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	Haffman<int> hf(a, 10,10);
}





#endif //__HAFFMAN_H__