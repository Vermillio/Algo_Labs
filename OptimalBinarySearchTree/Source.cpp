#include "OptimalBinarySearchTree.h"

int main() {

	tree::OptimalBST tree;

	vector<int> freq = { 1, 1, 1 };
	int res = tree.generate(freq);
	cout << "Cost = " << res << endl;
	return 0;
}