#pragma once

#pragma once
#include <iostream>
#include <memory>
#include <limits>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cassert>

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

		size_t size() const;
		Nodep insert(T key);
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

		void merge(Nodep p, Nodep s);
		void cut(Nodep t);
	};

	template<class T>
	bool cmp(typename FibonacciHeap<T>::Nodep &a, typename FibonacciHeap<T>::Nodep &b)
	{
		if (a == nullptr)
			return true;
		if (b == nullptr)
			return false;
		return a->key < b->key;
	}

	template<class T>
	using FibHeap = FibonacciHeap<T>;

	template<class T>
	inline size_t FibonacciHeap<T>::size() const
	{
		return Size;
	}

	template<class T>
	inline typename FibHeap<T>::Nodep FibonacciHeap<T>::insert(T key)
	{
		Nodep newNodep(new Node(key));
		if (!FibTrees)
		{
			min_elem = FibTrees = newNodep;
			newNodep->next = newNodep;
			newNodep->prev = newNodep;
			Size = 1;
			TreeNum = 1;
			return newNodep;
		}
		newNodep->next = FibTrees->next;
		FibTrees->next->prev = newNodep;
		FibTrees->next = newNodep;
		newNodep->prev = FibTrees;
		if (!min_elem || min_elem->key > key)
			min_elem = newNodep;
		++Size;
		++TreeNum;
		return newNodep;
	}

	//!
	template<class T>
	inline bool FibonacciHeap<T>::remove(Nodep p)
	{
		if (!p)
			return false;

		if (Size == 1)
		{
			FibTrees = nullptr;
			min_elem = nullptr;
			TreeNum = 0;
			Size = 0;
			return true;
		}

		decreaseKey(p, -numeric_limits<T>::infinity());
		T min;
		extractMin(min);
		return true;
	}

	template<class T>
	inline bool FibonacciHeap<T>::getMin(T &min)
	{
		if (min_elem == nullptr)
			return false;
		min = min_elem->key;
		return true;
	}

	template<class T>
	inline bool FibonacciHeap<T>::extractMin(T &min)
	{
		if (!min_elem || TreeNum==0)
			return false;

		if (TreeNum == 1)
		{
			min = min_elem->key;
			TreeNum = min_elem->degree;
			if (min_elem->son)
			{
				Nodep p = min_elem->son;
				Nodep new_min = min_elem->son;
				for (int i = 0; i < min_elem->degree; ++i)
				{
					if (p->key < new_min->key)
						new_min = p;
					p->parent = nullptr;
					p = p->next;
				}
				FibTrees = min_elem = new_min;
				--Size;
			}
			else
			{
				min_elem = nullptr;
				FibTrees = nullptr;
				Size = 0;
				TreeNum = 0;
			}
			return true;
		}

		//Remove min element
		if (min_elem->son)
		{
			Nodep t = min_elem->son;
			for (int j = 1; j < min_elem->degree; ++j)
			{
				t->parent = nullptr;
				t = t->next;
			}
			t->parent = nullptr;
			t->next = min_elem->next;
			min_elem->next->prev = t;
			min_elem->prev->next = min_elem->son;
			min_elem->son->prev = min_elem->prev;
			if (FibTrees == min_elem)
				FibTrees = min_elem->son;
		}
		else {
			min_elem->next->prev = min_elem->prev;
			min_elem->prev->next = min_elem->next;
			if (FibTrees == min_elem)
				FibTrees = min_elem->next;
		}

		TreeNum += min_elem->degree - 1;
		--Size;
		min = min_elem->key;

		min_elem = FibTrees;

		Nodep p = FibTrees;
		for (int i = 0; i < TreeNum; ++i)
		{
			if (p->key < min_elem->key)
				min_elem = p;
			if (p->degree == p->next->degree)
			{
				if (p->key > p->next->key)
				{
					if (p != p->next)
					{
						Nodep tmp = p->next;
						merge(p->next, p);
						p = tmp;
						--i;
					}
				}
				else {
					if (p != p->next)
					{
						merge(p, p->next);
						--i;
					}
				}
			} 
			else
				p = p->next;
		}
		
		return true;
	}

	template<class T>
	inline bool FibonacciHeap<T>::decreaseKey(Nodep p, T new_val)
	{
		if (!p || p->key < new_val)
			return false;
		p->key = new_val;
		if (!p->parent || p->key >= p->parent->key)
			return true;
		cut(p);
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
	inline void FibonacciHeap<T>::merge(Nodep p, Nodep s)
	{
		//remove s from FibTrees list
		s->prev->next = s->next;
		s->next->prev = s->prev;

		//insert s into p->sons list
		if (p->son)
		{
			p->son->next->prev = s;
			s->next = p->son->next;
			s->prev = p->son;
			p->son->next = s;
		}
		else
		{
			p->son = s;
			s->next = s;
			s->prev = s;
		}
		s->parent = p;
		++p->degree;

		//update main pointer if needed
		if (FibTrees == s)
			FibTrees = p;
		if (min_elem == s)
			min_elem = p;
		--TreeNum;
	}

	template<class T>
	inline void FibonacciHeap<T>::cut(Nodep t)
	{
		//remove t from it's list
		t->prev->next = t->next;
		t->next->prev = t->prev;

		//add t to FibTrees
		t->next = FibTrees->next;
		t->prev = FibTrees;
		FibTrees->next->prev = t;
		FibTrees->next = t;
		++TreeNum;

		//update t->parent
		Nodep p = t->parent;
		--p->degree;
		t->parent = nullptr;

		if (t->key < min_elem->key)
			min_elem = t;

		if (!p->parent)
			return;

		if (!p->marked)
			p->marked = true;
		else cut(p);
	}

}