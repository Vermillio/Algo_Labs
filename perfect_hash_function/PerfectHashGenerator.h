#pragma once

#define FNV_32_PRIME 0x01000193
#include <vector>
#include <map>
#include <list>
#include <set>
#include <algorithm>

using namespace std;

namespace hshlib
{



	template<typename T>
	class PerfectHashing
	{
	public:
		void create(map<char*, T*> dict);
		void create(char *filename);
		T* lookUp(char *word);

	private:
		vector<T*> values;
		vector<int> D;
		int readFile(char *filename, map<char*, T*> &container);
		int hash(int d, char *word, int s);
	};
	int cmp(vector<char*> &x, vector<char*> &y);
}

/*
<summary>Create minimal perfect hashing for given dictionary of constant size</summary>
<returns>void</returns>
*/
template<typename T>
void hshlib::PerfectHashing<T>::create(map<char*, T*> dict)
{
	int size = dict.size();
	vector<vector<char*>> buckets(size);

	D.resize(size);
	values.assign(size, nullptr);

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
}


/*
<summary>Create minimal perfect hashing from text file</summary>
<returns></returns>
*/
template<typename T>
void hshlib::PerfectHashing<T>::create(char * filename)
{
	map<char*, T> dict = nullptr;
	int size = readFile(filename, dict);
	create(dict, size);
}

/*
<summary>Look up a value. Must be sure key exists in the table otherwise result is unpredictable.</summary>
<returns>Pointer to the value found.</returns>
*/
template<typename T>
T* hshlib::PerfectHashing<T>::lookUp(char * key)
{
	int k = hash(0, key, D.size());
	int d = D[k];
	if (d < 0)
		return values[-d - 1];
	k = hash(d, key, values.size());
	return values[k];
}

/*
<summary>Read binary file that contains dictionary.</summary>
<returns>Size of resulting dictionary.</returns>
*/
template<typename T>
int hshlib::PerfectHashing<T>::readFile(char * filename, map<char*, T*> &container)
{
	if (container.size()!=0 || filename == nullptr || filename == "")
		return 0;
	ifstream file(filename, ios::in | ios::binary);
	char *key;
	T val
	while (!file.eof())
	{
		file >> key;
		file >> val;
		container.insert({ key, &val });
	}
	return container.size();
}

/*
<summary>Calculate hash function (FNV algorithm).</summary>
<returns>Hash value.</returns>
*/
template<typename T>
int hshlib::PerfectHashing<T>::hash(int d, char * word, int s)
{
	long long int R = d;
	if (d == 0)
		R = FNV_32_PRIME;
	char *c = word;
	while (*c != '\0')
	{
		R = (R*FNV_32_PRIME) ^ *c;
		++c;
	}
	d = R%s;
	if (d < 0)
		d += s;
	return d;
}
