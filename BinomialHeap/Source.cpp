#include "BinomialHeap.h"

using namespace std;
using namespace heap;

int main()
{
	size_t TestSize = 100;
	BinomialHeap<int>::TestClass tester;
	size_t errors = tester.run(TestSize);
	if (errors == 0)
		cout << "Tests passed." << endl;
	else cout << "Errors num : " << errors << endl;
	system("pause");
	return 0;
}