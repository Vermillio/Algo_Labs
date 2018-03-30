#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\FibonacciHeap\FibonacciHeap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace heap;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestInsertRemove)
		{
			int repeat = 100;
			srand(clock());
			FibHeap<int> heap;
			vector<FibHeap<int>::Nodep> inserted;
			for (size_t i = 0; i < repeat; ++i)
				inserted.push_back(heap.insert(int(rand())));
			for (size_t i = 0; i < inserted.size(); ++i)
			{
				heap.remove(inserted[i]);
			}
			Assert::AreEqual((int)heap.size(), 0, L"Heap size is not 0 after removing elements.");
			heap.clear();
		}

	};
}