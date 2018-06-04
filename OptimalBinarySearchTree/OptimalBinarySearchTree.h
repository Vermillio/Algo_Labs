#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>

using namespace std;


namespace tree {



	class OptimalBST {
		int sum(vector<int> freq, int i, int j);
		int optimalCost(vector<int>, int i, int j);
	public:
		int generate(vector<int> freq);
	};

	
	inline int OptimalBST::sum(vector<int> freq, int i, int j)
	{
		int sum = 0;
		for (size_t k = i; k <= j; ++k) {
			sum += freq[k];
		}
		return sum;
	}

	inline int OptimalBST::optimalCost(vector<int> freq, int i, int j)
	{
		// Base cases
		if (j < i)      // no elements in this subarray
			return 0;
		if (j == i)     // one element in this subarray
			return freq[i];

		// Get sum of freq[i], freq[i+1], ... freq[j]
		int fsum = sum(freq, i, j);

		// Initialize minimum value
		int min = numeric_limits<int>::max();

		// One by one consider all elements as root and
		// recursively find cost of the BST, compare the
		// cost with min and update min if needed
		for (int r = i; r <= j; ++r)
		{
			int cost = optimalCost(freq, i, r - 1) +
				optimalCost(freq, r + 1, j);
			if (cost < min)
				min = cost;
		}

		// Return minimum value
		return min + fsum;
	}
	inline int OptimalBST::generate(vector<int> freq)
	{
		return optimalCost(freq, 0, freq.size() - 1);
	}
}