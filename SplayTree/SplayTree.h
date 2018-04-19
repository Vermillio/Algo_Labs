#pragma once
#include <conio.h>
#include <string>
#include <utility>
#include <iostream>
#include <windows.h>
#include <memory>
using namespace std;

namespace Tree
{

	template<class T, class D>
	class NodeBase {
		T data;
		shared_ptr<Node> l, r, parent;
		NodeBase() {};
	};

	template<class T>
	class Node : public NodeBase<T, Node>
	{
		using Nodep = shared_ptr<Node>;

		Node	() {
			l = nullptr;
			r = nullptr;
			parent = nullptr;
		};
		
		Node	(T &x, 
				 Nodep _l = nullptr, 
				 Nodep _r = nullptr, 
				 Nodep _parent = nullptr) {
			l = nullptr;
			r = nullptr;
			parent = nullptr;
			data = x;
		};
		
		~Node() {}
	};


	template<class T, class NodeClass>
	class SplayTree
	{
		static_assert(is_base_of<NodeBase, NodeClass>::value, "NodeClass must inherit NodeBase");
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
		void							splay	(NodeClassp &t);
		pair<NodeClassp, NodeClassp>	split	(NodeClassp &t, const T &x);
		NodeClassp						merge	(NodeClassp &t1, NodeClassp &t2);
		void							print	(NodeClassp &p, int level);			///Print one line of tree (horisontally oriented)

	public:
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
		SplayTree	()	: root(nullptr), Size(0), selected(nullptr)	{	};
		~SplayTree	()	{ clear(); };
		
		template<class T>
		friend	class	BpTreeHandler;
	};



	template<class T, class NodeClass>
	class BpTreeHandler
	{

	private:

	public:
		///This function tests all SplayTree main functions.
		///Gets user's input from console and visualizes changes in a user-friendly way.
		///Parameter Tr is used for working with an existing tree to save result.
		///If Tr is not specified, tree is created automatically and gets deleted after returning from function.
		void	getInput	(SplayTree<T, NodeClass> *Tr = nullptr);
	};



}



template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::getSelected() const
{
	return selected;
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::getRoot() const
{
	if (root)
		return root;
	else return nullptr;
}

template<class T, class NodeClass>
inline int Tree::SplayTree<T, NodeClass>::getSize() const
{
	return Size;
}

//+

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
inline void Tree::SplayTree<T, NodeClass>::splay(NodeClassp &t)
{
	while (t != root)
	{
		NodeClassp p = t->parent;
		NodeClassp g = t->parent->parent;
		if (t->parent == root)															//	Zig
		{
			if (p->l == t)
				rotate_right(p);
			else 
				rotate_left(p);
		}
		else if (p->l == t && g->l == p)				//	Zig-Zig
		{
			rotate_right(p);
			rotate_right(g);
		}
		else if (p->r == t && g->r == p)
		{
			rotate_right(p);
			rotate_left(g);
		}
		else if (p->l == t && g->r == t->parent)				//	Zig-Zag
		{
			rotate_right(p);
			rotate_left(g);

		}
		else if (p->r == t && g->l == p)
		{
			rotate_left(p);
			rotate_right(g);
		}
	}


}

template<class T, class NodeClass>
inline pair<typename Tree::SplayTree<T, NodeClass>::NodeClassp, typename Tree::SplayTree<T, NodeClass>::NodeClassp> Tree::SplayTree<T, NodeClass>::split(NodeClassp &t, const T &x)
{
	if (!t)
		return pair<NodeClassp, NodeClassp>({nullptr, nullptr});
	NodeClassp p = search(t, x);
	if (p->data <= x)
	{
		NodeClassp r = root->r;
		if (root->r)
		{
			root->r->parent = nullptr;
			root->r = nullptr;
		}
		return pair<NodeClassp , NodeClassp>({ p, r });
	}
	else
	{
		NodeClassp l = root->l;
		if (root->l)
		{
			root->l->parent = nullptr;
			root->l = nullptr;
		}
		return pair<NodeClassp , NodeClassp>({ l, p });
	}
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
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::search(NodeClassp & t, const T & x)
{
	NodeClassp p = t;
	while (p != nullptr)
	{
		if (x > p->data)
		{
			if (p->r)
				p = p->r;
			else {
				splay(t, p);
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
				splay(t, p);
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
			splay(t, p);
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
typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::insert(const T &x)
{
	return insert(root, x);
}

template<class T, class NodeClass>
inline typename Tree::SplayTree<T, NodeClass>::NodeClassp Tree::SplayTree<T, NodeClass>::insert(NodeClassp & t, const T & x)
{
	if (!t)
	{
		t = make_shared<NodeClass>(x);
		++Size;
		selected = t;
		return t;
	}
	pair<NodeClassp, NodeClassp> subtrees = split(t, x);

	t = make_shared<NodeClass>(x);	
	
	t->l = subtrees.first;
	if (t->l)
		t->l->parent = t;

	t->r = subtrees.second;
	if (t->r)
		t->r->parent = t;


	++Size;
	selected = t;
	return t;
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
void Tree::SplayTree<T, NodeClass>::print(const NodeClassp &p, int level)
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
inline void Tree::BpTreeHandler<T, NodeClass>::getInput(SplayTree<T, NodeClass> *Tr)
{
	bool existing = true;
	if (!Tr)
	{
		Tr = new SplayTree<T>();
		existing = false;
	}
	int choise;
	string message = "";
	do
	{
		system("cls");
		cout << message << endl;
		cout << "---TREE---" << endl;
		cout << *Tr;
		cout << "SIZE IS " << Tr->getSize() << " NODES." << endl;
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
			Tr->insert(data);
			message = "Successfully inserted " + to_string(data) + ".";
			break;
		case 2:
		{
			cout << "Enter data to remove >>>";
			cin >> data;
			bool success = Tr->remove(data);
			if (success)
				message = "Successfully removed " + to_string(data) + ".";
			else message = "Not found " + to_string(data) + ".";
			break;
		}
		case 3:
			cout << "Enter data to search for >>>";
			cin >> data;
			if (Tr->search(data)->data == data)
				message = "Found " + to_string(data) + ".";
			else 
				message = "Not found " + to_string(data) + ".";
			break;
		case 4:
			Tr->clear();
			message = "Tree is now empty.";
			break;
		default:
			break;
		}
	} while (choise >= 1 && choise <= 4);
	if (!existing)
		delete Tr;
}



