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


		void insert(T key);
		void remove(size_t ind);
		T getMin();
		T extractMin();
		bool decreaseKey(size_t ind, T new_val);
		void clear();
		size_t degree()	const { return Size; };
		int checkCorrect();

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
		
		vector<BinomialTree> binTrees;
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
		return binTrees[min_ind].keys[0];
	}

	template<class T>
	inline T BinomialHeap<T>::extractMin()
	{
		size_t min_ind = getMin();
		if (min_ind < 0)
			return T();

		vector<BinomialTree> newTrees;
		size_t chunk_size = binTrees[min_ind].keys.degree() / 2;
		for (size_t cur_degree = binTrees[min_ind].degree; cur_degree > 0; --cur_degree)
		{
			vector<T> tmp = vector<T>(binTrees[min_ind].keys.begin() + chunk_size, binTrees[min_ind].keys.end());
			binTrees[min_ind].keys.erase(binTrees[min_ind].keys.begin() + chunk_size, binTrees[min_ind].keys.end());
			newTrees.push_back(BinomialTree(tmp, cur_degree-1));
			chunk_size >>= 1;
		}

		T min_key = binTrees[min_ind].keys[0];
		binTrees.erase(binTrees.begin() + min_ind);
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
			sum+= pow(2, binTrees[tree_ind].degree);
		} while ((int)ind - sum >= 0);

		size_t key_ind = tree_ind != 0 ? ind % (sum - binTrees[tree_ind].keys.degree()) : ind;
		if (binTrees[tree_ind].keys[key_ind] <= new_val)
			return false;
		binTrees[tree_ind].keys[key_ind] = new_val;
		size_t par = parent(key_ind);
		while (par != key_ind && binTrees[tree_ind].keys[par] > binTrees[tree_ind].keys[key_ind])
		{
			swap(binTrees[tree_ind].keys[par], binTrees[tree_ind].keys[key_ind]); // TODO: CHECK IF SWAP WORKS
			key_ind = par;
			par = parent(key_ind);
		}
		return true;
	}

	template<class T>
	inline void BinomialHeap<T>::clear()
	{
		Size = 0;
		binTrees.clear();
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
		if (binTrees.degree() == 0)
			return -1;
		size_t min_ind = 0;
		for (int i = 0; i < binTrees.degree(); ++i)
		{
			if (binTrees[i].keys[0] < binTrees[min_ind].keys[0])
				min_ind = i;
		}
		return min_ind;
	}

	template<class T>
	inline void BinomialHeap<T>::union_main(vector<BinomialTree> &newTrees)
	{
		if (binTrees.size() == 0)
		{
			binTrees = newTrees;
		}
		else {
			size_t ind_2 = 0;
			size_t ind_1 = 0;
			while (ind_1 < binTrees.size() && ind_2 < newTrees.size())
				if (newTrees[ind_2].degree >= binTrees[ind_1].degree)
				{
					binTrees.insert(binTrees.begin() + ind_1 + 1, newTrees[ind_2]);
					ind_1 += 2;
					++ind_2;
				}
				else
					++ind_1;
			while (ind_2 < newTrees.size())
			{
				binTrees.push_back(newTrees[ind_2]);
				++ind_2;
			}
		}

		if (binTrees.size() <= 1)
			return;

		//merge trees of same degree
		for (size_t i = binTrees.size() - 1; i > 0; --i)
		{
			while (i > 0 && i <= binTrees.size() - 1 && binTrees[i-1].degree == binTrees[i].degree)
				mergeTrees(i-1, i);
		}
	}

	template<class T>
	inline void BinomialHeap<T>::mergeTrees(size_t ind_1, size_t ind_2)
	{
		if (binTrees[ind_1].keys[0] <= binTrees[ind_2].keys[0])	
		{
			binTrees[ind_1].keys.insert(binTrees[ind_1].keys.begin() + binTrees[ind_1].keys.size(), binTrees[ind_2].keys.begin(), binTrees[ind_2].keys.end());
			binTrees[ind_1].degree += 1;
			binTrees.erase(binTrees.begin() + ind_2);
		} else {
			binTrees[ind_2].keys.insert(binTrees[ind_2].keys.begin() + binTrees[ind_2].keys.size(), binTrees[ind_1].keys.begin(), binTrees[ind_1].keys.end());
			binTrees[ind_2].degree += 1;
			binTrees.erase(binTrees.begin() + ind_1); 
		}
	}

	template<class T>
	inline int BinomialHeap<T>::checkCorrect()
	{
		int errors = 0;
		for (size_t i = 0; i < binTrees.size(); ++i)
		{
			if (pow(2, binTrees[i].degree) != binTrees[i].keys.size())
				++errors;
			for (size_t j = 1; j < binTrees[i].keys.size(); ++j)
				if (binTrees[i].keys[parent(j)] > binTrees[i].keys[j])
					++errors;
		}
		return errors;
	}
}