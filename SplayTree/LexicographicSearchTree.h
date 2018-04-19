

#ifndef LEXICOGRAPHIC_SEARCH_TREE_H
#define LEXICOGRAPHIC_SEARCH_TREE_H

#include "SplayTree.h"
#include <string>
#include <vector>
#include <fstream>
#include <functional>

using namespace std;


namespace Tree {


	class Letter {
	protected:
		char value;
	public:
		Letter() {};
		Letter(char _c) : value(_c) {};

		char to_char() { return value; }

		bool operator==(const Letter &x) const { return value == x.value; };
		bool operator!=(const Letter &x) const { return value != x.value; };
		bool operator<(const Letter &x) const { return value < x.value; };
		bool operator>(const Letter &x) const { return value > x.value; };
		bool operator<=(const Letter &x) const { return value <= x.value; };
		bool operator>=(const Letter &x) const { return value >= x.value; };
		friend istream& operator>>(istream &c, Letter &x) {
			string y;
			c >> y;
			if (y.size() != 0)
				x.value = y[0];
			return c;
		};
		friend ostream& operator<< (ostream &c, const Letter &x) {
			c << to_string(x);
			return c;
		};

		friend string to_string(const Letter &x) {
			return string(1, x.value);
		}
	};

	class LSTnode;

	using LSTnodep = shared_ptr<LSTnode>;

	class LSTnode : public NodeBase<Letter, LSTnode> {
	public:
		LSTnodep m;
		bool isEndOfWord;

		LSTnode() {
			m = l = r = parent = nullptr;
			isEndOfWord = false;
		}

		LSTnode(const Letter &x, const LSTnodep &_l = nullptr, const LSTnodep &_r = nullptr, const LSTnodep &_parent = nullptr, bool _isEndOfWord=false) {
			l = _l;
			r = _r;
			parent = _parent;
			data = x;
			isEndOfWord = _isEndOfWord;
		};

		~LSTnode() {}
	};

	class LexicographicSearchTree : protected SplayTree<Letter, LSTnode> {
	protected:
		int MaxPrintSize = 250;
		LSTnodep lookUp(const string &str, size_t &position);
		void writeImageProc(const LSTnodep &node, ostream & file, int label) const;
	public:
		vector<string> request(const string &str);
		bool add(const string &str);
		int loadDict(const string &filename);
		void makeImage(const string &filename) const;
		bool isValidWord(const string &str) const;
		void input(const string &imageFilename);
	};
}

#endif