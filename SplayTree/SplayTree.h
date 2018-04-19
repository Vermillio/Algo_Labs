#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <conio.h>
#include <string>
#include <utility>
#include <iostream>
#include <windows.h>
#include <memory>
using namespace std;

namespace Tree
{

	template<class T, class Node>
	class NodeBase {
	public:
		T data;
		shared_ptr<Node> l, r, parent;
		NodeBase() {};
	};

	template<class T>
	class Node : public NodeBase<T, Node<T>>
	{
	public:
		using Nodep = shared_ptr<Node>;

		Node() {
			l = r = parent = nullptr;
		}
		
		Node(const T &x, const Nodep &_l = nullptr, const Nodep &_r = nullptr, const Nodep &_parent = nullptr) {
			l = _l;
			r = _r;
			parent = _parent;
			data = x;
		}
		
		~Node() {}
	};

	template<class T, class NodeClass=Node<T>>
	class SplayTree
	{
		static_assert(is_base_of<NodeBase<T, NodeClass>, NodeClass>::value, "NodeClass must inherit NodeBase");
		using NodeClassp = shared_ptr<NodeClass>;

	protected:
		NodeClassp	root;
		int			Size;
		NodeClassp	selected;

		///Rotation functions. Used for tree balancing.
		void		rotate_left				(NodeClassp &curRoot, NodeClassp &t);
		void		rotate_right			(NodeClassp &curRoot, NodeClassp &t);

		NodeClassp		max_elem				(const NodeClassp &t) const;		///Find maximum element of subtree t.
		NodeClassp		min_elem				(const NodeClassp &t) const;		///Find minimum element of subtree t.
		NodeClassp		successor				(const NodeClassp &t) const;		///Find minimum element that is greater than t.
		NodeClassp		predecessor				(const NodeClassp &t) const;		///Find maximum element that is less than t.
		void							splay	(NodeClassp &curRoot, NodeClassp &t);
		pair<NodeClassp, NodeClassp>	split	(NodeClassp &t, const T &x);
		NodeClassp						merge	(NodeClassp &t1, NodeClassp &t2);
		void							print	(const NodeClassp &p, int level) const;			///Print one line of tree (horizontally oriented)

	public:
		SplayTree() : root(nullptr), Size(0), selected(nullptr) {	};
		~SplayTree() { clear(); };

		NodeClassp&		getSelected		()	const; 					///Get last changed element.
		NodeClassp&		getRoot			()	const;					///Get current tree root.
		size_t			getSize			()	const;					///Returns current tree size (number of nodes).
		NodeClassp		insert			(const T &x);				///Insert element x into tree. New element becomes root.
		NodeClassp		insert			(NodeClassp &t, const T &x);///Insert element x into arbitrary subtree t. New element becomes t.
		NodeClassp		search			(const T &x);				///Look up element x in the whole tree.
		NodeClassp		search			(NodeClassp &t, const T &x);/// Look up element x in arbitrary subtree t.
		bool			remove			(const T &x);				///Remove element x from the tree. Returns 1 - element is successfully removed. Returns 0 - element not found.
		void			remove			(NodeClassp &x);			///Remove node from the tree.
		void			clear			();							///Erase all nodes and set size to 0.
		void			print			();							///Print tree to the console (horizontally oriented)

		
		///This function tests all SplayTree main functions.
		///Gets user's input from console and visualizes changes in a user-friendly way.
		virtual void	getInput();
	};
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp& Tree::SplayTree<T, NodeClass>::getSelected() const
{
	return selected;
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp& Tree::SplayTree<T, NodeClass>::getRoot() const
{
	if (root)
		return root;
	else return nullptr;
}

template<class T, class NodeClass>
inline size_t Tree::SplayTree<T, NodeClass>::getSize() const
{
	return Size;
}

template<class T, class NodeClass>
inline void Tree::SplayTree<T, NodeClass>::rotate_left(NodeClassp &curRoot, NodeClassp &t)
{
	NodeClassp pivot = t->r;
	t->r = pivot->l;
	if (pivot->l)
		pivot->l->parent = t;
	pivot->l = t;
	pivot->parent = t->parent;
	if (t == curRoot)
		curRoot = pivot;
	else {
		if (t->parent->l == t)
			t->parent->l = pivot;
		else t->parent->r = pivot;
	}
	t->parent = pivot;
}

template<class T, class NodeClass>
inline void Tree::SplayTree<T, NodeClass>::rotate_right(NodeClassp &curRoot, NodeClassp &t)
{
	NodeClassp pivot = t->l;
	t->l = pivot->r;
	if (pivot->r)
		pivot->r->parent = t;
	pivot->r = t;
	pivot->parent = t->parent;
	if (t == curRoot)
		curRoot = pivot;
	else {
		if (t->parent->l == t)
			t->parent->l = pivot;
		else t->parent->r = pivot;
	}
	t->parent = pivot;
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::max_elem(const NodeClassp &t) const
{
	NodeClassp p = t;
	while (p->r)
		p = p->r;
	return p;
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::min_elem(const NodeClassp &t) const
{
	NodeClassp p = t;
	while (p->l)
		p = p->l;
	return p;
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::successor(const NodeClassp &t) const
{
	if (!t)
		return nullptr;
	else if (t->r)
		return min_elem(t->r);
	NodeClassp s = t;
	while (s != t->parent->l || s != root)
		s = s->parent;
	return s;
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::predecessor(const NodeClassp &t) const
{
	if (!t)
		return nullptr;
	else if (t->l)
		return max_elem(t->l);
	NodeClassp s = t;
	while (s != t->parent->r || s != root)
		s = s->parent;
	return s;
}

template<class T, class NodeClass>
inline void Tree::SplayTree<T, NodeClass>::splay(NodeClassp &curRoot, NodeClassp &t)
{
	while (t != curRoot)
	{
		NodeClassp p = t->parent;
		NodeClassp g = t->parent->parent;
		if (p == curRoot) {							//	Zig
			if (p->l == t)
				rotate_right(curRoot, p);
			else
				rotate_left(curRoot, p);
		}
		else if (p->l == t && g->l == p) {			//	Zig-Zig
			rotate_right(curRoot, p);
			rotate_right(curRoot, g);
		}
		else if (p->r == t && g->r == p) {
			rotate_left(curRoot, p);
			rotate_left(curRoot, g);
		}
		else if (p->l == t && g->r == t->parent) {	//	Zig-Zag
			rotate_right(curRoot, p);
			rotate_left(curRoot, g);
		}
		else if (p->r == t && g->l == p) {
			rotate_left(curRoot, p);
			rotate_right(curRoot, g);
		}
	}
}

template<class T, class NodeClass>
inline pair<typename Tree::SplayTree<T, NodeClass>::NodeClassp, typename Tree::SplayTree<T, NodeClass>::NodeClassp> Tree::SplayTree<T, NodeClass>::split(NodeClassp &curRoot, const T &x)
{
	if (!curRoot)
		return pair<NodeClassp, NodeClassp>({nullptr, nullptr});
	NodeClassp p = search(curRoot, x);
	if (p->data < x) {
		NodeClassp r = curRoot->r;
		if (curRoot->r) {
			curRoot->r->parent = nullptr;
			curRoot->r = nullptr;
		}
		return pair<NodeClassp , NodeClassp>({ p, r });
	}
	NodeClassp l = curRoot->l;
	if (curRoot->l) {
		curRoot->l->parent = nullptr;
		curRoot->l = nullptr;
	}
	return pair<NodeClassp , NodeClassp>({ l, p });
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp  Tree::SplayTree<T, NodeClass>::merge(NodeClassp &t1, NodeClassp &t2)
{
	if (!t1)
		return t2;
	if (!t2)
		return t1;
	NodeClassp p = max_elem(t1);
	splay(t1, p);
	p->r = t2;
	t2->parent = p;
	return p;
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp  Tree::SplayTree<T, NodeClass>::search(const T &x)
{
	return search(root, x);
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::search(NodeClassp & curRoot, const T & x)
{
	NodeClassp p = curRoot;
	while (p != nullptr)
	{
		if (x > p->data)
		{
			if (p->r)
				p = p->r;
			else {
				splay(curRoot, p);
				if (p->data == x)
					selected = p;
				else selected = nullptr;
				return p;
			}
		}
		else if (x < p->data)
		{
			if (p->l)
				p = p->l;
			else {
				splay(curRoot, p);
				if (p->data == x)
					selected = p;
				else selected = nullptr;
				return p;
			}
		}
		else
		{
			while (p->r && p->r->data == p->data)
				p = p->r;
			splay(curRoot, p);
			if (p->data == x)
				selected = p;
			else selected = nullptr;
			return p;
		}
	}
	selected = nullptr;
	return p;
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::insert(const T &x)
{
	return insert(root, x);
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::insert(NodeClassp & curRoot, const T & x)
{
	if (!curRoot)
	{
		curRoot = make_shared<NodeClass>(x);
		++Size;
		selected = curRoot;
		return curRoot;
	}
	pair<NodeClassp, NodeClassp> subtrees = split(curRoot, x);

	curRoot = make_shared<NodeClass>(x);	
	
	curRoot->l = subtrees.first;
	if (curRoot->l)
		curRoot->l->parent = curRoot;

	curRoot->r = subtrees.second;
	if (curRoot->r)
		curRoot->r->parent = curRoot;


	++Size;
	selected = curRoot;
	return curRoot;
}

template<class T, class NodeClass>
inline bool Tree::SplayTree<T, NodeClass>::remove(const T &x)
{
	NodeClassp p = search(x);
	if (!p || p->data != x)
	{
		selected = nullptr;
		return false;
	}
	remove(p);
	return true;
}

template<class T, class NodeClass>
inline void Tree::SplayTree<T, NodeClass>::remove(NodeClassp &x)
{
	if (!x)
		return;
	root = merge(x->l, x->r);
	x->parent = x->l = x->r = nullptr;
	x = nullptr;
	selected = nullptr;
	--Size;
}

template<class T, class NodeClass>
inline void Tree::SplayTree<T, NodeClass>::clear()
{
	root = nullptr;
	Size = 0;
	selected = nullptr;
}

template<class T, class NodeClass>
inline void Tree::SplayTree<T, NodeClass>::print()
{
	print(root, 0);
	cout << endl;
}

template<class T, class NodeClass>
inline void Tree::SplayTree<T, NodeClass>::print(const NodeClassp &p, int level) const
{
	if (p)
	{
		print(p->r, level + 1);
		for (int i = 0; i < level; i++)
			cout << "     ";
		if (p == selected)
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 11);
			cout << "[" << p->data << "]" << endl;
			SetConsoleTextAttribute(hConsole, 7);
		} else
			cout << "(" << p->data << ")" << endl;
		print(p->l, level + 1);
	}
	return;
}

template<class T, class NodeClass>
inline void Tree::SplayTree<T, NodeClass>::getInput()
{
	int choise;
	string message = "";
	do
	{
		system("cls");
		cout << message << endl;
		cout << "---TREE---" << endl;
		print();
		cout << "SIZE IS " << getSize() << " NODES." << endl;
		cout << endl;
		cout << "1 - INSERT, 2 - REMOVE, 3 - SEARCH, 4 - CLEAR, <ANYTHING ELSE> - EXIT" << endl;
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
			cout << "Enter data to search for >>>";
			cin >> data;
			if (search(data)->data == data)
				message = "Found " + to_string(data) + ".";
			else 
				message = "Not found " + to_string(data) + ".";
			break;
		case 4:
			clear();
			message = "Tree is now empty.";
			break;
		default:
			break;
		}
	} while (choise >= 1 && choise <= 4);
}

#endif