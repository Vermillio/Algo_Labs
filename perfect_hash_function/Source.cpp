//This program implements perfect hashing.
//Perfect hashing is a technique for building a hash table with no collisions. 
//It is only possible to build one when we know all of the keys in advance. 
//Minimal perfect hashing implies that the resulting table contains one entry for each key, and no empty slots.

//We use 2 levels of hashing with 2 different hash functions.
//Memory for storing hash table is O(n).
//Function returns good information about position of element as long as we know that it is stored in the table. Otherwise, result is unpredictable.

#include "PerfectHashGenerator.h"
#include <iostream>

using namespace std;

//char *DICTIONARY = "dict.bin";


map<string, char*> dict;

int main(int argc, char **argv)
{
	size_t size = 0;
	//char *words[] = {
	//"excitement", "consolidate", "declaration", "photocopy", "delivery",
	//"achievement", "humanity", "representative", "embarrassment", "environment",
	//"recommendation", "retirement", "reputation", "unlikely", "possibility",
	//"conventional", "manufacture", "psychology", "evolution", "hypothesize",
	//"aluminium", "dictionary", "credibility", "relaxation", "consciousness",
	//"entitlement", "continuation", "contemporary", "personality", "unfortunate" };
	size = 30;
	hshlib::PerfectHashing<char*> HashTable;
	HashTable.create("submission.csv", dict);
	int errors = 0;
	int max = 2919;
	int min = 1460;
	for (size_t i = 0; i < size; ++i)
	{
		int num = rand() % (max - min) + min;
		string a = to_string(num);
		string c = HashTable.lookUp(a);
		if (c != dict.find(a)->second)
			++errors;
	}
	if (errors == 0)
		cout << "Success" << endl;
	else cout << "Mismatches : " << errors << endl;

	system("pause");
	return 0;
}