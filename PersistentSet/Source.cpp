#include "PersistentSet.h"
#include <memory>
using namespace std;
using namespace pset;

//template<class T>
//using  nodep<T> = shared_ptr<PSrb<T>::node>;

int main()
{
//	nodep<int> ptr(new PSrb<int>::node(5));
	PersistentSet_rb<int>::InputHandler hndlr;
	hndlr.getInput();
	return 0;
}