#include "LexicographicSearchTree.h"


using namespace std;

Tree::LSTnodep Tree::LexicographicSearchTree::lookUp(const string & str, size_t &position)
{
	LSTnodep curRoot = root;
	LSTnodep found = nullptr;
	LSTnodep last = nullptr;
	for (size_t i = 0; i < str.size(); ++i) {
		found = search(curRoot, str[i]);
		if (!found) {
			position = i;
			return last;
		}
	}
	position = str.size();
	return found;
}

std::vector<std::string> Tree::LexicographicSearchTree::request(const string &str)
{
	///check if str is a valid word (consists only of Latin letters)
	if (!isValidWord(str))
		return {};

	///check if string is present in tree
	size_t n=0;
	LSTnodep found = lookUp(str, n);
	if (n != str.size())
		return {};

	vector<string> words = {str};
	string current;

	function<void(const LSTnodep&)> findAllWords;
	findAllWords = [&findAllWords, &words, &current] (const LSTnodep &node) -> void {
		if (!node)
			return;
		findAllWords(node->l);
		current += string(1, node->data.to_char());
		if (node->isEndOfWord())
			words.push_back(current);
		findAllWords(node->m);
		current = current.substr(0, current.size() - 1);
		findAllWords(node->r);
	};
	findAllWords(found->m);
	return words;
}

bool Tree::LexicographicSearchTree::add(const string &str)
{
	///check if str is a valid word
	if (!isValidWord(str))
		return {};

	///find first n symbols of str and retrieve pointer to last existing symbol
	size_t n = 0;
	LSTnodep lastExisting = lookUp(str, n);
	
	///if the whole string is present, then there's no need to insert something.
	if (n == str.size())
		return false;

	///create auxiliary root if needed
	if (!lastExisting) {
		LSTnodep emptyRoot = make_shared<LSTnode>();
		emptyRoot->m = root;
		lastExisting = emptyRoot;
	}

	///insert remaining symbols
	for (size_t j = n; j < str.size() && lastExisting && lastExisting->data.to_char() == str[j]; j++) {
		insert(lastExisting->m, str[j]);
		lastExisting = lastExisting->m;
	}
	return true;
}

int Tree::LexicographicSearchTree::loadDict(const string &filename)
{
	ifstream file(filename);
	string s;
	int count;
	while (!file.eof()) {
		file >> s;
		if (!add(s))
			++count;
	}
	return count;
}

bool Tree::LexicographicSearchTree::isValidWord(const string & str) const
{
	if (str.size() == 0)
		return false;
	for (auto c : str)
		if (!isalpha(c))
			return false;
	return true;
}
