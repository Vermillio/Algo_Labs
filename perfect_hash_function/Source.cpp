//This program implements perfect hashing.
//Perfect hashing is a technique for building a hash table with no collisions. 
//It is only possible to build one when we know all of the keys in advance. 
//Minimal perfect hashing implies that the resulting table contains one entry for each key, and no empty slots.

//We use 2 levels of hashing with 2 different hash functions.
//Memory for storing hash table is O(n).
//Function returns good information about position of element as long as we know that it is stored in the table. Otherwise, result is unpredictable.

#include "PerfectHashGenerator.h"
#include <iostream>

//#include <string>

using namespace std;

char *DICTIONARY = "dict.bin";


map<char*, char*> dict = {
	{ "embarrassment", "disturbance" },
	{ "excitement", "mystery" },
	{ "consciousness", "past" },
	{ "unlikely", "minor" },
	{ "continuation", "round" },
	{ "manufacture", "secretary" },
	{ "relaxation", "swim" },
	{ "personality", "dress" },
	{ "humanity", "church" },
	{ "hypothesize", "research" },
	{ "recommendation", "advice" },
	{ "conventional", "critical" },
	{ "reputation", "soap" },
	{ "representative", "justice" },
	{ "environment", "desert" },
	{ "evolution", "essential" },
	{ "contemporary", "contempt" },
	{ "aluminium", "sugar" },
	{ "retirement", "terrace" },
	{ "dictionary", "overall" },
	{"credibility", "exchange" },
	{ "delivery", "flat" },
	{ "photocopy", "lifestyle" },
	{ "achievement", "award" },
	{ "possibility", "adventure" },
	{ "unfortunate", "movie" },
	{ "psychology", "support" },
	{ "declaration", "proclaim" },
	{ "entitlement", "discriminate" },
	{ "consolidate", "dribble" }
};

int main(int argc, char **argv)
{
	//char **words = nullptr;
	int size = 0;
	/*if (argc < 2)
	{*/
		char *words[] = {
		"excitement", "consolidate", "declaration", "photocopy", "delivery",
		"achievement", "humanity", "representative", "embarrassment", "environment",
		"recommendation", "retirement", "reputation", "unlikely", "possibility",
		"conventional", "manufacture", "psychology", "evolution", "hypothesize",
		"aluminium", "dictionary", "credibility", "relaxation", "consciousness",
		"entitlement", "continuation", "contemporary", "personality", "unfortunate" };
		size = 30;
	/*}*/
	//func(words)
	//else
	//{
	//	char **words = argv;
	//	size = argc - 1;
	//}
		//func(argv)
	hshlib::PerfectHashing<char> HashTable;
	HashTable.create(dict);
	int errors = 0;
	for (size_t i = 0; i < size; ++i)
	{
		char* c = HashTable.lookUp(words[i]);
		if (c != dict.find(words[i])->second)
			++errors;
	}
	if (errors == 0)
		cout << "Success" << endl;
	else cout << "Mismatches : " << errors << endl;

	system("pause");
	return 0;
}