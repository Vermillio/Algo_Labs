#include "PerfectHashGenerator.h"
#include <fstream>
#include <set>
//#include <vector>
using namespace std;




/*
<summary>Comparator for sorting function.</summary>
*/
int hshlib::cmp(vector<char*>& x, vector<char*>& y)
{
	return x.size() > y.size();
}
