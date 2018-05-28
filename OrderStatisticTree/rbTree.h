#pragma once
/*
Classic Red-Black tree implementation with addition of SUBTREE_SIZE field in the NODE structure 
and updating this field while insertion and deleting operations are executed.
This doesn't change the complexity of insertion, but adds log(n) to the deleting.
*/
#include <memory>


using namespace std;

namespace OST
{

	enum	Dir {
		_l_ = 0,
		_r_ = 1
	};

	template<class T>
	struct	Node 
	{
		using node = Node<T>;
		using nodep = shared_ptr<node>;
		using nodepp = shared_ptr<shared_ptr<node>>;

		//Is color of node red
		bool red;

		//Value this node represents.
		T data;

		//Pointers to neighbour nodes.
		nodep *child;
		nodep parent;

		//Size of sub-tree.
		int S;


		//CONSTRUCTORS
		Node() : S(1), red(false), parent(nullptr) {
			child = new nodep[2];
			child[0] = nullptr;
			child[1] = nullptr;
		};

		Node(nodep t) {
			if (t == nullptr)
				return;
			red = t->red;
			data = t->data;
			child = new node*[2];
			child[_l_] = t->child[_l_];
			child[_r_] = t->child[_r_];
			parent = t->parent;
			S = t->S;
		};
		
		Node(const T &x) : red(false), data(x), S(1), child(new nodep[2]), parent(nullptr) {
			child[_l_] = nullptr;
			child[_r_] = nullptr;
		};

		Node(const T &x, nodep l, nodep r) : red(false), data(x), child(new nodep[2]), parent(nullptr) {
			child[0] = l;
			child[1] = r;
			S = size(l) + size(r) + 1;
		};

		~Node() {
			if (child)
				delete[] child;
		};
	};

	template<class T>
	int sz(typename Node<T>::nodep t) {
		if (!t)
			return 0;
		else return t->S;
	}

	template<class T>
	class RbTree
	{
	public:

		using node = Node<T>;
		using nodep=shared_ptr<Node<T>>;

		nodep		root;

		//Total number of nodes in the tree.
		int				TreeSize;


		//Horizontal console output of the tree. [NODE] <---> (value, subtree-size)
		void			print();


		//Look up value.
		nodep		search(const T &data);


		//Clear all tree.
		void			clear();


		//Insert data into the tree.
		void			insert(const T &data);


		//Remove node from the tree.
		void			remove(nodep t);


		//Remove data from the tree.
		void			remove(const T &data);


	private:


		//Inner search procedure.
		nodep			search(nodep t, const T &data);


		//Used after element removal to update subtree sizes (S field of structure node).
		void			reduceSize(nodep t);


		//Tree balancing functions.
		void			checkInsert(nodep t);


		void			checkRemove(nodep t);


		//Rotation functions. Used for tree balancing.
		nodep		rotate_once(nodep t, bool dir);


		nodep		rotate_twice(nodep t, bool dir);


		//Clear subtree of t.
		//Not for usage. It's inner procedure.
		void			clear(nodep t);

		//Displays error.
		void			errorMessage(int key);


		//procedure of printing line-by-line.
		void			consoleOutput(nodep root, int level);

	};


}



template<class T>
typename OST::RbTree<T>::nodep OST::RbTree<T>::rotate_once(typename OST::RbTree<T>::nodep t, bool dir)
{

	nodep pivot = t->child[!dir];
	t->child[!dir] = pivot->child[dir];
	if (pivot->child[dir])
		pivot->child[dir]->parent = t;
	pivot->child[dir] = t;
	pivot->parent = t->parent;
	t->parent = pivot;
	t->S = sz<T>(t->child[_l_]) + sz<T>(t->child[_r_]) + 1;
	pivot->S = sz<T>(pivot->child[dir]) + sz<T>(pivot->child[!dir])+1;
	t->red = 1;
	pivot->red = 0;
	return pivot;
}

template<class T>
typename OST::RbTree<T>::nodep OST::RbTree<T>::rotate_twice(typename OST::RbTree<T>::nodep t, bool dir)
{
	t->child[!dir] = rotate_once(t->child[!dir], !dir);
	return rotate_once(t, dir);
}

template<class T>
inline void OST::RbTree<T>::reduceSize(typename OST::RbTree<T>::nodep t)
{
	while (t->parent)
	{
		--t->S;
		t = t->parent;
	}
	--t->S;
}

template<class T>
void   OST::RbTree<T>::checkInsert(typename OST::RbTree<T>::nodep t)
{
	if (!t)
		return;
	if (t->parent->red == 0)
		return;

	nodep g = t->parent->parent;
	if (g)
	{
		if (g->child[0] && g->child[1] && g->child[0]->red && g->child[1]->red)
		{
			g->red = 1;
			g->child[0]->red = g->child[1]->red = 0;
			checkInsert(g);
		}
		else
		{
			nodep p = g->parent;
			bool dir2 = p->child[1] == g;
			bool dir = g->child[0] != t->parent;
			if (g->child[dir]->child[!dir] == t)
				p->child[dir2] = rotate_twice(g, !dir);
			else
				p->child[dir2] = rotate_once(g, !dir);
		}
	}
}

template<class T>
inline void   OST::RbTree<T>::checkRemove(typename OST::RbTree<T>::nodep t)
{
	while (t != root && t->red == 0)
	{
		bool dir = t == t->parent->child[1];
		nodepw = t->parent->child[!dir];
		if (w->red == true)
		{
			w->red = false;
			t->parent->red = true;
			rotate_once(t->parent, dir);
			w = t->parent->child[!dir];
		}
		if (w->child[0]->red == false && w->child[1]->red == false)
		{
			w->red = true;
			t = t->parent;
		}
		else if (w->child[!dir]->red == false)
		{
			w->child[dir]->red = false;
			w->red = true;
			rotate_once(w, !dir);
		}
		w->red = t->parent->red;
		t->parent->red = false;
		w->child[1]->red = false;
		rotate_once(t->parent, 0);
		t = root;
	}
	t->red = 0;
}

template<class T>
inline void   OST::RbTree<T>::insert(const T &data)
{
	if (!root)
	{
		root = make_shared<node>(data);
		++TreeSize;
		root->red = 0;
		return;
	}
	T x;
	nodep head = make_shared<node>(x);
	nodep q = nullptr;
	bool dir = 0;
	q = head->child[1] = root;
	root->parent = head;
	head->red = 0;
	while (true)
	{
		dir = q->data <= data;
		if (q->child[dir] == nullptr)
		{
			q->child[dir] = make_shared<node>(data);
			q->child[dir]->parent = q;
			++TreeSize;
			++q->S;
			q = q->child[dir];
			break;
		}
		++q->S;
		q = q->child[dir];
	}

	checkInsert(q);

	root = head->child[1];
	root->parent = nullptr;

	if (root->red != 0)
		root->red = 0;

	return;
}

template<class T>
void   OST::RbTree<T>::remove(typename OST::RbTree<T>::nodep t)
{
	nodepx = nullptr, *y = nullptr;
	
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
		x = y->child[_l_];
	else
		x = y->child[_r_];

	if (!y->parent)
	{
		root = x;
	}
	else {
		if (y->parent->child[_l_] && y == y->parent->child[_l_])
			y->parent->child[_l_] = x;
		else
			y->parent->child[_r_] = x;
		if (x)
			x->parent = y->parent;
	}

	if (y != t)
	{
		t->data = y->data;
	}

	if (x && y->red == 0)
		checkRemove(x);
	delete y;
	--TreeSize;
}

template<class T>
inline void OST::RbTree<T>::remove(const T &data)
{
	nodep t = search(data);
	if (!t)
		return;
	remove(t);
}

template<class T>
void OST::RbTree<T>::clear(typename OST::RbTree<T>::nodep t)
{
	if (!t)
		return;
	clear(t->child[0]);
	clear(t->child[1]);
	t = nullptr;

}

template<class T>
void OST::RbTree<T>::clear()
{
	clear(root);
	TreeSize = 0;
}

template<class T>
typename OST::RbTree<T>::nodep OST::RbTree<T>::search(typename OST::RbTree<T>::nodep t, const T &data)
{
	if (t)
	{
		if (t->data == data)
			return t;
		else if (t->data < data)
			return search(t->child[_r_], data);
		else if (t->data >= data)
			return search(t->child[_l_], data);
	}
	return nullptr;
}

template<class T>
typename OST::RbTree<T>::nodep OST::RbTree<T>::search(const T & data)
{
	return search(root, data);
}

template<class T>
void OST::RbTree<T>::print()
{
	consoleOutput(root, 0);
	cout << endl;
}

template<class T>
void OST::RbTree<T>::errorMessage(int key)
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
void OST::RbTree<T>::consoleOutput(typename OST::RbTree<T>::nodep p, int level)
{
	if (p)
	{
		consoleOutput(p->child[_r_], level + 1);
		for (int i = 0; i < level; i++)
			cout << "     ";
		cout << "(" << p->data << ", " << p->S << ")" << endl;
		consoleOutput(p->child[_l_], level + 1);
	}
	return;
}