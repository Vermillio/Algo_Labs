//This program implements Splay tree, which is self-balancing tree.
//A splay tree is a self-adjusting binary search tree with the additional property that recently accessed elements are quick to access again.

#include "LexicographicSearchTree.h"

using namespace std;
using namespace Tree;

void test() {
	LexicographicSearchTree tree;
	string dictFilename;
	string imageFilename;

	int choise;
	cout << "Test Lexicographic Search Tree" << endl;
	cout << "0 - standard dictionary, 1 - reduced dictionary (tree images will be printed)" << endl;
	do cin >> choise;
	while (choise < 0 && choise > 1);
	
	if (!choise) {
		dictFilename = "dict.txt";
		imageFilename = "dict";
	}
	else {
		dictFilename = "dict_reduced.txt";
		imageFilename = "dict_reduced";
	}
	system("cls");
	int missed = tree.loadDict(dictFilename);
	cout << "Words skipped : " << missed << endl;
	cout << "Press any key to continue" << endl;
	_getch();
	tree.input(imageFilename);
}

int main()
{
	test();
	system("pause");
	return 0;
}