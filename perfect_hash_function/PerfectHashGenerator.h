#pragma once

#define FNV_32_PRIME 0x01000193
#include "CsvParser.h"
#include <map>
#include <list>
#include <set>
#include <algorithm>
#include <utility>

using namespace std;

namespace hshlib
{

//	template<class T>
//	ifstream& operator>>(ifstream &f, map<char*, T> x);

//	template<class T>
//	ofstream& operator<<(ofstream &f, map<char*, T> x);

	template<class T>
	class PerfectHashing
	{
	public:
		void create(map<string, T> &dict);
		void create(string filename, map<string, T> &dict);
		T lookUp(string word);

	private:
		vector<T> values;
		vector<int> D;
		int hash(int d, string word, int s);
	};

	int cmp(vector<string> &x, vector<string> &y);

//	template<class T>
//	ifstream & operator>>(ifstream & f, map<char*, T> x)
//	{
//		istreambuf_iterator<char> iter(file);
//		copy(iter, ifstreambuf_iterator<char> {}, back_inserter(container));
//		f.flush();
//		return f;
//	}
//	template<class T>
//	ofstream & operator<<(ofstream & f, map<char*, T> x)
//	{
//		copy(x.begin(), x.end(), ofstream_iterator<char>(f));
//		f.flush();
//		return f;
//	}
}


//Create minimal perfect hashing for given dictionary of constant size.
template<class T>
void hshlib::PerfectHashing<T>::create(map<string, T> &dict)
{
	int size = dict.size();
	vector<vector<string>> buckets(size);

	D.resize(size);
	values.resize(size);

	for (auto i = dict.begin(); i != dict.end(); ++i)				//Place keys into buckets (keys are placed in same bucket if they have equal values of first hash function).
	{
		int p = hash(0, i->first, size);
		buckets[p].push_back(i->first);
	}

	sort(buckets.begin(), buckets.end(), cmp);						//Sort buckets by length in descending order to process ones with most items first.

	size_t i = 0;
	while (i < size)
	{
		if (buckets[i].size() <= 1)
			break;
		int sz = buckets[i].size();
		int d = 1;
		size_t j = 0;
		vector<int> slots;
		while (j < sz)												//Find value of d (parameter of hash function) which gives different hash function value for every key in the bucket
		{
			int slot = hash(d, buckets[i][j], size);
			if (find(slots.begin(), slots.end(), slot) != slots.end() 
				|| values[slot] != nullptr)
			{
				++d;
				j = 0;
				slots.clear();
			}
			else {
				slots.push_back(slot);
				++j;
			}
		}
		int z = hash(0, buckets[i][0], size);
		D[z] = d;

		for (size_t j = 0; j < sz; ++j)								
			values[slots[j]] = dict.find(buckets[i][j])->second;

		++i;
	}

	list<int> free_indices;

	for (size_t k = 0; k < size; ++k)								//Find empty free slots in values vector.
		if (values[k] == nullptr)
			free_indices.push_back(k);

	while (i < size)												//Only buckets with 1 item remain, store them directly into free slots. Use a negative value of d to indicate this.
	{
		if (buckets[i].size() == 0)
			break;
		int slot = free_indices.front();
		free_indices.pop_front();
		int z = hash(0, buckets[i][0], size);
		D[z] = -slot - 1;											//Substract 1 to ensure it's negative even if it's 0.
		values[slot] = dict.find(buckets[i][0])->second;
		++i;
	}
	cout << endl;
}



//Create minimal perfect hashing from file that contains dict (std::map).
template<class T>
void hshlib::PerfectHashing<T>::create(string filename, map<string, T> &dict)
{
	if (filename == "")
		return;
	prsr::CsvParser Parse;
	vector<vector<string>> data = Parse(filename);
	if (data.size() == 0)
		return;
	if (data[0].size() != 2)
		return;

	for (auto i : data)
	{
		if (i.size() < 2)
			continue;
//		{
//			cout << "SIZE ERR" << endl;
//			system("pause");
//			return;
//		}
		pair<string, T> p(i[0], T(i[1].c_str()));
		dict.insert(p);
	}
//	ifstream file(filename, ios::in || ios::binary);
	//	string key;
	//	char *val_tmp = nullptr;
	//	T val;
	//	while (!file.eof())
	//	{
	//		file >> key;
	////		file >> val;
	////		*val = reinterpret_cast<T>(val_tmp);
	//		//file.read((char*)&val, sizeof(T));
	////		file.read(charval;
	//		pair<char*, T> m;
	//		m.first = (char*)key.c_str();
	////		m.second = val;
	//		container.insert(m);
	//	}
//	int size = readFile(filename, dict);
//	file >> dict;
	create(dict);
}


//Look up a value. Must be sure key exists in the table otherwise result is unpredictable.
//Returns pointer to the value found.
template<class T>
T hshlib::PerfectHashing<T>::lookUp(string key)
{
	int k = hash(0, key, D.size());
	int d = D[k];
	if (d < 0)
		return values[-d - 1];
	k = hash(d, key, values.size());
	return values[k];
}


//Calculate hash function (FNV algorithm used).
template<class T>
int hshlib::PerfectHashing<T>::hash(int d, string word, int s)
{
	long long int R = d;
	if (d == 0)
		R = FNV_32_PRIME;
	for (auto sym : word)
	{
		R = (R*FNV_32_PRIME) ^ sym;
		++sym;
	}
	d = R%s;
	if (d < 0)
		d += s;
	return d;
}


//Comparator for sorting function.
int hshlib::cmp(vector<string>& x, vector<string>& y)
{
	return x.size() > y.size();
}