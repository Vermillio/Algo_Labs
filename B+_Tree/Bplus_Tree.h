///B+tree class.
#pragma once
#define _CRT_SECURE_NO_WARNINGS


#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <Queue>

using namespace std;

	template<class KeyClass, class ValClass>
	struct Node
	{
		KeyClass *keys;

		ValClass *vals;

		bool	leaf;

		Node	*parent;
		
		Node	**links;
		
		int		size;
		int		linksSize;
		int		dataSize;

		//Binary search.
		//Returns position of the first occurance of k or place after it's predecessor.
		//Returned pos is connected to keys field.
		int								binSearch				(KeyClass &keys, bool first);

		//Get first occurance of an element placed on position pos.
		int								catchFirstOcc			(int pos);

		//Get last occurance of an element placed on position pos.
		int								catchLastOcc			(int pos);

		//Update every data field (need if sons are leafs).
		void							updateKey				();

		//Insert element into keys array.
		void							push					(KeyClass &elem, int pos);

		//Insert node into links array.
		void							push					(Node<KeyClass, ValClass> *p, int pos);

		void push(const ValClass & v, int pos);


		//Erase element on position pos.
		void							erase					(int pos, bool delete_node);

		//Constructors
										Node					() : links(nullptr), size(0), parent(nullptr), keys(nullptr), leaf(true) {};

										Node					(	bool _leaf = false,
																	int Capacity = 0,
																	int _size = 0,
																	Node ** _keys = nullptr,
																	KeyClass *_data = nullptr,
																	Node *_parent = nullptr	) : links(_keys), parent(_parent), leaf(_leaf), 
																								size(_size), keys(_data)			
																								{
																									if (Capacity != 0)
																										dataSize = Capacity + 1;
																									keys = new KeyClass[dataSize];
																									vals = new ValClass[dataSize];
																									if (!leaf)
																									{
																										linksSize = Capacity + 1;
																										links = new Node<KeyClass, ValClass>*[linksSize];
																										for (int i = 0; i < linksSize; ++i)
																											links[i] = nullptr;
																									}
																									else {
																										linksSize = 1;
																										links = new Node<KeyClass, ValClass>*();
																										*links = nullptr;
																									}
										}

		//Destructor			
		~Node()	
		{	
				parent = nullptr;
				delete[] keys;
				keys = nullptr;
				delete[] vals;
				vals = nullptr;
				if (!leaf && links)
				{
					for (int i = 0; i < size; ++i)
					{
						if (links[i])
						{
							links[i]->parent = nullptr;
							delete links[i];
							links[i] = nullptr;
						}
					}
					delete[] links;
					links = nullptr;
				}
				else if (leaf)
				{
					if (links && *links) {
						*links = nullptr;
						delete links;
					}
					links = nullptr;
				}
				cout << "Deleted.";
			};
	};


	template<class KeyClass, class ValClass>
	class BpTree
	{

	private:

		//Max node size.
		int	Capacity;

		//Tree root.
		Node<KeyClass, ValClass> *root;

		//Current number of nodes.
		int	size;

		//Delete node p without deleting nodes linked to p.
		void cutNode (Node<KeyClass, ValClass> *p);

		//Merge nodes on positions pos and pos+1.
		Node<KeyClass, ValClass>		*mergeWithNext	(Node<KeyClass, ValClass> *&p, int pos);

		//After-deletion procedure.
		void							merge			(Node<KeyClass, ValClass> *p);

		//After-insertion procedure.
		void							split			(Node<KeyClass, ValClass> *p);

		//Split node p by 2.
		Node<KeyClass, ValClass>	*makeSecondNode		(Node<KeyClass, ValClass> *&p);
		
		//If root is full, add new root. Increases tree height.
		void	createNewRoot();

	public:

		//Main test function
		void Test();

		//Access to Capacity field.
		int							getCapacity	()	const;

		//Access to root field.
		Node<KeyClass, ValClass>	*getRoot()	const;

		//Access to size field.
		int								getSize	()	const;

		//Insert element x.
		void							insert	(KeyClass &key, ValClass & v);

		//Search for element x.
		//If x is found returns node which contains x
		//If x is not found returns nullptr.
		//If index != nullptr it receives a position of x in returned node.
		Node<KeyClass, ValClass>	*search	(KeyClass &key, int *index);

		ValClass * search(KeyClass &key);

		//Remove element x.
		//Returns true if x is removed.
		//Returns false if x is not found.
		bool							remove	(KeyClass &key);

		//Free all allocated memory.
		void							clear	();

		//Constructors
		BpTree	(int _capacity) :
					root(nullptr), 
					size(0) 	{ 
									if (_capacity < 2)
										Capacity = 2;
									else Capacity = _capacity;
								};

		//Destructor
		~BpTree	()	{ clear(); };

	};


	template<class KeyClass, class ValClass>
	class BpTreeHandler
	{

	private:

		queue<int> q;

		void write_proc(Node<KeyClass, ValClass> *p, ofstream *file, int label, int capacity);
		void writeLabels(ofstream *file);


	public:

		//This function tests all BpTree main functions.
		//Gets user's input from console and visualizes changes in a user-friendly way.
		//Parameter Tr is used for working with an existing tree to save result.
		//If Tr is not specified, tree is created automatically and gets deleted.
		void							getInput(BpTree<KeyClass, ValClass> *Tr = nullptr);



		//This function performs tree visualization. It writes it into a DOT file, and then pushes the file to GraphViz app.
		//Format is PNG.
		void write_tree_to_image_file(BpTree<KeyClass, ValClass> *Tr, string filename);

		string get_cd();
	};

	template<class KeyClass, class ValClass>
	inline void Node<KeyClass, ValClass>::updateKey()
	{
		if (leaf && parent)
		{
			parent->updateKey();
			return;
		}
		if (size == 2)
			if (links[0]->size == 0)
				keys[0] = links[1]->keys[0];
			else
				keys[0] = links[0]->keys[0];
		else
			for (int i = 0; i < size - 1; ++i)
				keys[i] = links[i+1] ? links[i + 1]->keys[0] : keys[i-1];

		if (parent)
			parent->updateKey();
	}

	template<class KeyClass, class ValClass>
	int Node<KeyClass, ValClass>::binSearch(KeyClass &k, bool first)
	{
		int sz = leaf ? size : size-1;
		int r = sz;
		int l = 0;

		if (k < keys[0])
			return 0;

		if (k > keys[r - 1])
			return sz;

		while (r - l >= 1)
		{
			int m = (r + l) / 2;
			if (keys[m] > k)
				r = m;
			else if (keys[m] == k)
			{
				return first ? catchFirstOcc(m) : catchLastOcc(m);
			}
			else if (keys[m] < k && (m + 1 >= sz-1 || k < keys[m + 1]))
			{
				return m+1;
			}
			else
				l = m + 1;
		}
		return l;
	}

	template<class KeyClass, class ValClass>
	inline int Node<KeyClass, ValClass>::catchFirstOcc(int pos)
	{
		if (pos >= size || pos <= 1)
			return pos;
		while (keys[pos-1] == keys[pos - 2])
			--pos;
		return pos;
	}

	template<class KeyClass, class ValClass>
	inline int Node<KeyClass, ValClass>::catchLastOcc(int pos)
	{
		if (pos >= size-1 || pos <= 0)
			return pos;
		while (keys[pos-1] == keys[pos])
			++pos;
		return pos;
	}

	template<class KeyClass, class ValClass>
	inline void Node<KeyClass, ValClass>::push(KeyClass & key, int pos)
	{
		if (leaf)
		{
			for (int i = size; i > pos; --i)
				keys[i] = keys[i - 1];
			++size;
		} else
			for (int i = size-1; i > pos; --i)
				keys[i] = keys[i - 1];
		keys[pos] = key;
 //?
//		updateKey();
	}

	template<class KeyClass, class ValClass>
	inline void Node<KeyClass, ValClass>::push(Node<KeyClass, ValClass>* key, int pos)
	{
		if (leaf)
			return;
		for (int i = size; i > pos; --i)
			links[i] = links[i - 1];
		links[pos] = key;
		key->parent = this;
		++size;
		updateKey();
	}

	template<class KeyClass, class ValClass>
	inline void Node<KeyClass, ValClass>::push(const ValClass & v, int pos)
	{
		if (leaf)
		{
			for (int i = size; i > pos; --i)
				vals[i] = vals[i - 1];
		}
		else
			for (int i = size - 1; i > pos; --i)
				vals[i] = vals[i - 1];
		vals[pos] = v;
	}

	template<class KeyClass, class ValClass>
	inline void BpTree<KeyClass, ValClass>::Test()
	{
		int choise;
		string message = "";
		do
		{
			system("cls");
			cout << message << endl;
			cout << "---Tree---" << endl;
			//Tr->print();
			cout << "total size = " << getSize() << " nodes." << endl;
			cout << endl;
			cout << "1 - insert, 2 - remove, 3 - search, 4 - clear, 5 - show image (graphviz, make sure you have /bin dir under source path), <other num> - exit" << endl;
			cout << ">>>";

			cin >> choise;
			KeyClass key;
			ValClass val;
			switch (choise)
			{
			case 1:
				cout << "Enter key >>>";
				cin >> key;
				cout << "Enter value >>>";
				cin >> val;
				insert(key, val);
				message = "Successfully inserted.";
				break;
			case 2:
			{
				cout << "Enter key to remove >>>";
				cin >> key;
				bool success = remove(key);
				if (success)
					message = "Successfully removed.";
				else message = "Not found.";
				break;
			}
			case 3:
			{
				cout << "Enter key to search >>>";
				cin >> key;
				int val;
				int *index = &val;
				ValClass * found = search(key);
				if (!found)
					message = "Not found."; //Found instead " << to_string(found->data[*index]) << ".";
				else
					message = "Found. Value = " + to_string(*found);
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
				BpTreeHandler<KeyClass, ValClass> hnd;
				hnd.write_tree_to_image_file(this, filename);
				break;
			}
			default:
				break;
			}
		} while (choise >= 1 && choise <= 5);
	}

	template<class KeyClass, class ValClass>
	inline void Node<KeyClass, ValClass>::erase(int pos, bool delete_node)
	{
		if (!leaf)
		{
			if (delete_node)
			{
				links[pos]->parent = nullptr;
				delete links[pos];
				links[pos] = nullptr;
			}
			for (int i = pos+1; i < size; ++i)
				links[i - 1] = links[i];
			for (int i=size; i<linksSize; ++i)
				links[i] = nullptr;
			if (pos != size - 1)
				for (int i = pos + 1; i < size - 1; ++i) {
					keys[i - 1] = keys[i];
					vals[i - 1] = vals[i];
				}
	//		keys[size-1] = nullptr;
		}
		else {
			for (int i = pos + 1; i < size; ++i) {
				keys[i - 1] = keys[i];
				vals[i - 1] = vals[i];
			}
		}
		--size;
		updateKey();
	}

	template<class KeyClass, class ValClass>
	inline void BpTree<KeyClass, ValClass>::cutNode(Node<KeyClass, ValClass>* p)
	{
		if (!p->leaf)
		{
			for (int i = 0; i < Capacity; ++i)
				p->links[i] = nullptr;

			delete[] p->links;
			p->links = nullptr;
		}
		else p->links = nullptr;
		p->parent = nullptr;
		delete p;
		int k = 0;
	}
	
	template<class KeyClass, class ValClass>
	inline Node<KeyClass, ValClass> * BpTree<KeyClass, ValClass>::mergeWithNext(Node<KeyClass, ValClass> *&p, int pos)
	{
		if (p->leaf)
			return nullptr;

		Node<KeyClass, ValClass> *m = p->links[pos];
		Node<KeyClass, ValClass> *n = p->links[pos + 1];

		if (!m->leaf)
		{
			m->push(n->keys[0], m->size - 1);
			m->push(n->vals[0], m->size - 1);
			if (n->size >= 2)
				for (int i = 1; i < n->size - 2; ++i) {
					m->push(n->keys[i], m->size + i);
					m->push(n->vals[i], m->size + i);
				}
			for (int i = 0; i < n->size; ++i)
				m->push(n->links[i], m->size);
		}
		else
		{
			for (int i = 0; i < n->size; ++i) {
				m->push(n->keys[i], m->size);
				m->push(n->vals[i], m->size);
			}
			m->links = n->links;
			*n->links = nullptr;
			//delete n->keys;
			//n->keys = nullptr;
		}

		if (m->leaf && p)
			p->updateKey();
		if (p != root || p->size>2)
			p->erase(pos + 1, false);
		else {
			root = m;
			cutNode(p);
			p = nullptr;
			m->parent = nullptr;
		}
		cutNode(n);
		return m;
	}

	template<class KeyClass, class ValClass>
	inline void BpTree<KeyClass, ValClass>::merge(Node<KeyClass, ValClass>* p)
	{
		if (!p)
			return;
		if (p == root && size == 0)
		{
			cutNode(p);
			root = nullptr;
			return;
		}
		while (p && p->parent && p->size < Capacity - 1)
		{
			int pos = p->parent->binSearch(p->keys[0], true);
			//if (pos == 0)
			//	break;
			while (pos < p->parent->size && p->parent->links[pos] != p)
				++pos;
			if (pos >= p->parent->size)
				break;

			Node<KeyClass, ValClass> *l = pos-1<0 ? nullptr : p->parent->links[pos - 1];
			Node<KeyClass, ValClass> *r = pos+1 == p->parent->size ? nullptr : p->parent->links[pos + 1];
			if (l && p->size+l->size + p->leaf <= Capacity) // && size1+size2>=Capacity-1
			{
				p = p->parent;
				mergeWithNext(p, pos-1);
			}
			else if (r && p->size+r->size+p->leaf <= Capacity) // && size1+size2>=Capacity-1
			{
				p = p->parent;
				mergeWithNext(p, pos);
			}
			else if (l && l->size > 1)
			{
				if (!l->leaf)
				{
					p->push(l->links[l->size - 1], 0);
					p->push(l->keys[l->size - 2], 0);
					p->push(l->vals[l->size - 2], 0);
				}
				else {
					p->push(l->keys[l->size - 1], 0);
					p->push(l->vals[l->size - 1], 0);
				}
				l->erase(l->size - 1, false);
				p = p->parent;
			}
			else if (r && r->size > 1) {
				if (!r->leaf)
				{
					p->push(r->links[0], p->size);
					p->push(r->keys[0], p->size - 1);
					p->push(r->vals[0], p->size - 1);
				}
				else {
					p->push(r->keys[0], p->size);
					p->push(r->vals[0], p->size);
				}
				r->erase(0, false);
				p = p->parent;
			}
		}
	}
	
	template<class KeyClass, class ValClass>
	inline void BpTree<KeyClass, ValClass>::split(Node<KeyClass, ValClass>* p)
	{
		
		if (!p->leaf && p->size - 1 < Capacity)
			return;
		if (p->size < Capacity)
			return;
		while (p->parent && ((p->leaf && p->size >= Capacity) || (!p->leaf && p->size > Capacity)))
		{
//			delete p;
			Node<KeyClass, ValClass> *k = makeSecondNode(p);
			if (!k->leaf)
			{
				int pos = p->parent->binSearch(k->keys[0], false);
				if (k->keys[0] < p->parent->keys[pos]) {
					p->parent->push(k, pos+1);
					p->parent->push(k->keys[0], pos);
					p->parent->push(k->vals[0], pos);
				}
				else {
					p->parent->push(k, pos + 1);
					p->parent->push(k->keys[0], pos + 1);
					p->parent->push(k->vals[0], pos+1);
				}
				k->erase(0, false);
			}
			else {
				int pos = p->parent->binSearch(k->keys[0], false);
				if (k->keys[0] < p->parent->keys[pos]) {
					p->parent->push(k->keys[0], pos);
					p->parent->push(k->vals[0], pos);
					p->parent->push(k, pos+1);
				}
				else {
					p->parent->push(k->keys[0], pos + 1);
					p->parent->push(k->vals[0], pos+1);
					//				p->parent->push(p, pos);
					p->parent->push(k, pos + 1);
				}
			}
			p = p->parent;
//			print();
		}

		createNewRoot();
	}

	template<class KeyClass, class ValClass>
	inline Node<KeyClass, ValClass>* BpTree<KeyClass, ValClass>::makeSecondNode(Node<KeyClass, ValClass>* &p)
	{
		Node<KeyClass, ValClass> *k = nullptr;
		if (p->leaf)
		{
			k = new Node<KeyClass, ValClass>(true, Capacity, ((p->size % 2 == 0) ? p->size / 2 : p->size / 2 + 1)); //-> p -> k ->
			for (int i = 0; i < k->size; ++i) {
				k->keys[i] = p->keys[p->size - k->size + i];
				k->vals[i] = p->vals[p->size - k->size + i];
			}
			p->size -= k->size;
			Node<KeyClass, ValClass> ** tmp = k->links;
			k->links = p->links;
			p->links = tmp;
			*p->links = k;
		}
		else {
			k = new Node<KeyClass, ValClass>(false, Capacity, (p->size % 2 == 1) ? p->size / 2 : p->size / 2 + 1);
			for (int i = 0; i < k->size; ++i)
			{
				k->links[i] = p->links[p->size - k->size + i];
				if (i != 0 && k->links[i]) {
					p->links[p->size - k->size + i] = nullptr;
					k->links[i]->parent = k;
				}
			}
			//for (int i = k->size-1; i < p->size; ++i)
			//	p->keys[i] = nullptr;
			for (int i = 0; i < k->size - 1; ++i) {
				k->keys[i] = p->keys[p->size - k->size + i];
				k->vals[i] = p->vals[p->size - k->size + i];
			}
			p->size -= k->size - 1;
		}
		k->parent = p->parent;
		return k;
	}

	

	template<class KeyClass, class ValClass>
	inline void BpTree<KeyClass, ValClass>::createNewRoot()
	{
		if (!root->leaf && root->size-1 < Capacity)
			return;
		if (root->size < Capacity)
			return;
		//delete root;


		Node<KeyClass, ValClass> *k = makeSecondNode(root);
//		delete k;
		
		
		Node<KeyClass, ValClass> * new_root = new Node<KeyClass, ValClass>(false, Capacity, 2);
		new_root->links[0] = root;
		new_root->links[1] = k;
		if (!root->leaf)
		{
			new_root->keys[0] = k->keys[0];
			k->erase(0, false);
		}
		else 
			new_root->updateKey();

		k->parent = new_root;
		root->parent = new_root;
		//delete root;
		//delete k;
		root = new_root;
		++size;
	}

	template<class KeyClass, class ValClass>
	inline int BpTree<KeyClass, ValClass>::getCapacity() const
	{
		return Capacity;
	}

	template<class KeyClass, class ValClass>
	inline Node<KeyClass, ValClass>* BpTree<KeyClass, ValClass>::getRoot() const
	{
		return root;
	}

	template<class KeyClass, class ValClass>
	inline int BpTree<KeyClass, ValClass>::getSize() const
	{
		return size;
	}

	template<class KeyClass, class ValClass>
	inline void BpTree<KeyClass, ValClass>::insert(KeyClass & t, ValClass & v)
	{
		int index;
		Node<KeyClass, ValClass> * p = search(t, &index);
		if (!p)
		{
			if (root)
				clear();
			root = new Node<KeyClass, ValClass>(true, Capacity, 0);
			root->push(t, 0);
			root->push(v, 0);
			size = 1;
			return;
		}

		int pos = p->binSearch(t, false);
		p->push(t, pos);
		p->push(v, pos);
		++size;

		split(p);
	}

	template<class KeyClass, class ValClass>
	Node<KeyClass, ValClass>* BpTree<KeyClass, ValClass>::search(KeyClass & key, int *index)
	{
		if (!root)
			return root;
		Node<KeyClass, ValClass> *p = root;
		int pos = p->binSearch(key, true);
		while (p->leaf == false)
		{
			p = p->links[pos];
			pos = p->binSearch(key, true);
		}
		if (index)
			*index = pos;
		return p;
	}

	template<class KeyClass, class ValClass>
	inline ValClass * BpTree<KeyClass, ValClass>::search(KeyClass & key)
	{
		int index;
		auto nd = search(key, &index);
		if (!nd)
			return nullptr;
		else return &nd->vals[index];
	}

	template<class KeyClass, class ValClass>
	inline bool BpTree<KeyClass, ValClass>::remove(KeyClass & key)
	{
		int index;
		Node<KeyClass, ValClass> *p = search(key, &index);
		int pos = p->binSearch(key, true);
		if (p->keys[pos] == key)
			p->erase(pos, true);
		else if (pos!=0 && p->keys[pos - 1] == key)
			p->erase(pos, true);
		else return false;
		--size;
		merge(p);
		return true;
	}

	template<class KeyClass, class ValClass>
	inline void BpTree<KeyClass, ValClass>::clear()
	{
		delete root;
		root = nullptr;
		size = 0;
	}


	template<class KeyClass, class ValClass>
	inline void BpTreeHandler<KeyClass, ValClass>::getInput(BpTree<KeyClass, ValClass> *Tr)
	{
		bool existing = true;
		if (!Tr)
		{
			int cap;
			cout << "TREE CAPACITY >>>";
			cin >> cap;
			while (cap < 2) {
				cout << "CAPACITY MUST BE >= 2. >>>";
				cin >> cap;
			}
			Tr = new BpTree<KeyClass, ValClass>(cap);
			existing = false;
		}
		int choise;
		string message = "";
		do
		{
			system("cls");
			cout << message << endl;
			cout << "---TREE---" << endl;
			//Tr->print();
			cout << "SIZE IS " << Tr->getSize() << " NODES." << endl;
			cout << endl;
			cout << "1 - INSERT, 2 - REMOVE, 3 - SEARCH, 4 - CLEAR, 5 - WRITE IMAGE FILE (GRAPHVIZ), <ANYTHING ELSE> - EXIT" << endl;
			cout << ">>>";

			cin >> choise;
			KeyClass key;
			ValClass val;
			switch (choise)
			{
			case 1:
				cout << "Enter key >>>";
				cin >> key;
				cout << "Enter value >>>";
				cin >> val;
				Tr->insert(key, val);
				message = "Successfully inserted " + to_string(key) + ".";
				break;
			case 2:
			{
				cout << "Enter keys to remove >>>";
				cin >> key;
				bool success = Tr->remove(key);
				if (success)
					message = "Successfully removed " + to_string(key) + ".";
				else message = "Not found " + to_string(key) + ".";
				break;
			}
			case 3:
			{
				cout << "Enter keys to search for >>>";
				cin >> key;
				int val;
				int *index = &val;
				Node<KeyClass, ValClass> *found = Tr->search(key, index);
				if (!found || found->keys[val] != key)
					message = "Not found " + to_string(key) + "."; //Found instead " << to_string(found->data[*index]) << ".";
				else
					message = "Found " + to_string(key) + ".";
				break;
			}
			case 4:
				Tr->clear();
				message = "Tree is now empty.";
				break;
			case 5:
			{
				string filename;
				cout << "Enter file name to save image (without extenstion) >>>";
				cin >> filename;
				write_tree_to_image_file(Tr, filename);
				break;
			}
			default:
				break;
			}
		} while (choise >= 1 && choise <= 5);
		if (!existing)
		{
			delete Tr;
		}
	}

	template<class KeyClass, class ValClass>
	inline void BpTreeHandler<KeyClass, ValClass>::write_proc(Node<KeyClass, ValClass>* p, ofstream * file, int label, int capacity)
	{
		if (!p)
			return;
		int next = label*capacity;

		if (p->leaf)
		{
			//cluster label
			string l = " <";
			for (int i = 0; i < p->size - 1; ++i)
			{
				l += "A" + to_string(next + i) + "> |" + to_string(p->keys[i]) + "| <";
			}
			l += "A" + to_string(next + p->size - 1) + "> |" + to_string(p->keys[p->size - 1]) + "| <A" + to_string(label*capacity + p->size - 1) + "> size=" + to_string(p->size) + " ";

			q.push(label);

			//cluster
			(*file) << "node" + to_string(label) << " [label = \"" << l << "\"];" << endl;


			return;
		}

		//cluster label
		string l = " <";
		for (int i = 0; i < p->size - 2; ++i)
		{
			l += "A" + to_string(label*capacity + i) + "> |" + to_string(p->keys[i]) + "| <";
		}
		l += "A" + to_string(label*capacity + p->size - 2) + "> |" + to_string(p->keys[p->size - 2]) + "| <A" + to_string(label*capacity+p->size-1) + ">size = " + to_string(p->size) + " ";


		//cluster
		(*file) << "node" + to_string(label) << " [label = \"" << l << "\"];" << endl;
	

		//verges
		for (int i = 0; i < p->size; ++i)
		{
			(*file) << "\"node" << to_string(label) << "\":A" + to_string(label*capacity + i) << " -> \"node" << to_string(label*capacity + i) << "\"  [weight=999];" << endl; // "\":A" << to_string((label*capacity + i)*capacity) << ";" << endl;
			if (p->links[i] && p->links[i]->parent == p)
				(*file) << "\"node" << to_string(label) << "\":A" + to_string(label*capacity + i) << " -> \"node" << to_string(label*capacity + i) << "\"  [weight=999, dir=back];" << endl; // "\":A" << to_string((label*capacity + i)*capacity) << ";" << endl;
		}

		for (int i = 0; i < p->size; ++i)
		{
			write_proc(p->links[i], file, label * capacity + i, capacity);
		}


	}

	template<class KeyClass, class ValClass>
	inline void BpTreeHandler<KeyClass, ValClass>::writeLabels(ofstream *file)
	{
		if (q.empty())
			return;
		int c = q.front();
		q.pop();
		while (!q.empty())
		{
			int d = c;
			c = q.front();
			q.pop();
			(*file) << "\"node" << to_string(d) << "\" -> \"node" + to_string(c) << "\" [weight=0, constraint=false];" << endl;
		}

	}

	template<class KeyClass, class ValClass>
	inline void BpTreeHandler<KeyClass, ValClass>::write_tree_to_image_file(BpTree<KeyClass, ValClass> *Tr, string filename)
	{
		ofstream fout("tmp.gv", ios::out);
		fout << "digraph " + filename + " {" << endl;
		fout << "node [shape = record,height=.1];" << endl;
		write_proc(Tr->getRoot(), &fout, 1, Tr->getCapacity());
		writeLabels(&fout);
		fout << "}" << endl;
		fout.close();
		string fname = filename + ".png";
		string cd = get_cd();
		string command = "dot -Tpng " + cd + "/tmp.gv > " + cd + "/" + fname;
		SetCurrentDirectory((cd + "/bin").c_str());
		system(command.c_str());
		SetCurrentDirectory(cd.c_str());
		system(fname.c_str());
		system("del tmp.gv");
		_getch();
	}

	template<class KeyClass, class ValClass>
	inline string BpTreeHandler<KeyClass, ValClass>::get_cd()
	{
		wchar_t * c = nullptr;
		wstring w = _wgetcwd(c, _MAX_PATH);
		return string(w.begin(), w.end());
	}


