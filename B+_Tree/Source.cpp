///A B+ tree is an N-ary tree with a variable but often large number of children per node.
///A B+ tree consists of a root, internal nodes and leaves.
///The root may be either a leaf or a node with two or more children.
///
///A B+ tree can be viewed as a B tree in which each node contains only keys (not key–value pairs), 
///and to which an additional level is added at the bottom with linked leaves.

#include "Bplus_Tree.h"
using namespace std;


int main()
{
	BpTreeHandler<int, string> hnd;
	hnd.getInput();
	return 0;
}