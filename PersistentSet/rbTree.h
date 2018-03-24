#include "PersistentSet.h"
#pragma once
/*
Classic Red-Black tree implementation with addition of SUBTREE_SIZE field in the NODE structure 
and updating this field while insertion and deleting operations are executed.
This doesn't change the complexity of insertion, but adds log(n) to the deleting.
*/


using namespace std;

namespace pset
{

	enum	Dir {
		_l_ = 0,
		_r_ = 1
	};

	template<class T>
	struct	node 
	{
		//Is color of node red
		bool isRed;

		//Pointer to the value this node represents.
		T data;

		//Pointers to neighbour nodes.
		node **child;
		node *parent;


		//CONSTRUCTORS
		node();
		node(node<T> * t);
		node(T data);
		node(T data, node *l, node *r);
		~node();
	};


	template<class T>
	class RbTree
	{
	public:

		node<T>		*root;

		//Total number of nodes in the tree.
		int				TreeSize;


		//Horizontal console output of the tree. [NODE] <---> (value, subtree-size)
		void			print();


		//Look for value by pointer.
		node<T>		*search(T *data);


		//Clear all tree.
		void			clear();


		//Insert data into the tree.
		void			insert(T *data);


		//Remove node from the tree.
		void			remove(node<T> *t);


		//Remove data from the tree.
		void			remove(T *data);


	private:


		//Inner search procedure.
		node<T>			*search(node<T> *t, T *data);


		//Tree balancing functions.
		void			checkInsert(node<T> *t);


		void			checkRemove(node<T> *t);


		//Rotation functions. Used for tree balancing.
		node<T>		*rotate_once(node<T> *t, bool dir);


		node<T>		*rotate_twice(node<T> *t, bool dir);


		//Clear subtree of t.
		//Not for usage. It's inner procedure.
		void			clear(node<T> *t);

		//Displays error.
		void			errorMessage(int key);


		//procedure of printing line-by-line.
		void			consoleOutput(node<T> *root, int level);

	};

}


template<class T>
pset::node<T> * pset::RbTree<T>::rotate_once(pset::node<T> *t, bool dir)
{

	node<T> *pivot = t->child[!dir];
	t->child[!dir] = pivot->child[dir];
	if (pivot->child[dir])
		pivot->child[dir]->parent = t;
	pivot->child[dir] = t;
	pivot->parent = t->parent;
	t->parent = pivot;
	pivot->S = t->S;
	t->S = sz(t->child[_l_]) + sz(t->child[_r_]) + 1;
	t->isRed = 1;
	pivot->isRed = 0;
	return pivot;
}

template<class T>
pset::node<T>* pset::RbTree<T>::rotate_twice(pset::node<T> *t, bool dir)
{
	t->child[!dir] = rotate_once(t->child[!dir], !dir);
	return rotate_once(t, dir);
}


template<class T>
void   pset::RbTree<T>::checkInsert(pset::node<T> *t)
{
	if (!t)
		return;
	if (t->parent->isRed == 0)
		return;

	node<T> *g = t->parent->parent;
	if (g)
	{
		if (g->child[0] && g->child[1] && g->child[0]->isRed && g->child[1]->isRed)
		{
			g->isRed = 1;
			g->child[0]->isRed = g->child[1]->isRed = 0;
			checkInsert(g);
		}
		else
		{
			node<T> *p = g->parent;
			bool dir2 = p->child[1] == g;
			bool dir = g->child[0] != t->parent;
			if (g->child[dir]->child[!dir] == t)
				p->child[dir2] = reinterpret_cast<node<T> *>(rotate_twice(g, !dir));
			else
				p->child[dir2] = rotate_once(g, !dir);
		}
	}
}

template<class T>
inline void   pset::RbTree<T>::checkRemove(pset::node<T> * t)
{
	while (t != root && t->isRed == 0)
	{
		bool dir = t == t->parent->child[1];
		node<T> *w = t->parent->child[!dir];
		if (w->isRed == true)
		{
			w->isRed = false;
			t->parent->isRed = true;
			rotate_once(t->parent, dir);
			w = t->parent->child[!dir];
		}
		if (w->child[0]->isRed == false && w->child[1]->isRed == false)
		{
			w->isRed = true;
			t = t->parent;
		}
		else if (w->child[!dir]->isRed == false)
		{
			w->child[dir]->isRed = false;
			w->isRed = true;
			rotate_once(w, !dir);
		}
		w->isRed = t->parent->isRed;
		t->parent->isRed = false;
		w->child[1]->isRed = false;
		rotate_once(t->parent, 0);
		t = root;
	}
	t->isRed = 0;
}

template<class T>
inline void   pset::RbTree<T>::insert(T *data)
{
	if (!root)
	{
		root = new node<T>(data);
		++TreeSize;
		root->isRed = 0;
		return;
	}
	T data;
	node<T> *head = new node<T>(&data);
	node<T> *q = nullptr;
	bool dir = 0;
	q = head->child[1] = root;
	root->parent = head;
	head->isRed = 0;
	while (true)
	{
		dir = *q->data < *data;
		if (q->child[dir] == nullptr)
		{
			q->child[dir] = new node<T>(data);
			q->child[dir]->parent = q;
			++TreeSize;
			q = q->child[dir];
			break;
		}
		q = q->child[dir];
	}

	checkInsert(q);

	root = head->child[1];
	root->parent = nullptr;
	delete head; //remove extra root

	if (root->isRed != 0)
		root->isRed = 0;

	return;
}

template<class T>
void   pset::RbTree<T>::remove(pset::node<T> * t)
{
	node<T> *data = nullptr, *y = nullptr;
	
	if (!t)
		return;
	if (t->child[_l_] && t->child[_r_])
	{
		y = t->child[_l_];
		while (y->child[_r_])
			y = y->child[_r_];
		t->data = y->data;
		remove(y);
		return;
	}
	else y = t;
	reduceSize(t->parent);
	if (y->child[_l_] != nullptr)
		data = y->child[_l_];
	else
		data = y->child[_r_];

	if (!y->parent)
	{
		root = data;
	}
	else {
		if (y->parent->child[_l_] && y == y->parent->child[_l_])
			y->parent->child[_l_] = data;
		else
			y->parent->child[_r_] = data;
		if (data)
			data->parent = y->parent;
	}

	if (y != t)
	{
		t->data = y->data;
	}

	if (data && y->isRed == 0)
		checkRemove(data);
	delete y;
	--TreeSize;
}

template<class T>
inline void pset::RbTree<T>::remove(T * data)
{
	node<T> *t = search(data);
	if (!t)
		return;
	remove(t);
}


template<class T>
pset::node<T>::node() : parent(nullptr), isRed(true), S(1)
{
	child = new node<T>*[2];
	child[0] = nullptr;
	child[1] = nullptr;
}


template<class T>
inline pset::node<T>::node(node<T>* t)
{
	if (t == nullptr)
		return;
	isRed = t->isRed;
	data = t->data;
	child = new node<T>*[2];
	child[_l_] = t->child[_l_];
	child[_r_] = t->child[_r_];
	parent = t->parent;
	S = t->S;
}

template<class T>
pset::node<T>::node(T data) : data(data), child(new node<T>*[2]), parent(nullptr), isRed(true), S(1)
{
	child[_l_] = nullptr;
	child[_r_] = nullptr;
}

template<class T>
pset::node<T>::node(T data, pset::node<T> * l, pset::node<T> * r) : data(data), child(new node*[2]), parent(nullptr), isRed(true)
{
	child[0] = l;
	child[1] = r;
	S = size(l) + size(r) + 1;
}

template<class T>
inline pset::node<T>::~node()
{
	if (child)
		delete[] child;
}


template<class T>
void pset::RbTree<T>::clear(pset::node<T> *t)
{
	if (!t)
		return;
	clear(t->child[0]);
	clear(t->child[1]);
//	delete[] t->child;
//	t->child = nullptr;
	delete t;
	t = nullptr;
}

template<class T>
void pset::RbTree<T>::clear()
{
	clear(root);
	TreeSize = 0;
}

template<class T>
pset::node<T> * pset::RbTree<T>::search(pset::node<T> *t, T *data)
{
	if (t)
	{
		if (t->data == data)
			return t;
		else if (*t->data > *data)
			return search(t->child[0], data);
		else if (*t->data <= *data)
			return search(t->child[1], data);
	}
//	else
//		return nullptr;
	return nullptr;
}

template<class T>
pset::node<T> * pset::RbTree<T>::search(T * data)
{
	return search(root, data);
}

template<class T>
void pset::RbTree<T>::print()
{
	consoleOutput(root, 0);
	cout << endl;
}


template<class T>
void pset::RbTree<T>::errorMessage(int key)
{
	switch (key)
	{
	case 0:
		cout << "Couldn't open file." << endl;
		break;
	case 1:
		cout << "Incorrect tree file." << endl;
		break;
	}
}


template<class T>
void pset::RbTree<T>::consoleOutput(pset::node<T> *p, int level)
{
	if (p)
	{
		consoleOutput(p->child[_r_], level + 1);
		for (int i = 0; i < level; i++)
			cout << "     ";
		cout << "(" << *p->data << ", " << p->S << ")" << endl;
		consoleOutput(p->child[_l_], level + 1);
	}
	return;
}