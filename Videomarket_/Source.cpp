#include "../B+_Tree/Bplus_Tree.h"
#include "Movie.h"
#include "CsvParser.h"
#include <vector>
#include "KeyValuePair.h"

using std::vector;

string to_string(string s) {
	return s;
}

void input(int &data) {
	cin >> data;
	while (cin.fail())
	{
		cout << "ERROR -- You did not enter an integer";
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cin >> data;
	}
}


template<class KeyClass, class ValClass>
void fill_tree(BpTree<KeyClass, Movie> &tr, vector<KeyClass> &keys, vector<ValClass> &vals) {
	if (keys.size() != vals.size())
		return;
	for (size_t i = 500; i < 500+keys.size()/500; ++i)
		tr.insert(keys[i], vals[i]);
}


void load_csv(vector<Movie> & movies, string filename, int feature) {
	prsr::CsvParser parser;
	vector<vector<string>> df = parser.parse(filename);
	if (df.size() == 0)
		return;
	movies.resize(df.size() - 1);
	int curSize = 0;
	int size = df.size();
	for (size_t i = 1; i < size; ++i) {
		if (i % 40 == 0) {
			system("cls");
			cout << "loading data " << (double)i / (double)size * 100 << "%" << endl;
		}
		if (df[i].size() > 9) {
			movies[i - 1] = Movie(df[i][1], df[i][2], atoi(df[i][3].c_str()), atof(df[i][4].c_str()), atoi(df[i][5].c_str()), atoi(df[i][6].c_str()), df[i][7], atoi(df[i][8].c_str()), atoi(df[i][9].c_str()));
			movies[i - 1].cmp_by((MovieFeatures)feature);
			++curSize;
		}
	}
	system("cls");
	cout << "loading data " << 100 << "%" << endl;
}

void test_csv(string filename) {
	int feature;

	int capacity;
	do {
		cout << "Enter capacity: ";
		cin >> capacity;
	} while (capacity < 2);

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

	vector<Movie> movies;
	load_csv(movies, filename, feature);

	switch (feature) {
	case 0: {
		vector<string> data;
		for (size_t i = 0; i < movies.size(); ++i) {
			data.push_back(movies[i].getTitle());
		}
		BpTree<string, Movie> tr(capacity);
		fill_tree<string, Movie>(tr, data, movies);
		tr.Test();
		//string
		break;
	}
	case 1: {
		vector<int> data;
		for (size_t i = 0; i < movies.size(); ++i) {
			data.push_back(movies[i].getNumVotedUsers());
		}
		BpTree<int, Movie> tr(capacity);
		fill_tree<int, Movie>(tr, data, movies);
		tr.Test();
		//int
		break;
	}
	case 2: {
		vector<float> data;
		for (size_t i = 0; i < movies.size(); ++i) {
			data.push_back(movies[i].getIMDBScore());
		}
		BpTree<float, Movie> tr(capacity);
		fill_tree<float, Movie>(tr, data, movies);
		tr.Test();
		//float
		break;
	}
	case 3: {
		vector<int> data;
		for (size_t i = 0; i < movies.size(); ++i) {
			data.push_back(movies[i].getDuration());
		}
		BpTree<int, Movie> tr(capacity);
		fill_tree<int, Movie>(tr, data, movies);
		tr.Test();

		//int
		break;
	}
	case 4: {
		vector<int> data;
		for (size_t i = 0; i < movies.size(); ++i) {
			data.push_back(movies[i].getGross());
		}
		BpTree<int, Movie> tr(capacity);
		fill_tree<int, Movie>(tr, data, movies);
		tr.Test();

		//int
		break;
	}
	case 5: {
		vector<int> data;
		for (size_t i = 0; i < movies.size(); ++i) {
			data.push_back(movies[i].getBudget());
		}
		BpTree<int, Movie> tr(capacity);
		fill_tree<int, Movie>(tr, data, movies);
		tr.Test();

		//int
		break;
	}
	case 6: {
		vector<int> data;
		for (size_t i = 0; i < movies.size(); ++i) {
			data.push_back(movies[i].getYear());
		}
		BpTree<int, Movie> tr(capacity);
		fill_tree<int, Movie>(tr, data, movies);
		tr.Test();

		//int
		break;
	}
	}

}


int main() {
	string imdb_dataset_path = "movie_metadata_new.csv";
	test_csv(imdb_dataset_path);
	return 0;
}