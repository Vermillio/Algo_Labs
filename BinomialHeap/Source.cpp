#include "BinomialHeap.h"
#include "Movie.h"
#include "CsvParser.h"
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>

using namespace std;
using namespace heap;

void manualTest(BinomialHeap<Movie> &heap) {
	int choise;
	string doCheckCorrect_s;
	do {
		cout << "Do checkCorrect after every operation? y - yes, n - no" << endl;
		cin >> doCheckCorrect_s;
	} while (doCheckCorrect_s != "y" && doCheckCorrect_s != "n");

	bool doCheckCorrect = doCheckCorrect_s == "y" ? 1 : 0;

	do {
		system("cls");
		cout << "heap size: " << heap.degree() << endl;
		cout << "1 - insert, 2 - remove, 3 - clear, 4 - get min, 5 - extract min, 6 - decrease key, <any num> - exit" << endl;
		cin >> choise;
		switch (choise) {
		case 1: {
			cout << "Enter item:" << endl;
			Movie x;
			cin>>x;
			heap.insert(x);
			cout << "Success" << endl;
			break;
		}
		case 2: {
			cout << "Enter index:" << endl;
			int ind;
			cin >> ind;
			while (ind < 0 || ind >= heap.degree()) {
				cout << "too big or too small value" << endl;
				cin >> ind;
			}
			heap.remove(ind);
			cout << "Success" << endl;
			break;
		}
		case 3: {
			heap.clear();
			cout << "Cleared" << endl;
			break;
		}
		case 4: {
			bool success;
			int index;
			Movie x;
			x = heap.getMin(success, index);
			if (success) {
				cout << " Min : " << endl;
				cout << x;
			}
			else cout << "Error" << endl;
			break;
		}
		case 5: {
			Movie x;
			bool success;
			x = heap.extractMin(success);
			if (success) {
				cout << "Min : " << endl;
				cout << x;
			}
			else cout << "Error" << endl;
			break;
		}
		case 6: {
			int ind;
			cout << "Enter index to decrease:" << endl;
			cin >> ind;
			if (ind < 0 || ind >= heap.degree()) {
				cout << "too big or too small value" << endl;
				continue;
			}
			cout << "Enter new value:" << endl;
			Movie x;
			cin >> x;
			heap.decreaseKey(ind, x);
			cout << "Success" << endl;
			break;
		}
		default: {
			break;
		}
		}
		if (doCheckCorrect) {
			int errors = heap.checkCorrect();
			if (errors == 0) {
				cout << "checkCorrect returned: correct" << endl;
			}
			else cout << "checkCorrect returned: " << errors << "errors" << endl;
		}
		_getch();
	} while (choise > 0 && choise <= 6);
}

void test_csv(string filename) {
	parse::CsvParser parser;
	vector<vector<string>> df = parser.parse(filename);
	vector<Movie> movies(df.size() - 1);
	BinomialHeap<Movie> tree;

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
		manualTest(tree);
	cout << "Test completed" << endl;
}


int main()
{
	test_csv("movie_metadata_new.csv");
	system("pause");
	return 0;
}