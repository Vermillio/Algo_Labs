#pragma once

#pragma once
#include <iostream>
#include <memory>
#include <limits>
#include <vector>

using namespace std;

namespace heap {
	template<class T>
	class FibonacciHeap
	{
	public:

		FibonacciHeap() : Size(0) {};

		struct FibonacciNode;
		using Node = FibonacciNode;
		using Nodep = shared_ptr<FibonacciNode>;
		struct FibonacciNode {
			T key;
			bool marked;
			size_t degree;
			Nodep prev;
			Nodep next;
			Nodep parent;
			Nodep son;
			FibonacciNode(T _key) : key(_key), degree(0), marked(false), prev(nullptr), next(nullptr), parent(nullptr), son(nullptr) {};
			FibonacciNode(T _key, size_t _degree, bool _marked, Nodep _prev, Nodep _next, Nodep _parent, Nodep _son) : key(_key), degree(_degree), marked(_marked), prev(_prev), next(_next), parent(_parent), son(_son) {};
		};

		class TestClass
		{
		public:
			//Executes test function.
			//Returns errors num.
			size_t run(size_t repeat);
			size_t checkCorrect(FibonacciHeap &heap);
		};

		void insert(T key);
		bool remove(Nodep p);
		bool getMin(T &min);
		bool extractMin(T &min);
		bool decreaseKey(Nodep p, T new_val);
		void clear();
		size_t degree() { return Size; };

	private:

		size_t TreeNum;
		size_t Size;
		Nodep min_elem;
		Nodep FibTrees;
	};


	template<class T>
	inline void FibonacciHeap<T>::insert(T key)
	{
		Nodep newNodep(new Node(key));
		newNodep->next = FibTrees->next;
		FibTrees->next = newNodep;
		++Size;
		++TreeNum;
	}

	//!
	template<class T>
	inline bool FibonacciHeap<T>::remove(Nodep p)
	{
		if (!p)
			return false;
		decreaseKey(ind, -numeric_limits<T>::infinity());
		extractMin();
		--Size;
		return true;
	}

	template<class T>
	inline bool FibonacciHeap<T>::getMin(T &min)
	{
		if (min_elem == nullptr)
			return false;
		min = min_elem->key;
		return true;
//		size_t min_ind = getMin_proc();
//		if (min_ind < 0)
//			return T();
//		return FibTrees[min_ind].keys[0];
	}

	template<class T>
	inline bool FibonacciHeap<T>::extractMin(T &min)
	{
		if (!min_elem || TreeNum==0)
			return false;

		if (TreeNum == 1)
		{
			FibTrees = min_elem->son;
			TreeNum = min_elem->degree - 1;
			--Size;
			min = min_elem->key;
			return true;
		}

		Nodep p = FibTrees;
		Nodep new_min = min_elem->next;

		for (int i = 0; i < TreeNum; ++i)
		{
			if (p == min_elem)
			{
				Nodep t = min_elem->son;
				for (int j = 1; j < min_elem->degree; ++j)
				{
					t->parent = nullptr;
					t = t->next;
				}
				t->parent = nullptr;
				p->prev->next = min_elem->son;
				t->next = p->next;

				if (FibTrees == min_elem)
					FibTrees = min_elem->son;
				TreeNum += min_elem->degree - 1;
				--Size;
				min = min_elem->key;
			}
			else if (p->key < new_min->key)
				new_min = p;
			if (p->degree == p->next->degree)
			{
				if (p->key > p->next->key)
				{
					//remove p from FibTrees list
					p->prev->next = p->next;

					//insert p into p->next as son (2nd place)
					p->next = y->son->next;
					p->next->son->next = p;
					++p->next->degree;

					//update main pointer if needed
					if (FibTrees == p)
						FibTrees = p->next;

					--TreeNum;
				}
				else {
					//remove p->next from FibTrees
					p->next = p->next->next;

					//insert p->next into p as son (2nd place)
					p->next->next = p->son->next;
					p->son->next = p->next;
					++p->degree;

					//update main pointer if needed
					if (FibTrees == p->next)
						FibTrees = p;
					++p->degree;
					--TreeNum;
				}
			}
			p = p->next;
		}

		min_elem = new_min;
		return true;
	}

	template<class T>
	inline bool FibonacciHeap<T>::decreaseKey(Nodep p, T new_val)
	{
		if (!p || p->key < new_val)
			return false;

		p->key = new_val;
		if (!p->parent)
		{
			//insert sons of p instead of p.
			p->prev->next = p->son;
			Nodep t = p->son;
			for (int i = 0; i < p->degree; ++i)
				t = t->next;
			t->next = p->next;
			TreeNum += p->degree - 1;
			return;
		}
		else {

		}
		//cut node from parent
		//if parent is not root
		//	if parent->marked == true
		//		cut parent
		//	else parent->marked = true
		//

		return true;
	}

	template<class T>
	inline void FibonacciHeap<T>::clear()
	{
		Size = 0;
		TreeNum = 0;
		FibTrees = nullptr;
	}


	template<class T>
	inline size_t FibonacciHeap<T>::TestClass::run(size_t repeat)
	{
		FibonacciHeap<T> heap;
		for (size_t i = 0; i < repeat; ++i)
			heap.insert(T(rand()));
		size_t errors = checkCorrect(heap);
		for (size_t i = 0; i < repeat / 2; ++i)
			heap.remove((size_t)rand() % heap.degree());
		errors += checkCorrect(heap);
		return errors;
	}

	template<class T>
	inline size_t FibonacciHeap<T>::TestClass::checkCorrect(FibonacciHeap<T> &heap)
	{
		size_t errors = 0;
		for (size_t i = 0; i < heap.TreeNum; ++i)
		{
//			if (pow(2, heap.FibTrees[i].degree) != heap.FibTrees[i].keys.degree())
//				++errors;
//			for (size_t j = 1; j < heap.FibTrees[i].keys.degree(); ++j)
//				if (heap.FibTrees[i].keys[heap.parent(j)] > heap.FibTrees[i].keys[j])
//					++errors;
		}
		return errors;
	}
}