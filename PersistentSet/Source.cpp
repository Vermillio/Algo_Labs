#include "PersistentSet.h"
#include "Movie.h"
#include "CsvParser.h"
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>

using namespace std;
using namespace set;

void test_csv(string filename) {
	parse::CsvParser parser;
	vector<vector<string>> df = parser.parse(filename);
	vector<Movie> movies(df.size() - 1);
	PersistentSet_rb<Movie> tree;

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	system("cls");

	int curSize = 0;
	int size = df.size() / 1000;
	for (size_t i = 1; i < size; ++i) {
		if (i % 40 == 0) {
			system("cls");
			cout << "loading data " << (double)i / (double)size * 100 << "%" << endl;
		}
		if (df[i].size() > 9) {
			movies[i - 1] = Movie(df[i][1], atoi(df[i][3].c_str()));
			tree.insert(movies[curSize]);
			++curSize;
		}
	}
	system("cls");
	cout << "loading data " << 100 << "%" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	string choise;
	cout << "Manual test? y - yes, n - no" << endl;

	do {
		cin >> choise;
	} while (choise != "y" && choise != "n");
	if (choise == "y")
		tree.getInput();
	cout << "Test completed" << endl;
}


int main()
{
	test_csv("movie_metadata_new.csv");
	system("pause");
	return 0;
}