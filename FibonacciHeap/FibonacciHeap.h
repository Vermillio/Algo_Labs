#pragma once

#pragma once
#include <iostream>
#include <memory>
#include <limits>
#include <ctime>
#include <algorithm>
#include <vector>
#include <cassert>
#include <conio.h>

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
		struct FibonacciNode : public std::enable_shared_from_this<Node>
		{
			T key;
			bool marked;
			size_t degree;
			Nodep prev;
			Nodep next;
			Nodep parent;
			Nodep son;
			FibonacciNode(T t_key) : key(t_key), degree(1), marked(false), prev(nullptr), next(nullptr), parent(nullptr), son(nullptr) {};
			FibonacciNode(T t_key, size_t t_degree, bool t_marked, Nodep t_prev, Nodep t_next, Nodep t_parent, Nodep t_son) : key(t_key), degree(t_degree), marked(t_marked), prev(t_prev), next(t_next), parent(t_parent), son(t_son) {};
			~FibonacciNode() {
				prev = next = parent = son = nullptr;
			}

			void rmFromList() {
				next->prev = prev;
				prev->next = next;

				if (parent) {
					if (parent->son.get() == this) {
						if (parent->degree == 2) {
							parent->son = nullptr;
						}
						else {
							parent->son = parent->son->next;
						}
					}
					--parent->degree;
				}

				parent = nullptr;
				next = prev = getSharedFromThis();
			}

			Nodep getSharedFromThis() { return this->shared_from_this(); }

			void addSon(Nodep p) {
				if (!p)
					return;
				if (!son) {
					p->parent = getSharedFromThis();
					if (p) {
						son = p;
					}
					else {
						son = nullptr;
						p->next = nullptr;
						p->prev = nullptr;
					}
					++degree;
				}
				else {
					son->addAfter(p);
				}
			}

			void addAfter(Nodep p) {

				next->prev = p;
				p->next = next;
				Nodep tmp = getSharedFromThis();
				//std::weak_ptr<Node> weakPtr(tmp);
				//std::shared_ptr<int> ptr_2 = weakPtr.lock();
				p->prev = tmp;
				next = p;
				if (p->parent) {
					parent = p->parent;
					parent->degree++;
				}
			}
		};

		size_t size() const { return Size; };
		Nodep insert(T key);
		bool remove(Nodep p);
		bool getMin(T &min);
		bool extractMin(T &min);
		bool decreaseKey(Nodep p, T new_val);
		void clear();
		size_t degree() { return TreeNum; };
		bool test();

		void input();
	private:

		int realDeg(Nodep t) {
			if (!t)
				return 0;
			if (!t->son)
				return 1;
			Nodep p = t->son, r = t->son->next;
			int d = 2;
			while (r != p) {
				r = r->next;
				++d;
				if (d > 100) {
					int warning = d;
				}
			}
			return d;
		}

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
	inline typename FibHeap<T>::Nodep FibonacciHeap<T>::insert(T key)
	{
		Nodep newNodep=std::make_shared<Node>(key);
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

		T new_val(std::numeric_limits<int>::min());

		decreaseKey(p, new_val);
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

		int real = realDeg(min_elem);
		if (min_elem->degree != real) {
			int warning = real;
		}


		if (TreeNum == 1)
		{
			min = min_elem->key;
			TreeNum = min_elem->degree-1;
			if (min_elem->son)
			{
				Nodep p = min_elem->son;
				Nodep new_min = min_elem->son;
				for (int i = 0; i < min_elem->degree-1; ++i)
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

		real = realDeg(min_elem);
		if (min_elem->degree != real) {
			int warning = real;
		}


		//Remove min element
		if (min_elem->son)
		{
			Nodep t = min_elem->son->next;
			while (t!=min_elem->son) {
				Nodep p = t->next;
				cut(t);
				t = p;
			}
			cut(t);

			int real = realDeg(min_elem);
			if (min_elem->degree != real) {
				int warning = real;
			}
		}
		if (FibTrees == min_elem)
			FibTrees = min_elem->next;

		min_elem->rmFromList();
		--Size;
		min = min_elem->key;

		if (atoi(FibTrees->key.getVoted().c_str()) == std::numeric_limits<int>::min()) {
			int warning = atoi(min_elem->key.getVoted().c_str());
		}
		min_elem = FibTrees;

		int times_merged = 0;
		Nodep p = FibTrees;
		for (int i = 0; i < TreeNum; ++i)
		{
			real = realDeg(p);
			if (p->degree != real) {
				int warning = real;
			}
			if (p->degree == p->next->degree && p!=p->next)
			{
				TreeNum--;
				if (p->key > p->next->key)
				{
					Nodep tmp = p->next;
					merge(p->next, p);
					++times_merged;
					p = tmp;
					--i;
				}
				else {
					merge(p, p->next);
					++times_merged;
					--i;
				}
			} 
			else
				p = p->next;
			if (p->key < min_elem->key)
				min_elem = p;
		}

		real = realDeg(min_elem);
		if (min_elem->degree != real) {
			int warning = real;
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
		if (new_val < min_elem->key) {
			min_elem = p;
			FibTrees = p;
		}
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
		
		s->rmFromList();
		
		p->addSon(s);

		//update main pointer if needed
		if (p) {
			if (FibTrees == s)
				FibTrees = p;
			if (min_elem == s)
				min_elem = p;
		}
	}

	template<class T>
	inline void FibonacciHeap<T>::cut(Nodep t)
	{
		Nodep p = t->parent;
		//remove t from it's list
		t->rmFromList();

		//add t to FibTrees
		FibTrees->addAfter(t);

		if (t->key < min_elem->key) {
			FibTrees=min_elem = t;
		}

		if (!p || p->parent)
			return;
		if (!p->marked)
			p->marked = true;
		else cut(p);

		++TreeNum;
	}

	template<class T>
	inline void FibonacciHeap<T>::input() {
		int choise;
		vector<Nodep> inserted_nodes;
		do {
			system("cls");
			cout << "1 - insert, 2 - remove, 3 - clear, 4 - get min, 5 - extract min, 6 - decrease key, <any num> - exit" << endl;
			cin >> choise;
			switch (choise) {
			case 1: {
				T x;
				cin >> x;
				inserted_nodes.push_back(insert(x));
				cout << "Success" << endl;
				break;
			}
			case 2: {
				T v;
				cin >> v;
				int index = -1;
				for (size_t i = 0; i < inserted_nodes.size(); ++i) {
					if (inserted_nodes[i]->key == v) {
						index = i;
					}
				}
				if (index == -1) {
					cout << "Not found" << endl;
					break;
				}
				remove(inserted_nodes[index]);
				cout << "Success" << endl;
				break;
			}
			case 3: {
				clear();
				cout << "Cleared" << endl;
				break;
			}
			case 4: {
				T x;
				getMin(x);
				cout << " Min : " << endl;
				cout << x;
				break;
			}
			case 5: {
				T x;
				extractMin(x);
				cout << "Min : " << endl;
				cout << x;
				break;
			}
			case 6: {
				T x;
				cin >> x;
				int index = -1;
				for (size_t i = 0; i < inserted_nodes.size(); ++i) {
					if (inserted_nodes[i]->key == x) {
						index = i;
					}
				}
				if (index == -1) {
					cout << "Not found" << endl;
					break;
				}
				int v;
				cin >> v;
				decreaseKey(inserted_nodes[index], v);
				cout << "Success" << endl;
				break;
			}
			default: {
				break;
			}
			}
			_getch();
		} while (choise > 0 && choise <= 6);
	}

	template<class T>
	inline bool FibonacciHeap<T>::test()
	{
		clear();
		int repeat = 100;
		srand(clock());
		vector<Nodep> inserted;
		for (size_t i = 0; i < repeat; ++i) 
		{
			T cur(i);
			Nodep new_node=insert(cur);
			inserted.push_back(new_node);
		}
		for (size_t i = 0; i < inserted.size(); ++i)
		{
			remove(inserted[i]);
		}
		return size()== 0;
	}

}