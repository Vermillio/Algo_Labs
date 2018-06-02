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


namespace set {



	string get_cd();

	template<class T, typename func = less<T>>
	class PersistentSet_rb
	{
	public:
		using PSrb = PersistentSet_rb;
		using PSrb_tmpl = PersistentSet_rb<T, func>;

		struct Node;
		using Nodep = shared_ptr<Node>;

		//Saves tree as image in PNG format and shows  on screen.
		void makeImage(string filename);

		//Gets input from console.
		//Saves changes if executed on existing tree.
		void getInput();

		//Checks order of elements in the tree.
		//Returns number of errors.
		int checkOrder();

		PersistentSet_rb() : Size(0) {};

		class TreeIterator 
		{
		public:

			TreeIterator(Nodep _ptr, Nodep _root) : ptr(_ptr), root(_root) {};

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

			Nodep successor();
			Nodep predecessor();

			Nodep ptr;
			Nodep root;
		};

		TreeIterator begin();
		TreeIterator end();

		bool insert(const T &x);
		bool remove(const T &x);
		Nodep search(T &x);
		void clear();
		bool empty() const;
		void print() const;
		void consoleOutput(Nodep p, int level) const;
		size_t treeSize() const;

	private:
		struct Node {
			bool isRed;
			Nodep l, r;
			T data;

			Node() : isRed(true), l(nullptr), r(nullptr) {};
			Node(Node *t) : data(t->data), isRed(t->isRed), l(t->l), r(t->r) {};
			Node(Nodep t) : data(t->data), isRed(t->isRed), l(t->l), r(t->r) {};
			Node(T _x) : data(_x), isRed(true), l(nullptr), r(nullptr) {};
			Node(T _x, bool _isRed, Nodep _l, Nodep _r) : data(_x), isRed(_isRed), l(_l), r(_r) {};
			~Node() {};

			Nodep clone() {
				Nodep t;
				t.reset(new Node(this));
				return t;
			}
			void swapRed(Nodep &t) {
				bool tmp = isRed;
				isRed = t->isRed;
				t->isRed = tmp;
			}
		};

		size_t Size;
		vector<Nodep> roots;

		Nodep rotate_l(Nodep t);
		Nodep rotate_r(Nodep t);
		Nodep rotate_twice_l(Nodep t);
		Nodep rotate_twice_r(Nodep t);
		void checkInsert(vector<Nodep> &path);
		void checkRemove(vector<Nodep> &path);

		void copy_path(const T &x, vector<Nodep> &path, bool insert = true);
		void find_path(const T &x, vector<Nodep> &path);
		void remove_node(Nodep parent, bool left);
		Nodep predecessor_p(Nodep t);

		//Image saving procedure.
		void write_proc(Nodep p, ostream &file, int label);
	};

	template<class T, typename func>
	using PSrb_nodep = typename PersistentSet_rb<T, func>::Nodep;

	template<class T, typename func>
	using PSrb_TreeIterator = typename PersistentSet_rb<T, func>::TreeIterator;

	template<class T, typename func>
	inline size_t PersistentSet_rb<T, func>::treeSize() const
	{
		return Size;
	}

	template<class T, typename func>
	PSrb_nodep<T, func> PersistentSet_rb<T, func>::rotate_l(PSrb_nodep<T, func> t)
	{
		Nodep pivot = t->r;
		t->r = pivot->l;
		pivot->l = t;
		return pivot;
	}

	template<class T, typename func>
	PSrb_nodep<T, func> PersistentSet_rb<T, func>::rotate_r(PSrb_nodep<T, func> t)
	{
		Nodep pivot = t->l;
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
			Nodep t = path[i];
			Nodep p = path[i - 1];
			if (p->isRed == false)
				break;
			if (i < 3)
				break;
			Nodep g = path[i - 2];
			Nodep q = path[i - 3];
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
		Nodep t = path[i];
		while (i >= 3)
		{
			t = path[i];
			Nodep p = path[i-1];
			Nodep g = path[i - 2];
			bool left = g->l == p;
			Nodep w = t == p->r ? p->l : p->r;
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
	inline void PersistentSet_rb<T, func>::makeImage(string filename)
	{
		ofstream fout("tmgv", ios::out);
		fout << "digraph " + filename + " {" << endl;
		write_proc(roots.back(), fout, 1);
		fout << "}" << endl;
		fout.close();
		string fname = filename + ".png";
		//convert_dot_to_png("C:/Users/morga/source/repos/2_course_2_sem/algo_labs/B+_Tree/B+_Tree/tmgv", fname);
		string cd = get_cd();
		string command = "dot -Tpng " + cd + "/tmgv > " + cd + "/" + fname;
		SetCurrentDirectory((cd + "/bin").c_str());
		system(command.c_str());
		SetCurrentDirectory(cd.c_str());
		system(fname.c_str());
		system("del tmgv");
		_getch();
	}

	template<class T, typename func>
	inline PSrb_TreeIterator<T, func> PersistentSet_rb<T, func>::begin()
	{
		if (roots.size() == 0 || !roots.back())
			return TreeIterator(nullptr, nullptr);
		Nodep p = roots.back();
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
			Nodep newNode  = make_shared<Node>(x);
			roots.push_back(newNode);
			Size = 1;
			return true;
		}
		Nodep p = roots.back();
		vector<Nodep> path;
		size_t curSize = Size;
		copy_path(x, path, true);
		Nodep tmpRoot=make_shared<Node>(0);
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
		Nodep p = roots.back();
		if (!p)
			return false;
		if (Size == 1)
		{
			roots.push_back(nullptr);
			return true;
		}
		vector<Nodep> path;
		copy_path(x, path, false);
		if (path.size() == 0)
			return false;
		Nodep tmpRoot(new Node(0));
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
		Nodep p = roots.back();
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
		Nodep p = roots.back();
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
					Nodep tmp(new Node(x));
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
				Nodep tmp(new Node(x));
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
		Nodep p = roots.back();
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
		Nodep t = left ? parent->l : parent->r;
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
			Nodep p = predecessor_p(t);
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
		Nodep p = t;
		Nodep c = t->l;
		while (c->r)
		{
			p = c;
			c = c->r;
		}
		return p;
	}

	template<class T, typename func>
	inline void PersistentSet_rb<T, func>::write_proc(PSrb_nodep<T, func> p, ostream & file, int label)
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
	inline void PersistentSet_rb<T, func>::getInput()
	{
		int choise = 0;
		string message = "";
		do
		{
			system("cls");
			cout << message << endl;
			cout << "---TREE---" << endl;
			cout << "SIZE IS " << treeSize() << " NODES." << endl;
			if (treeSize() > 50)
				cout << "Size too big to print tree. You can write it to image file" << endl;
			else
				print();
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
				insert(data);
				message = "Successfully inserted " + to_string(data) + ".";
				break;
			case 2:
			{
				cout << "Enter data to remove >>>";
				cin >> data;
				bool success = remove(data);
				if (success)
					message = "Successfully removed " + to_string(data) + ".";
				else message = "Not found " + to_string(data) + ".";
				break;
			}
			case 3:
			{
				cout << "Enter data to search for >>>";
				cin >> data;
				PSrb_nodep<T, func> found = search(data);
				if (!found)
					message = "Not found " + to_string(data) + ".";
				else
					message = "Found " + to_string(data) + ".";
				break;
			}
			case 4:
				clear();
				message = "Tree is now empty.";
				break;
			case 5:
			{
				string filename;
				cout << "Enter file name to save image (without extenstion) >>>";
				cin >> filename;
				makeImage(filename);
				break;
			}
			case 6:
			{
				int errors_num = checkOrder();
				message = "Test complete. Number of errors : " + to_string(errors_num) + ".";
			}
			default:
				break;
			}
			cin.ignore(cin.rdbuf()->in_avail());
			cin.clear();
		} while (choise >= 1 && choise <= 6);
	}


	template<class T, typename func>
	inline int PersistentSet_rb<T, func>::checkOrder()
	{
		int error_num = 0;

		for (PSrb_TreeIterator<T, func> it = begin()+1; it != end(); ++it)
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
		Nodep p = ptr;
		for (int i = 0; i < N; ++i)
			p = successor();
		return TreeIterator(p, root);
	}

	template<class T, typename func>
	inline PSrb_TreeIterator<T, func> PersistentSet_rb<T, func>::TreeIterator::operator-(int N)
	{
		Nodep p = ptr;
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