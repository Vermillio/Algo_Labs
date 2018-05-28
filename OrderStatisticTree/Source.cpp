/*
This program implements operations with Order Statistic Tree: selection of an element with n-th smallest value
and finding rank n of given element (reverse operation).
These are the most fundamental tools in non-parametric statistics and inference.
*/

#include "OrderStatisticTree.h"
#include "KeyValuePair.h"
#include "Movie.h"
#include <iostream>
#include <ctime>
#include <conio.h>
#include <chrono>
#include <thread>

using std::cout;
using std::vector;
using std::string;
using namespace OST;

//Outputs brief program usage reference into prompt.
void show_help()
{
	cout << "	---HELP---" << endl
		<< "Usage: APP_PATH <command> [options]" << endl
		<< "	 COMMANDS" << endl
		<< "help									Show help for commands." << endl
		<< "test [array_size], [repets_num]			Run test function (with optional parameters)." << endl
		<< "input									Run program in manual console input mode. (RECOMMENDED)" << endl
		<< "	ERROR CODES" << endl
		<< "-1										Element not found, wrong data pushed as argument or element is not found in tree." << endl
		<< "	OPTIONS" << endl
		<< "array_size								Size of randomly generated test array for building OST." << endl
		<< "repeats_num								Number of operations/checks executed on test data." << endl;
}

//Console data input for manual usage of Order Statistic Tree.
void run_input()
{
	int size;
	cout << "Enter size: ";
	do {
		cout << ">>";
		cin >> size;
	} while (size <= 0);
	OrderStatisticTree<int> tr;
	int *arr = new int[size];
	cout << "Enter elements: ";
	for (int i = 0; i < size; ++i)
	{
		cout << ">>";
		cin >> arr[i];
	}
	tr.insert(arr, size);
	tr.print();
	int choise = 1;
	cout << "1 - select, 2 - rank, OTHER - end" << endl << ">>";
	cin >> choise;
	while (choise > 0 && choise < 3)
	{
		if (choise == 1)
		{
			int x;
			cout << "---SELECT---" << endl;
			cout << "(rank) >>";
			cin >> x;
			int * n = tr.select(x);
			if (n == nullptr)
				continue;
			cout << *n << endl;
		}
		else {
			int n;
			cout << "---RANK---" << endl;
			cout << "(number of entered element) >>";
			cin >> n;
			int x = tr.rank(arr[n]);
			cout << x << endl;
		}
		cin >> choise;
	}
	delete[] arr;
}

//Test tree capabilities and correctness via prompt.
void manualTest(OrderStatisticTree<Movie> &tree) {
	while (true) {
		int choise;
		cout << "0 - Select, 1 - Rank, -1 - Exit: " << endl;
		do {
			cin >> choise;
		} while (choise < -1 || choise > 1);
		if (choise == -1)
			break;
		if (choise == 1) {
			string data;
			cout << "Enter search value: " << endl;
			cin >> data;

		}
		else {
			int n;
			cout << "Enter rank: " << endl;
			cin >> n;
			while (n < 0 || n > tree.TreeSize) {
				system("cls");
				cout << "Out of range. Enter again: " << endl;
				cin >> n;
			}
			Movie *m = tree.select(n);
			if (m == nullptr) {
				cout << "Error. Found nullptr" << endl;
			}
			else {
				cout << m->toString() << endl;
			}
		}
	}
}

//Load data from csv file and optionally do manual test.
void test_csv(string filename) {
	Parse::CsvParser parser;
	vector<vector<string>> df = parser(filename);
	vector<Movie> movies(df.size()-1);
	OrderStatisticTree<Movie> tree;

	int feature;
	do {
		cout << "Choose feature to compare movies" << endl
			<< "0 - title" << endl
			<< "1 - num_voted_users" << endl
			<< "2 - imdb_score" << endl
			<< "3 - duration" << endl
			<< "4 - gross" << endl
			<< "5 - budget" << endl
			<< "6 - year" << endl;
		cout << ">>";
		cin >> feature;
	} while (feature < 0 || feature > 6);


	int curSize = 0;
	int size = df.size() / 4;
	for (size_t i = 1; i < size; ++i) {
		if (i % 40 == 0) {
			system("cls");
			cout << "loading data " << (double)i/(double)size * 100 << "%" << endl;
		}
		if (df[i].size() > 9) {
			movies[i-1]= Movie(df[i][1], df[i][2], atoi(df[i][3].c_str()), atof(df[i][4].c_str()), atoi(df[i][5].c_str()), atoi(df[i][6].c_str()), df[i][7], atoi(df[i][8].c_str()), atoi(df[i][9].c_str()));
			movies[i - 1].cmp_by((MovieFeatures)feature);
			tree.insert(movies[curSize]);
			++curSize;
		}
	}
	system("cls");
	cout << "loading data " << 100 << "%" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	int errors = tree.test();
	cout << "Data test completed. Number of errors = " << errors << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	system("cls");
	string choise;
	cout << "Manual test? y - yes, n - no" << endl;
	do {
		cin >> choise;
	} while (choise != "y" && choise != "n");
	if (choise == "y")
		manualTest(tree);
	cout << "Test completed";
}

//Entry point.
int main(int argc, char **argv)
{
	if (argc == 1)
		show_help();
	else if (strcmp(argv[1], "help") == 0)
		show_help();
	else if (strcmp(argv[1], "test") == 0)
		test_csv("movie_metadata_new.csv");
	else {
		cout << "Unknown command." << endl;
		show_help();
	}
	_getch();
	return 0;
}