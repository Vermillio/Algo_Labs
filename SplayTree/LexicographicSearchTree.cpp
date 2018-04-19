#include "LexicographicSearchTree.h"

using namespace std;

Tree::LSTnodep Tree::LexicographicSearchTree::lookUp(const string & str, size_t &position)
{
	LSTnodep *curRoot = &root;
	LSTnodep found = nullptr;
	LSTnodep last = nullptr;
	for (size_t i = 0; i < str.size(); ++i) {
		found = search(*curRoot, str[i]);
		if (!found||found->data.to_char()!=str[i]) {
			position = i;
			return last;
		}
		last = found;
		curRoot = &found->m;
	}
	position = str.size();
	return found;
}

void Tree::LexicographicSearchTree::writeImageProc(const LSTnodep &node, ostream & file, int label) const
{
	if (!node)
		return;
	file << "A" + std::to_string(label) << " [label=\"" << node->data << "\"];" << endl;
	if (node->l)
	{
		file << "A" + std::to_string(label) << "-> A" + std::to_string(label * 3) << ";" << endl;
		writeImageProc(node->l, file, label * 3);
	}

	if (node->m) {
		file << "A" + std::to_string(label) << "-> A" + std::to_string(label * 3+1) << "[weight=10000] ;" << endl;
		writeImageProc(node->m, file, label * 3+1);
	}

	if (node->r)
	{
		file << "A" + std::to_string(label) << "-> A" + std::to_string(label * 3 + 2) << ";" << endl;
		writeImageProc(node->r, file, label * 3 + 2);
	}
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

	vector<string> words = {};

	if (found->isEndOfWord)
		words.push_back(str);
	string current=str;

	function<void(const LSTnodep&)> findAllWords;
	findAllWords = [&findAllWords, &words, &current] (const LSTnodep &node) -> void {
		if (!node)
			return;
		findAllWords(node->l);
		current += string(1, node->data.to_char());
		if (node->isEndOfWord)
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
	LSTnodep current = lastExisting;

	///create auxiliary root if needed
	LSTnodep emptyRoot = nullptr;
	if (!current) {
		emptyRoot = make_shared<LSTnode>();
		emptyRoot->m = root;
		current = emptyRoot;
	}

	///insert remaining symbols
	size_t j = n;

	do {
		insert(current->m, str[j]);
		lastExisting = current;
		current = current->m;
	} while (++j < str.size() && current && current->data.to_char() == str[j-1]);
	lastExisting->isEndOfWord = true;
	if (emptyRoot)
		root = emptyRoot->m;
	return true;
}

int Tree::LexicographicSearchTree::loadDict(const string &filename)
{
	ifstream file(filename);
	string s;
	int count=0;
	while (!file.eof()) {
		file >> s;
		if (!add(s))
			++count;
	}
	return count;
}

void Tree::LexicographicSearchTree::makeImage(const string &filename) const
{
	if (Size > MaxPrintSize) {
		cout << "Too big tree to show on screen." << endl;
		return;
	}
	ofstream fout("tmp.gv", ios::out);
	fout << "digraph " + filename + " {" << endl;
	writeImageProc(root, fout, 1);
	fout << "}" << endl;
	fout.close();
	string fname = filename + ".png";
	//convert_dot_to_png("C:/Users/morga/source/repos/2_course_2_sem/algo_labs/B+_Tree/B+_Tree/tmp.gv", fname);
	char cd_char[MAX_PATH];
	GetCurrentDirectory(sizeof(cd_char), cd_char);
	string cd=cd_char;
	string command = "dot -Tpng " + cd + "/tmp.gv > " + cd + "/" + fname;
	SetCurrentDirectory((cd + "/bin").c_str());
	system(command.c_str());
	SetCurrentDirectory(cd.c_str());
	system(fname.c_str());
	//system("del tmp.gv");
	_getch();
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

void Tree::LexicographicSearchTree::input(const string &imageFilename)
{
	while (true) {
		system("cls");
		cout << "Enter string (enter . to exit) >>>";
		string s;
		cin >> s;
		if (s == ".") {
			cout << "EXIT" << endl;
			_getch();
			return;
		}
		vector<string> words = request(s);
		cout << "Auto complete: " << endl;
		for (auto c : words)
			cout << c << endl;
		makeImage(imageFilename);
		_getch();
	}
}
