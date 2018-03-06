#pragma once
#include <conio.h>
#include <string>
#include <utility>
#include <iostream>
#include <windows.h>
using namespace std;

namespace SplTr
{
	template<class T>
	struct Node
	{
		T		data;
		
		Node	*l, *r, *parent;

		Node	()									: l(nullptr), r(nullptr), parent(nullptr)	{};
		
		Node	(T &x, 
				 Node *_l = nullptr, 
				 Node *_r = nullptr, 
				 Node * _parent = nullptr)			: l(_l), r(_r), parent(_parent), data(x)	{};
		
		~Node() {
														if (l)
														{
															l->parent = nullptr;
															delete l;
														}
														l = nullptr;
														if (r)
														{
															r->parent = nullptr;
															delete r;
														}
														if (parent)
															delete parent;
														parent = nullptr;
														};
	};


	template<class T>
	class SplayTree
	{

	private:
		Node<T>							*root;
		int								Size;
		Node<T>							*selected;

		//Rotation functions. Used for tree balancing.
		Node<T>							*rotate_left			(Node<T> *t);
		Node<T>							*rotate_right			(Node<T> *t);

		Node<T>							*max_elem				(Node<T> * t) const;
		Node<T>							*min_elem				(Node<T> * t) const;
		Node<T>							*successor				(Node<T> * t) const;
		Node<T>							*predecessor			(Node<T> * t) const;
		void							splay					(Node<T> * t);
		pair<Node<T>*, Node<T>*>		split					(Node<T> *t, T &x);
		Node<T>							*merge					(Node<T> *t1, Node<T> *t2);

	public:
		Node<T>							*getSelected			()	const;
		Node<T>							*getRoot				()	const;
		int								getSize					()	const;
		Node<T>							*insert					(T &x);
		Node<T>							*search					(T &x);
		bool							remove					(T &x);
		void							remove					(Node<T> *&x);
		void							clear					();

		//Auxiliary function for tree output.
		void							printLine(Node<T> *p, int level);

										SplayTree				()									: root(nullptr), Size(0), selected(nullptr)	{	};
										~SplayTree				()									{ clear(); };
		
		template<class T>
		friend	class					InputHandler;
	};



	template<class T>
	class InputHandler
	{

	private:

	public:
		void							run						(SplayTree<T> *Tr = nullptr);
	};



}


//Returns last changed element.
template<class T>
inline SplTr::Node<T>* SplTr::SplayTree<T>::getSelected() const
{
	return selected;
}

//Returns current tree root.
template<class T>
SplTr::Node<T>* SplTr::SplayTree<T>::getRoot() const
{
	if (root)
		return root;
	else return nullptr;
}

//Returns current tree size (number of nodes).
template<class T>
int SplTr::SplayTree<T>::getSize() const
{
	return Size;
}


//For balancing.
template<class T>
inline SplTr::Node<T>* SplTr::SplayTree<T>::rotate_left(Node<T>* t)
{
	Node<T> *pivot = t->r;
	t->r = pivot->l;
	if (pivot->l)
		pivot->l->parent = t;
	pivot->l = t;
	pivot->parent = t->parent;
	if (t == root)
		root = pivot;
	else {
		if (t->parent->l == t)
			t->parent->l = pivot;
		else t->parent->r = pivot;
	}
	t->parent = pivot;
	return pivot;
}


//For balancing.
template<class T>
inline SplTr::Node<T>* SplTr::SplayTree<T>::rotate_right(Node<T>* t)
{
	Node<T> *pivot = t->l;
	t->l = pivot->r;
	if (pivot->r)
		pivot->r->parent = t;
	pivot->r = t;
	pivot->parent = t->parent;
	if (t == root)
		root = pivot;
	else {
		if (t->parent->l == t)
			t->parent->l = pivot;
		else t->parent->r = pivot;
	}
	t->parent = pivot;
	return pivot;
}


//Find maximum element of subtree t.
template<class T>
inline SplTr::Node<T>* SplTr::SplayTree<T>::max_elem(Node<T>* t) const
{
	Node<T> *p = t;
	while (p->r)
		p = p->r;
	return p;
}


//Find minimum element of subtree t.
template<class T>
inline SplTr::Node<T>* SplTr::SplayTree<T>::min_elem(Node<T>* t) const
{
	Node<T> *p = t;
	while (p->l)
		p = p->l;
	return p;
}


//Find minimal element that is greater than t.
template<class T>
inline SplTr::Node<T>* SplTr::SplayTree<T>::successor(Node<T>* t) const
{
	if (!t)
		return nullptr;
	else if (t->r)
		return min_elem(t->r);
	Node<T> *s = t;
	while (s != t->parent->l || s != root)
		s = s->parent;
	return s;
}


//Find maximum element that is less than t.
template<class T>
inline SplTr::Node<T>* SplTr::SplayTree<T>::predecessor(Node<T>* t) const
{
	if (!t)
		return nullptr;
	else if (t->l)
		return max_elem(t->l);
	Node<T> *s = t;
	while (s != t->parent->r || s != root)
		s = s->parent;
	return s;
}


//Not for usage.
//Main balancing procedure.
template<class T>
inline void SplTr::SplayTree<T>::splay(Node<T>* t)
{
	while (t != root)
	{
		if (t->parent == root)															//	Zig
		{
			if (t->parent->l == t)
				rotate_right(t->parent);
			else 
				rotate_left(t->parent);
		}
		else if (t->parent->l == t && t->parent->parent->l == t->parent)				//	Zig-Zig
		{
			rotate_right(t->parent->parent);
			rotate_right(t->parent);
		}
		else if (t->parent->r == t && t->parent->parent->r == t->parent)
		{
			rotate_left(t->parent->parent);
			rotate_left(t->parent);
		}
		else if (t->parent->l == t && t->parent->parent->r == t->parent)				//	Zig-Zag
		{
			Node<T>*p = t->parent->parent;

			rotate_right(t->parent);
			rotate_left(p);

		}
		else if (t->parent->r == t && t->parent->parent->l == t->parent)
		{
			Node<T>*p = t->parent->parent;
			rotate_left(t->parent);
			rotate_right(p);
		}
	}


}

//Not for usage.
//Is used for insertion.
template<class T>
inline pair<SplTr::Node<T> *, SplTr::Node<T> *> SplTr::SplayTree<T>::split(Node<T>* t, T &x)
{
	if (!t)
		return pair<Node<T> *, Node<T> *>({nullptr, nullptr});
	Node<T> *p = search(x);
	if (p->data <= x)
	{
		Node<T> *r = root->r;
		if (root->r)
		{
			root->r->parent = nullptr;
			root->r = nullptr;
		}
		return pair<Node<T> *, Node<T> *>({ p, r });
	}
	else
	{
		Node<T> *l = root->l;
		if (root->l)
		{
			root->l->parent = nullptr;
			root->l = nullptr;
		}
		return pair<Node<T> *, Node<T> *>({ l, p });
	}
}


//Not for usage.
//Is used for removal.
template<class T>
inline SplTr::Node<T> * SplTr::SplayTree<T>::merge(Node<T>* t1, Node<T>* t2)
{
	if (!t1)
		return t2;
	if (!t2)
		return t1;
	Node<T> * p = max_elem(t1);
	splay(p);
	p->r = t2;
	t2->parent = p;
	return p;
}


//Look for element x in the tree.
template<class T>
inline SplTr::Node<T> * SplTr::SplayTree<T>::search(T & x)
{
	Node<T> *p = root;
	while (p != nullptr)
	{
		if (x > p->data)
		{
			if (p->r)
				p = p->r;
			else {
				splay(p);
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
				splay(p);
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
			splay(p);
			if (p->data == x)
				selected = p;
			else selected = nullptr;
			return p;
		}
	}
	selected = nullptr;
	return nullptr;
}




//Insert element x into the tree.
//New element becomes root.
template<class T>
SplTr::Node<T> *SplTr::SplayTree<T>::insert(T & x)
{
	if (!root)
	{
		root = new Node<T>(x);
		Size = 1;
		selected = root;
		return root;
	}
	Node<T> *p = root;

	pair<Node<T> *, Node<T> *> subtrees = split(root, x);
	root = new Node<T>(x);

	root->l = subtrees.first;
	if (root->l)
		root->l->parent = root;

	root->r = subtrees.second;
	if (root->r)
		root->r->parent = root;
	++Size;
	selected = root;
	return root;
}


//Remove element x from the tree.
//Returns 1 - element is successfully removed.
//Returns 0 - element not found.
template<class T>
inline bool SplTr::SplayTree<T>::remove(T & x)
{
	Node<T> *p = search(x);
	if (!p || p->data != x)
	{
		selected = nullptr;
		return 0;
	}
	remove(p);
	return 1;
}


//Remove node from the tree.
template<class T>
inline void SplTr::SplayTree<T>::remove(Node<T> * &x)
{
	if (!x)
		return;
	root = merge(x->l, x->r);
	x->parent = x->l = x->r = nullptr;
	delete(x);
	x = nullptr;
	selected = nullptr;
	--Size;
}

//Release the memory.
template<class T>
inline void SplTr::SplayTree<T>::clear()
{
	if (root)
		delete(root);
	root = nullptr;
	Size = 0;
	selected = nullptr;
}




template<class T>
ostream & operator<<(ostream & c, SplTr::SplayTree<T>& tr)
{
	tr.printLine(tr.getRoot(), 0);
	cout << endl;
	return c;
}

//Print one line of tree. (horizontal oriented)
template<class T>
void SplTr::SplayTree<T>::printLine(Node<T> *p, int level)
{
	if (p)
	{
		printLine(p->r, level + 1);
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
		printLine(p->l, level + 1);
	}
	return;
}

//
template<class T>
inline void SplTr::InputHandler<T>::run(SplayTree<T> *Tr)
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



