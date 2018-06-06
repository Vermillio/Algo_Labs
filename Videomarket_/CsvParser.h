#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;

namespace prsr
{
	//	template<class T1, class T2>
	class CsvParser
	{
	public:
		vector<vector<string>> parse(string filename);
	private:
		vector<string> getNextLine(istream & f);
	};

	vector<vector<string>> CsvParser::parse(string filename)
	{
		vector<vector<string>> result;
		ifstream file(filename);
		while (file.good())
			result.push_back(getNextLine(file));
		file.close();
		return result;
	}

	inline vector<string> CsvParser::getNextLine(istream & f)
	{
		vector<string>   result;
		string           line;
		getline(f, line);
		stringstream          lineStream(line);
		string                cell;

		while (getline(lineStream, cell, ','))
		{
			result.push_back(cell);
		}
		// This checks for a trailing comma with no data after it.
		if (!lineStream && cell.empty())
		{
			result.push_back("");
		}
		return result;
	}
}