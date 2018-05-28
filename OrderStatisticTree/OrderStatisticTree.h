#pragma once

/*
Red-Black tree extension including basic statistics functions : selection of an element with n-th smallest value
and finding rank n of given element (reverse operation).
*/

#include "rbTree.h"
#include "CsvParser.h"


using namespace std;

namespace OST
{
	template<typename T, typename compare=less<T>>
	struct has_operator_less {
		template <typename U, U> struct type_check;
		template <typename V> static char(&chk(type_check<bool(const T&), &V::foo>*))[1];
		template <typename  > static char(&chk(...))[2];
		static bool const value = (sizeof(chk<T>(0)) == 1);
	};

	template<class T>
	class OrderStatisticTree : public RbTree<T>
	{
	public:
		using nodep = typename RbTree<T>::nodep;

		//Given n, selects n-th smallest element.
		//Returns -1 if n goes beyond tree size.
		T* select(int N);


		//Given element, gets it's position in ascending order.
		//(order statistic rank)
		//Returns -1 if element is not found.
		int rank(T &el);


		//Inserts array of elements into the tree.
		void insert(T *arr, int sz);
		
		void insert(const T &elem);

		//Simple test function.
		//Generates test data randomly.
		//Returns number of errors.
		int test(int repeat = 50);


		//Standard constructor.
		OrderStatisticTree();

		OrderStatisticTree(const vector<T> &data) {
			for (size_t i = 0; i < data.size(); ++i)
				insert(data[i]);
		};

		//Standard destructor.
		~OrderStatisticTree();

	private:

		//Inner selection procedure.
		//Not for usage.
		T* select(nodep root, int N);

	};
};



template<class T>
T* OST::OrderStatisticTree<T>::select(int n)
{
	if (n < 0 || n > this->TreeSize)
		return nullptr;
	return select(this->root, n);
}

template<class T>
T* OST::OrderStatisticTree<T>::select(typename OST::RbTree<T>::nodep tr, int n)
{
	if (tr == nullptr)
		return nullptr;
	int r = OST::sz<T>(tr->child[_l_]);
	if (r == n)
		return &tr->data;
	if (n < r)
		return select(tr->child[_l_], n);
	else return select(tr->child[_r_], n - (r+1));
}

template<class T>
int OST::OrderStatisticTree<T>::rank(T &t)
{
	nodep p = this->search(t);
	if (!p)
		return -1;
	int r = sz<T>(p->child[_l_]);
	while (p != this->root)
	{
		if (p == p->parent->child[_r_])
			r += sz<T>(p->parent->child[_l_])+1;
		p = p->parent;
	}
	return r;
}

template<class T>
void OST::OrderStatisticTree<T>::insert(T * arr, int sz)
{
	for (int i = 0; i < sz; ++i)
		RbTree<T>::insert(arr[i]);
}

template<class T>
inline void OST::OrderStatisticTree<T>::insert(const T & elem)
{
	RbTree<T>::insert(elem);
}

template<class T>
inline int OST::OrderStatisticTree<T>::test(int repeat)
{
	srand(clock());
	int err_count = 0;
	for (int i = 0; i < repeat; ++i)
	{
		int k = rand() % this->TreeSize;
		T * elem = select(k);
		if (elem == nullptr)
			continue;
		int k_2 = rank(*elem);
		T * elem_2 = select(k_2);
		if (elem_2 == nullptr) {
			++err_count;
		}
		else if (!(*elem == *elem_2))
			++err_count;
	}
	return err_count;
}

template<class T>
OST::OrderStatisticTree<T>::OrderStatisticTree() 
{
	this->TreeSize = 0;
	this->root = nullptr;
}



template<class T>
OST::OrderStatisticTree<T>::~OrderStatisticTree()
{
	this->clear();
}
