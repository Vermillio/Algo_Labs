#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <queue>
#include <string>
#include <conio.h>
#include <fstream>
#include <windows.h>

using namespace std;


namespace pset {

	string get_cd();

	template<class T, typename func = less<T>>
	class PersistentSet_rb
	{
	public:
		using PSrb = PersistentSet_rb;
		using PSrb_tmpl = PersistentSet_rb<T, func>;

		struct node;
		using nodep = shared_ptr<node>;
		class InputHandler
		{
		private:
			//Image saving procedure.
			void write_proc(nodep p, ostream &file, int label);

		public:
			//Saves tree as image in PNG format and shows  on screen.
			void makeImage(PSrb_tmpl &Set, string filename);

			//Gets input from console.
			//Saves changes if executed on existing tree.
			void getInput(PSrb_tmpl *Set = nullptr);

			//Checks order of elements in the tree.
			//Returns number of errors.
			int checkOrder(PSrb_tmpl &Set);
		};

		PersistentSet_rb() : Size(0) {};

		class TreeIterator 
		{
		public:

			TreeIterator(nodep _ptr, nodep _root) : ptr(_ptr), root(_root) {};

			TreeIterator& operator++();
			TreeIterator& operator--();
			TreeIterator operator+(int N);
			TreeIterator operator-(int N);
			const T& operator*() const					{ return ptr->data; };
			const T& operator-> () const				{ return ptr->data; };
			void operator=(TreeIterator &it);
			bool operator==(TreeIterator &it) const;
			bool operator!=(TreeIterator &it) const;
		private:

			nodep successor();
			nodep predecessor();

			nodep ptr;
			nodep root;
		};

		TreeIterator begin();
		TreeIterator end();

		bool insert(const T &x);
		bool remove(const T &x);
		nodep search(T &x);
		void clear();
		bool empty() const;
		void print() const;
		void consoleOutput(nodep p, int level) const;

	private:
		struct node {
			bool isRed;
			nodep l, r;
			T data;

			node() : isRed(true), l(nullptr), r(nullptr) {};
			node(node *t) : data(t->data), isRed(t->isRed), l(t->l), r(t->r) {};
			node(nodep t) : data(t->data), isRed(t->isRed), l(t->l), r(t->r) {};
			node(T _x) : data(_x), isRed(true), l(nullptr), r(nullptr) {};
			node(T _x, bool _isRed, nodep _l, nodep _r) : data(_x), isRed(_isRed), l(_l), r(_r) {};
			~node() {};

			nodep clone() {
				nodep t;
				t.reset(new node(this));
				return t;
			}
			void swapRed(nodep &t) {
				bool tmp = isRed;
				isRed = t->isRed;
				t->isRed = tmp;
			}
		};

		size_t Size;
		vector<nodep> roots;

		size_t size() const;
		nodep rotate_l(nodep t);
		nodep rotate_r(nodep t);
		nodep rotate_twice_l(nodep t);
		nodep rotate_twice_r(nodep t);
		void checkInsert(vector<nodep> &path);
		void checkRemove(vector<nodep> &path);

		void copy_path(const T &x, vector<nodep> &path, bool insert = true);
		void find_path(const T &x, vector<nodep> &path);
		void remove_node(nodep parent, bool left);
		nodep predecessor_p(nodep t);

	};

	template<class T, typename func>
	using PSrb_nodep = typename PersistentSet_rb<T, func>::nodep;

	template<class T, typename func>
	using PSrb_TreeIterator = typename PersistentSet_rb<T, func>::TreeIterator;

	template<class T, typename func>
	inline size_t PersistentSet_rb<T, func>::size() const
	{
		return Size;
	}

	template<class T, typename func>
	PSrb_nodep<T, func> PersistentSet_rb<T, func>::rotate_l(PSrb_nodep<T, func> t)
	{
		nodep pivot = t->r;
		t->r = pivot->l;
		pivot->l = t;
		return pivot;
	}

	template<class T, typename func>
	PSrb_nodep<T, func> PersistentSet_rb<T, func>::rotate_r(PSrb_nodep<T, func> t)
	{
		nodep pivot = t->l;
		t->l = pivot->r;
		pivot->r = t;
		return pivot;
	}

	template<class T, typename func>
	PSrb_nodep<T, func> PersistentSet_rb<T, func>::rotate_twice_l(PSrb_nodep<T, func> t)
	{
		t->r = rotate_r(t->r);
		return rotate_l(t);
	}

	template<class T, typename func>
	PSrb_nodep<T, func> PersistentSet_rb<T, func>::rotate_twice_r(PSrb_nodep<T, func> t)
	{
		t->l = rotate_l(t->l);
		return rotate_r(t);
	}

	template<class T, typename func>
	void PersistentSet_rb<T, func>::checkInsert(vector<PSrb_nodep<T, func>> &path)
	{
		if (path.size() < 3)
		{
			path[1]->isRed = false;
			return;
		}
		size_t i = path.size() - 1;
		while (i>=2)
		{
			nodep t = path[i];
			nodep p = path[i - 1];
			if (p->isRed == false)
				break;
			if (i < 3)
				break;
			nodep g = path[i - 2];
			nodep q = path[i - 3];
			if (g) {
					if (p == g->l && p->isRed && (g->r==nullptr || g->r->isRed == false))
					{
						if (t == p->r)
						{
							if (q->r==g)
								q->r = rotate_twice_r(g);
							else q->l = rotate_twice_r(g);
							p->swapRed(g);
							iter_swap(path.begin() + i - 1, path.begin() + i);
							path.erase(path.begin() + i - 2);
						}
						else {
							if (q->r == g)
								q->r = rotate_r(g);
							else q->l = rotate_r(g);
							p->swapRed(g);
							path.erase(path.begin() + i - 2);
						}

					}
					else if (p == g->r && p->isRed && (g->l==nullptr || g->l->isRed == false))
					{
						if (t == p->l)
						{
							if (q->l == g)
								q->l = rotate_twice_l(g);
							else q->r = rotate_twice_l(g);
							swap(p->isRed, g->isRed);
							iter_swap(path.begin() + i - 1, path.begin() + i);
							path.erase(path.begin() + i - 2);
						}
						else {
							if (q->l == g)
								q->l = rotate_l(g);
							else q->r = rotate_l(g);
							p->swapRed(g);
							path.erase(path.begin() + i - 2);
						}
					}
					else if (g->l && g->r && g->l->isRed && g->r->isRed)
					{
						g->isRed = true;
						if (g->l)
							g->l->isRed = false;
						if (g->r)
							g->r->isRed = false;
					}
				}
			--i;
		}
		path[1]->isRed = false;
	}

	template<class T, typename func>
	inline void   PersistentSet_rb<T, func>::checkRemove(vector<PSrb_nodep<T, func>> &path)
	{
		if (path.size()<=1)
			return;
		size_t i = path.size() - 1;
		nodep t = path[i];
		while (i >= 3)
		{
			t = path[i];
			nodep p = path[i-1];
			nodep g = path[i - 2];
			bool left = g->l == p;
			nodep w = t == p->r ? p->l : p->r;
			if (!w)
			{
				--i;
				continue;
			}
			if (w->isRed == true)
			{
				w->isRed = false;
				p->isRed = true;
				if (t == p->r) {
					if (left)
						 g->l = rotate_r(p);
					else g->r = rotate_r(p);
					path.erase(path.begin() + i - 1);
				} else {
					if (left)
						g->l = rotate_l(p);
					else g->r = rotate_l(p);
					path.erase(path.begin() + i - 1);	}
			}
			else if (!p->isRed && w->l && w->r && !w->isRed && !w->l->isRed && !w->r->isRed)
			{
				w->isRed = true;
			}
			else if (p->isRed && w->l && w->r && !w->isRed && !w->l->isRed && !w->r->isRed)
			{
				w->swapRed(p);
			}
			else if (t == p->r && w->r && w->l && !w->l->isRed && !w->r->isRed)
			{
				w->r->swapRed(w);
				p->l = rotate_r(w);
			}
			else if (t == p->l && w->l && w->r && !w->isRed && w->l->isRed && !w->r->isRed)
			{
				w->l->swapRed(w);
				p->r = rotate_l(w);
			}
			else if (t == p->l && w->r && !w->isRed && w->r->isRed)
			{
				w->r->isRed = false;
				
				if (left)
					 g->l = rotate_l(p);
				else g->r = rotate_l(p);
				
				path.erase(path.begin() + i - 1);
				p->swapRed(w);
			}
			else if (t == p->r && w->l && !w->isRed && w->l->isRed)
			{
				w->l->isRed = false;
				if (left)
					g->l = rotate_r(p);
				else g->r = rotate_r(p);
				path.erase(path.begin() + i - 1);
				p->swapRed(w);
			}
			--i;
		}
		path[1]->isRed = 0;
	}

	template<class T, typename func>
	inline PSrb_TreeIterator<T, func> PersistentSet_rb<T, func>::begin()
	{
		if (roots.size() == 0 || !roots.back())
			return TreeIterator(nullptr, nullptr);
		nodep p = roots.back();
		while (p->l)
			p = p->l;
		return TreeIterator(p, roots.back());
	}

	template<class T, typename func>
	inline PSrb_TreeIterator<T, func> PersistentSet_rb<T, func>::end()
	{
		if (roots.size()==0)
			return TreeIterator(nullptr, nullptr);
		else
			return TreeIterator(nullptr, roots.back());
	}

	template<class T, typename func>
	inline bool PersistentSet_rb<T, func>::insert(const T &x)
	{
		if (roots.size() == 0 || !roots.back())
		{
			nodep newNode(new node(x));
			roots.push_back(newNode);
			Size = 1;
			return true;
		}
		nodep p = roots.back();
		vector<nodep> path;
		size_t curSize = Size;
		copy_path(x, path, true);
		nodep tmpRoot(new node(0));
		tmpRoot->r = path.front();
		path.insert(path.begin(), tmpRoot);
		checkInsert(path);
		if (path.size()!=0)
			roots.push_back(path[0]->r);
		if (curSize != Size)
			return true;
		else return false;
	}

	template<class T, typename func>
	inline bool PersistentSet_rb<T, func>::remove(const T &x)
	{
		if (roots.size() == 0 || Size==0)
			return false;
		nodep p = roots.back();
		if (!p)
			return false;
		if (Size == 1)
		{
			roots.push_back(nullptr);
			return true;
		}
		vector<nodep> path;
		copy_path(x, path, false);
		if (path.size() == 0)
			return false;
		nodep tmpRoot(new node(0));
		tmpRoot->r = path[0];
		path.insert(path.begin(), tmpRoot);
		bool dir = path[path.size() - 2]->l && path[path.size() - 2]->l->data == x;
		remove_node(path[path.size()-2], dir);
		path.pop_back();
		checkRemove(path);
		roots.push_back(path[0]->r);
		--Size;
		return true;
	}
	
	template<class T, typename func>
	PSrb_nodep<T, func> PersistentSet_rb<T, func>::search(T & x)
	{
		if (roots.size() == 0 || !roots.back() || Size==0)
			return nullptr;

		if (Size == 1)
		{
			if (roots.back()->data == x)
				return roots.back();
			else return nullptr;
		}
		nodep p = roots.back();
		while (p)
		{
			if (p->data < x)
				p = p->r;
			else if (p->data > x)
				p = p->l;
			else return p;
		}
		return nullptr;
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::clear()
	{
		roots.clear();
		Size = 0;
	}

	template<class T, typename func>
	inline bool PersistentSet_rb<T, func>::empty() const 
	{
		return Size==0;
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::copy_path(const T &x, vector<PSrb_nodep<T, func>> &path, bool insert /*=true*/)
	{
		if (roots.size() == 0)
			return;
		path.clear();
		nodep p = roots.back();
		while (p)
		{
			path.push_back(p);
			if (x == p->data)
			{
				return;
			}
			else if (x < p->data) 
			{
				if (p->l)	{
					path.back()->l = p->l->clone();
					p = path.back()->l;
				} 
				else {
					if (!insert)
					{
						path.clear();
						return;
					}
					nodep tmp(new node(x));
					path.back()->l = tmp;
					path.push_back(tmp);
					++Size;
					return;
				}
			}
			else if (p->r)
			{
				path.back()->r = p->r->clone();
				p = path.back()->r;
			} else {
				if (!insert)
				{
					path.clear();
					return;
				}
				nodep tmp(new node(x));
				path.back()->r = tmp;
				path.push_back(tmp);
				++Size;
				return;
			}
		}
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::find_path(const T &x, vector<PSrb_nodep<T, func>> &path)
	{
		nodep p = roots.back();
		path.clear();
//		path.push_back(p);
		while (p)
		{
			path.push_back(p);
			if (x == p->data)
				return;
			else if (x < p->data) {
				if (p->l)
					p = p->l;
				else {
					path.clear();
					return;	
				}
			}
			else if (p->r)
				p = p->r;
			else {
				path.clear();
				return; 
			}
		}
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::remove_node(PSrb_nodep<T, func> parent, bool left)
	{
		nodep t = left ? parent->l : parent->r;
		if (t->l == nullptr)
		{
			if (left)
				parent->l = t->r;
			else parent->r = t->r;
		}
		else if (t->r == nullptr)
		{
			if (left)
				parent->l = t->l;
			else parent->r = t->l;
		}
		else {
//			bool dir;
			nodep p = predecessor_p(t);
			if (p)
			{
				swap(t==p ? t->l->data : p->r->data, t->data);
				remove_node(p, t == p);
			}
		}
	}

	template<class T, typename func>
	inline PSrb_nodep<T, func> PersistentSet_rb<T, func>::predecessor_p(PSrb_nodep<T, func> t)
	{
		if (!t || !t->l)
			return nullptr;
		nodep p = t;
		nodep c = t->l;
		while (c->r)
		{
			p = c;
			c = c->r;
		}
		return p;
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::InputHandler::write_proc(PSrb_nodep<T, func> p, ostream & file, int label)
	{
		if (!p)
			return;
		file << "A" + to_string(label) << " [label=\"" << to_string(p->data) << "\"];" << endl;
		if (p->l)
		{
			file << "A" + to_string(label) << "-> A" + to_string(label * 2) << ";" << endl;
			write_proc(p->l, file, label * 2);
		}
		if (p->r)
		{
			file << "A" + to_string(label) << "-> A" + to_string(label * 2+1) << ";" << endl;
			write_proc(p->r, file, label * 2+1);
		}
	}

	inline string get_cd()
	{
		wchar_t * c = nullptr;
		wstring w = _wgetcwd(c, _MAX_PATH);
		return string(w.begin(), w.end());
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::InputHandler::makeImage(PSrb_tmpl & Set, string filename)
	{
		ofstream fout("tmp.gv", ios::out);
		fout << "digraph " + filename + " {" << endl;
		write_proc(Set.roots.back(), fout, 1);
		fout << "}" << endl;
		fout.close();
		string fname = filename + ".png";
		//convert_dot_to_png("C:/Users/morga/source/repos/2_course_2_sem/algo_labs/B+_Tree/B+_Tree/tmp.gv", fname);
		string cd = get_cd();
		string command = "dot -Tpng " + cd + "/tmp.gv > " + cd + "/" + fname;
		SetCurrentDirectory((cd + "/bin").c_str());
		system(command.c_str());
		SetCurrentDirectory(cd.c_str());
		system(fname.c_str());
		//system("del tmp.gv");
		_getch();
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::InputHandler::getInput(PSrb_tmpl* Set)
	{
		bool existing = true;
		if (Set == nullptr)
		{
			Set = new PSrb_tmpl();
			existing = false;
		}

		int choise = 0;
		string message = "";
		do
		{
			system("cls");
			cout << message << endl;
			cout << "---TREE---" << endl;
			cout << "SIZE IS " << Set->size() << " NODES." << endl;
			Set->print();
			cout << endl;
			cout << "1 - INSERT, 2 - REMOVE, 3 - SEARCH, 4 - CLEAR, 5 - WRITE IMAGE FILE (GRAPHVIZ), 6 - TEST ORDER WITH ITERATOR, <ANYTHING ELSE> - EXIT" << endl;
			cout << ">>>";

			cin >> choise;
			T data;
			switch (choise)
			{
			case 1:
				cout << "Enter data to insert >>>";
				cin >> data;
				Set->insert(data);
				message = "Successfully inserted " + to_string(data) + ".";
				break;
			case 2:
			{
				cout << "Enter data to remove >>>";
				cin >> data;
				bool success = Set->remove(data);
				if (success)
					message = "Successfully removed " + to_string(data) + ".";
				else message = "Not found " + to_string(data) + ".";
				break;
			}
			case 3:
			{
				cout << "Enter data to search for >>>";
				cin >> data;
				PSrb_nodep<T, func> found = Set->search(data);
				if (!found)
					message = "Not found " + to_string(data) + ".";
				else
					message = "Found " + to_string(data) + ".";
				break;
			}
			case 4:
				Set->clear();
				message = "Tree is now empty.";
				break;
			case 5:
			{
				string filename;
				cout << "Enter file name to save image (without extenstion) >>>";
				cin >> filename;
				makeImage(*Set, filename);
				break;
			}
			case 6:
			{
				int errors_num = checkOrder(*Set);
				message = "Test complete. Number of errors : " + to_string(errors_num) + ".";
			}
			default:
				break;
			}
		} while (choise >= 1 && choise <= 6);
		if (!existing)
		{
			delete Set;
		}
	}

	template<class T, typename func>
	inline int PersistentSet_rb<T, func>::InputHandler::checkOrder(PSrb_tmpl &Set)
	{
		int error_num = 0;

		for (PSrb_TreeIterator<T, func> it = Set.begin()+1; it != Set.end(); ++it)
		{
			if (*(it - 1) > *it)
				++error_num;
		}
		return error_num;
	}

	template<class T, typename func>
	void PersistentSet_rb<T, func>::print() const
	{
		if (roots.size() != 0)
			consoleOutput(roots.back(), 0);
		cout << endl;
	}

	template<class T, typename func>
	void PersistentSet_rb<T, func>::consoleOutput(PSrb_nodep<T, func> p, int level) const
	{
		if (!p)
			return;
		consoleOutput(p->r, level + 1);
		for (int i = 0; i < level; i++)
			cout << "     ";
		cout << "(" << to_string(p->data) << ", " << (p->isRed ? "r" : "b") << ")" << endl;
		consoleOutput(p->l , level + 1);
	}

	template<class T, typename func>
	inline PSrb_TreeIterator<T, func>& PersistentSet_rb<T, func>::TreeIterator::operator++()
	{
		ptr = successor();
		return *this;
	}

	template<class T, typename func>
	inline PSrb_TreeIterator<T, func>& PersistentSet_rb<T, func>::TreeIterator::operator--()
	{
		ptr = predecessor();
		return *this;
	}

	template<class T, typename func>
	inline PSrb_TreeIterator<T, func> PersistentSet_rb<T, func>::TreeIterator::operator+(int N)
	{
		nodep p = ptr;
		for (int i = 0; i < N; ++i)
			p = successor();
		return TreeIterator(p, root);
	}

	template<class T, typename func>
	inline PSrb_TreeIterator<T, func> PersistentSet_rb<T, func>::TreeIterator::operator-(int N)
	{
		nodep p = ptr;
		for (int i = 0; i < N; ++i)
			p = predecessor();
		return TreeIterator(p, root);
	}

	template<class T, typename func>
	inline bool PersistentSet_rb<T, func>::TreeIterator::operator!=(TreeIterator & it) const
	{
		return !(*this == it);
	}

	template<class T, typename func>
	inline PSrb_nodep<T, func> PersistentSet_rb<T, func>::TreeIterator::successor()
	{
		if (!root || !ptr)
			return ptr;
		PSrb_nodep<T, func> parent = nullptr;
		PSrb_nodep<T, func> p = root;
		while (p)
		{
			if (p->data < ptr->data)
			{
				parent = p;
				p = p->r;
			}
			else if (p->data > ptr->data)
			{
				parent = p;
				p = p->l;
			}
			else if (p->r)
				return p->r;
			else if (parent && parent->l && parent->l==p)
				return parent;
			else return nullptr;
		}
	}

	template<class T, typename func>
	inline PSrb_nodep<T, func> PersistentSet_rb<T, func>::TreeIterator::predecessor()
	{
		if (!root || !ptr)
			return ptr;
		PSrb_nodep<T, func> parent = nullptr;
		PSrb_nodep<T, func> p = root;
		while (p)
		{
			if (p->data < ptr->data)
			{
				parent = p;
				p = p->r;
			}
			else if (p->data > ptr->data)
			{
				parent = p;
				p = p->l;
			}
			else if (p->l)
				return p->l;
			else if (parent && parent->r && parent->r == p)
				return parent;
			else return nullptr;
		}
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::TreeIterator::operator=(PSrb_TreeIterator<T, func> & it)
	{
		ptr = it.ptr;
		root = it.root;
	}

	template<class T, typename func>
	inline bool PersistentSet_rb<T, func>::TreeIterator::operator==(PSrb_TreeIterator<T, func> & it) const
	{
		if (ptr == it.ptr && root == it.root)
			return true;
		else 
			return false;
	}
}