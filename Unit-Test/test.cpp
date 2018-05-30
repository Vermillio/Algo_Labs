#include "pch.h"
#include "../BinomialHeap/BinomialHeap.h"
#include "../BinomialHeap/Movie.h"
#include "../BinomialHeap/CsvParser.h"

using namespace heap;

TEST(TestCaseName, BinomialTreeMainTest) {
	int repeat = 1000;
	BinomialHeap<int> heap;

	for (size_t i = 0; i < repeat; ++i)
		heap.insert((int)rand());
	
	ASSERT_EQ(heap.checkCorrect(), 0);

	//for (size_t i = 0; i < repeat / 2; ++i)
	//	heap.remove((size_t)rand() % heap.degree());

}
