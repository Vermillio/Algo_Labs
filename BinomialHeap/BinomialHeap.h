#pragma once
#include <iostream>
#include <memory>
#include <limits>
#include <vector>

using namespace std;

namespace heap {
	template<class T>
	class BinomialHeap
	{
	public:

		BinomialHeap() : Size(0) {};

		class TestClass
		{
		public:
			//Executes test function.
			//Returns errors num.
			size_t run(size_t repeat);
			size_t checkCorrect(BinomialHeap &heap);
		};

		void insert(T key);
		void remove(size_t ind);
		T getMin();
		T extractMin();
		bool decreaseKey(size_t ind, T new_val);
		void clear();
		size_t size()								{ return Size; };

	private:

		struct BinomialTree {
			vector<T> keys;
			size_t degree;
			BinomialTree() {};
			BinomialTree(vector<T> _keys, size_t _degree) : keys(_keys), degree(_degree) {};
		};

		size_t parent(size_t ind);
		size_t getMin_proc();
		void union_main(vector<BinomialTree> &newTrees);
		void mergeTrees(size_t ind_1, size_t ind_2);
		

		size_t Size;
		
		vector<BinomialTree> BinomialTrees;
	};


	template<class T>
	inline void BinomialHeap<T>::insert(T key)
	{
		vector<BinomialTree> newTree;
		newTree.push_back(BinomialTree(vector<T>({key}), 0));
		union_main(newTree);
		++Size;
	}

	template<class T>
	inline void BinomialHeap<T>::remove(size_t ind)
	{
		if (ind >= Size)
			return;
		decreaseKey(ind, -numeric_limits<T>::infinity());
		extractMin();
		--Size;
	}

	template<class T>
	inline T BinomialHeap<T>::getMin()
	{
		size_t min_ind = getMin_proc();
		if (min_ind < 0)
			return T();
		return BinomialTrees[min_ind].keys[0];
	}

	template<class T>
	inline T BinomialHeap<T>::extractMin()
	{
		size_t min_ind = getMin();
		if (min_ind < 0)
			return T();

		vector<BinomialTree> newTrees;
		size_t chunk_size = BinomialTrees[min_ind].keys.size() / 2;
		for (size_t cur_degree = BinomialTrees[min_ind].degree; cur_degree > 0; --cur_degree)
		{
			vector<T> tmp = vector<T>(BinomialTrees[min_ind].keys.begin() + chunk_size, BinomialTrees[min_ind].keys.end());
			BinomialTrees[min_ind].keys.erase(BinomialTrees[min_ind].keys.begin() + chunk_size, BinomialTrees[min_ind].keys.end());
			newTrees.push_back(BinomialTree(tmp, cur_degree-1));
			chunk_size >>= 1;
		}

		T min_key = BinomialTrees[min_ind].keys[0];
		BinomialTrees.erase(BinomialTrees.begin() + min_ind);
		union_main(newTrees);
		return min_key;
	}

	template<class T>
	inline bool BinomialHeap<T>::decreaseKey(size_t ind, T new_val)
	{
		int tree_ind = -1;
		int sum = 0;
		
		do {
			++tree_ind;
			sum+= pow(2, BinomialTrees[tree_ind].degree);
		} while ((int)ind - sum >= 0);

		size_t key_ind = tree_ind != 0 ? ind % (sum - BinomialTrees[tree_ind].keys.size()) : ind;
		if (BinomialTrees[tree_ind].keys[key_ind] <= new_val)
			return false;
		BinomialTrees[tree_ind].keys[key_ind] = new_val;
		size_t par = parent(key_ind);
		while (par != key_ind && BinomialTrees[tree_ind].keys[par] > BinomialTrees[tree_ind].keys[key_ind])
		{
			swap(BinomialTrees[tree_ind].keys[par], BinomialTrees[tree_ind].keys[key_ind]); // TODO: CHECK IF SWAP WORKS
			key_ind = par;
			par = parent(key_ind);
		}
		return true;
	}

	template<class T>
	inline void BinomialHeap<T>::clear()
	{
		Size = 0;
		BinomialTrees.clear();
	}

	template<class T>
	inline size_t BinomialHeap<T>::parent(size_t ind)
	{
		if (ind == 0)
			return 0;
		size_t shift = 1;
		size_t tmp = ind;
		while (tmp % 2 == 0)
		{
			tmp /= 2;
			shift*=2;
		}
		return ind - shift;
	}

	template<class T>
	inline size_t BinomialHeap<T>::getMin_proc()
	{
		if (BinomialTrees.size() == 0)
			return -1;
		size_t min_ind = 0;
		for (int i = 0; i < BinomialTrees.size(); ++i)
		{
			if (BinomialTrees[i].keys[0] < BinomialTrees[min_ind].keys[0])
				min_ind = i;
		}
		return min_ind;
	}

	template<class T>
	inline void BinomialHeap<T>::union_main(vector<BinomialTree> &newTrees)
	{
		if (BinomialTrees.size() == 0)
		{
			BinomialTrees = newTrees;
		}
		else {
			size_t ind_2 = 0;
			size_t ind_1 = 0;
			while (ind_1 < BinomialTrees.size() && ind_2 < newTrees.size())
				if (newTrees[ind_2].degree >= BinomialTrees[ind_1].degree)
				{
					BinomialTrees.insert(BinomialTrees.begin() + ind_1 + 1, newTrees[ind_2]);
					ind_1 += 2;
					++ind_2;
				}
				else
					++ind_1;
			while (ind_2 < newTrees.size())
			{
				BinomialTrees.push_back(newTrees[ind_2]);
				++ind_2;
			}
		}

		if (BinomialTrees.size() <= 1)
			return;

		//merge trees of same degree
		for (size_t i = BinomialTrees.size() - 1; i > 0; --i)
		{
			while (i > 0 && i <= BinomialTrees.size() - 1 && BinomialTrees[i-1].degree == BinomialTrees[i].degree)
				mergeTrees(i-1, i);
		}
	}

	template<class T>
	inline void BinomialHeap<T>::mergeTrees(size_t ind_1, size_t ind_2)
	{
		if (BinomialTrees[ind_1].keys[0] <= BinomialTrees[ind_2].keys[0])	
		{
			BinomialTrees[ind_1].keys.insert(BinomialTrees[ind_1].keys.begin() + BinomialTrees[ind_1].keys.size(), BinomialTrees[ind_2].keys.begin(), BinomialTrees[ind_2].keys.end());
			BinomialTrees[ind_1].degree += 1;
			BinomialTrees.erase(BinomialTrees.begin() + ind_2);
		} else {
			BinomialTrees[ind_2].keys.insert(BinomialTrees[ind_2].keys.begin() + BinomialTrees[ind_2].keys.size(), BinomialTrees[ind_1].keys.begin(), BinomialTrees[ind_1].keys.end());
			BinomialTrees[ind_2].degree += 1;
			BinomialTrees.erase(BinomialTrees.begin() + ind_1); 
		}
	}

	template<class T>
	inline size_t BinomialHeap<T>::TestClass::run(size_t repeat)
	{
		BinomialHeap<T> heap;
		for (size_t i = 0; i < repeat; ++i)
			heap.insert(T(rand()));
		size_t errors = checkCorrect(heap);
		for (size_t i = 0; i < repeat/2; ++i)
			heap.remove((size_t)rand() % heap.size());
		errors += checkCorrect(heap);
		return errors;
	}
	template<class T>
	inline size_t BinomialHeap<T>::TestClass::checkCorrect(BinomialHeap<T> &heap)
	{
		size_t errors = 0;
		for (size_t i = 0; i < heap.BinomialTrees.size(); ++i)
		{
			if (pow(2, heap.BinomialTrees[i].degree) != heap.BinomialTrees[i].keys.size())
				++errors;
			for (size_t j = 1; j <  heap.BinomialTrees[i].keys.size(); ++j)
				if (heap.BinomialTrees[i].keys[heap.parent(j)] > heap.BinomialTrees[i].keys[j])
					++errors;
		}
		return errors;
	}
}