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

//namespace BpTr
//{
	template<class KeyClass>
	struct Node
	{
		KeyClass *data;

		bool	leaf;

		Node	*parent;
		Node	**keys;
		int		size;

		int		keysSize;
		int		dataSize;

		//Binary search.
		//Returns position of the first occurance of k or place after it's predecessor.
		//Returned pos is connected to keys field.
		int								binSearch				(KeyClass &data, bool first);

		//Get first occurance of an element placed on position pos.
		int								catchFirstOcc			(int pos);

		//Get last occurance of an element placed on position pos.
		int								catchLastOcc			(int pos);

		//Update every data field (need if sons are leafs).
		void							updateKey				();

		//Insert element into data array.
		void							push					(KeyClass &elem, int pos);

		//Insert node into keys array.
		void							push					(Node<KeyClass> *key, int pos);

		//Erase element on position pos.
		void							erase					(int pos, bool delete_node);

		//Constructors
										Node					() : keys(nullptr), size(0), parent(nullptr), data(nullptr), leaf(true) {};

										Node					(	bool _leaf = false,
																	int Capacity = 0,
																	int _size = 0,
																	Node ** _keys = nullptr,
																	KeyClass *_data = nullptr,
																	Node *_parent = nullptr	) : keys(_keys), parent(_parent), leaf(_leaf), 
																								size(_size), data(_data)			
																								{
																									if (Capacity != 0)
																										dataSize = Capacity + 1;
																									data = new KeyClass[dataSize];
																									//for (int i = 0; i < dataSize; ++i) {
																									//	data[i] = nullptr;
																									//}
																									if (!leaf)
																									{
																										keysSize = Capacity + 1;
																										keys = new Node<KeyClass>*[keysSize];
																										for (int i = 0; i < keysSize; ++i)
																											keys[i] = nullptr;
																									}
																									else {
																										keysSize = 1;
																										keys = new Node<KeyClass>*();
																										*keys = nullptr;
																									}
										}

		//Destructor			
										~Node					()							{	

																									parent = nullptr;
																									delete[] data;
																									data = nullptr;
																									if (!leaf && keys)
																									{
																										for (int i = 0; i < size; ++i)
																										{
																											if (keys[i])
																											{
																												keys[i]->parent = nullptr;
																												delete keys[i];
																												keys[i] = nullptr;
																											}
																										}
																										delete[] keys;
																										keys = nullptr;
																									}
																									else if (leaf)
																									{
																										if (keys && *keys) {
																											*keys = nullptr;
																											delete keys;
																										}
																										keys = nullptr;
																									}
																									cout << "Deleted.";
																								};
	};


	template<class KeyClass>
	class BpTree
	{

	private:

		//Max node size.
		int								Capacity;

		//Tree root.
		Node<KeyClass>							*root;

		//Current number of nodes.
		int								size;

		//Delete node p without deleting nodes linked to p.
		void							cutNode						(Node<KeyClass> *p);

		//Merge nodes on positions pos and pos+1.
		Node<KeyClass>							*mergeWithNext				(Node<KeyClass> *&p, int pos);

		//After-deletion procedure.
		void							merge						(Node<KeyClass> *p);

		//After-insertion procedure.
		void							split						(Node<KeyClass> *p);

		//Split node p by 2.
		Node<KeyClass>							*makeSecondNode				(Node<KeyClass> *&p);
		
		//If root is full, add new root. Increases tree height.
		void							createNewRoot				();

		
///		int								print						(Node<T> *p, bool nextline);

	public:

		//Access to Capacity field.
		int								getCapacity					()	const;

		//Access to root field.
		Node<KeyClass>							*getRoot					()	const;

		//Access to size field.
		int								getSize						()	const;

		//Insert element x.
		void							insert						(KeyClass &data);

		//Search for element x.
		//If x is found returns node which contains x
		//If x is not found returns nullptr.
		//If index != nullptr it receives a position of x in returned node.
		Node<KeyClass>							*search						(KeyClass &data, int *index = nullptr);

		//Remove element x.
		//Returns true if x is removed.
		//Returns false if x is not found.
		bool							remove						(KeyClass &data);

		//Free all allocated memory.
		void							clear						();

		//Constructors
										BpTree						(int _capacity) :
																						root(nullptr), 
																						size(0) 			{ 
																														if (_capacity < 2)
																															Capacity = 2;
																														else Capacity = _capacity;
																													};

		//Destructor
										~BpTree						()												{ clear(); };

	};


	template<class KeyClass>
	class BpTreeHandler
	{

	private:

		queue<int> q;

		void write_proc(Node<KeyClass> *p, ofstream *file, int label, int capacity);
		void writeLabels(ofstream *file);


	public:

		//This function tests all BpTree main functions.
		//Gets user's input from console and visualizes changes in a user-friendly way.
		//Parameter Tr is used for working with an existing tree to save result.
		//If Tr is not specified, tree is created automatically and gets deleted.
		void							getInput(BpTree<KeyClass> *Tr = nullptr);



		//This function performs tree visualization. It writes it into a DOT file, and then pushes the file to GraphViz app.
		//Format is PNG.
		void write_tree_to_image_file(BpTree<KeyClass> *Tr, string filename);

		string get_cd();
	};

	template<class KeyClass>
	inline void Node<KeyClass>::updateKey()
	{
		if (leaf && parent)
		{
			parent->updateKey();
			return;
		}
		if (size == 2)
			if (keys[0]->size == 0)
				data[0] = keys[1]->data[0];
			else
				data[0] = keys[0]->data[0];
		else
			for (int i = 0; i < size - 1; ++i)
				data[i] = keys[i+1] ? keys[i + 1]->data[0] : data[i-1];

		if (parent)
			parent->updateKey();
	}

	template<class KeyClass>
	int Node<KeyClass>::binSearch(KeyClass &k, bool first)
	{
		int sz = leaf ? size : size-1;
		int r = sz;
		int l = 0;

		if (k < data[0])
			return 0;

		if (k > data[r - 1])
			return sz;

		while (r - l >= 1)
		{
			int m = (r + l) / 2;
			if (data[m] > k)
				r = m;
			else if (data[m] == k)
			{
				return first ? catchFirstOcc(m) : catchLastOcc(m);
			}
			else if (data[m] < k && (m + 1 >= sz-1 || k < data[m + 1]))
			{
				return m+1;
			}
			else
				l = m + 1;
		}
		return l;
	}

	template<class KeyClass>
	inline int Node<KeyClass>::catchFirstOcc(int pos)
	{
		if (pos >= size || pos <= 1)
			return pos;
		while (data[pos-1] == data[pos - 2])
			--pos;
		return pos;
	}

	template<class KeyClass>
	inline int Node<KeyClass>::catchLastOcc(int pos)
	{
		if (pos >= size-1 || pos <= 0)
			return pos;
		while (data[pos-1] == data[pos])
			++pos;
		return pos;
	}

	template<class KeyClass>
	inline void Node<KeyClass>::push(KeyClass & elem, int pos)
	{
		//if (!leaf)
		//	return;
	//	int sz = leaf ? size : size - 1;
		if (leaf)
		{
			for (int i = size; i > pos; --i)
				data[i] = data[i - 1];
			++size;
		} else
			for (int i = size-1; i > pos; --i)
				data[i] = data[i - 1];
		data[pos] = elem;
 //?
//		updateKey();
	}

	template<class KeyClass>
	inline void Node<KeyClass>::push(Node<KeyClass>* key, int pos)
	{
		if (leaf)
			return;
		for (int i = size; i > pos; --i)
			keys[i] = keys[i - 1];
		keys[pos] = key;
		key->parent = this;
		++size;
		updateKey();
	}

	template<class KeyClass>
	inline void Node<KeyClass>::erase(int pos, bool delete_node)
	{
		if (!leaf)
		{
			if (delete_node)
			{
				keys[pos]->parent = nullptr;
				delete keys[pos];
				keys[pos] = nullptr;
			}
			for (int i = pos+1; i < size; ++i)
				keys[i - 1] = keys[i];
			for (int i=size; i<keysSize; ++i)
				keys[i] = nullptr;
			if (pos != size - 1)
				for (int i = pos+1; i <size-1; ++i)
					data[i - 1] = data[i];
	//		keys[size-1] = nullptr;
		}
		else {
			for (int i = pos+1; i < size; ++i)
				data[i - 1] = data[i];
		}
		--size;
		updateKey();
	}

	template<class KeyClass>
	inline void BpTree<KeyClass>::cutNode(Node<KeyClass>* p)
	{
		if (!p->leaf)
		{
			for (int i = 0; i < Capacity; ++i)
				p->keys[i] = nullptr;

			delete[] p->keys;
			p->keys = nullptr;
		}
		else p->keys = nullptr;
		p->parent = nullptr;
		delete p;
		int k = 0;
	}
	
	template<class KeyClass>
	inline Node<KeyClass> *BpTree<KeyClass>::mergeWithNext(Node<KeyClass>* &p, int pos)
	{
		if (p->leaf)
			return nullptr;
		Node<KeyClass> *m = p->keys[pos];
		Node<KeyClass> *n = p->keys[pos + 1];

		if (!m->leaf)
		{
			m->push(n->data[0], m->size - 1);
			if (n->size >= 2)
				for (int i = 1; i < n->size - 2; ++i)
					m->push(n->data[i], m->size + i);
			for (int i = 0; i < n->size; ++i)
				m->push(n->keys[i], m->size);
		}
		else
		{
			for (int i = 0; i < n->size; ++i)
				m->push(n->data[i], m->size);
			m->keys = n->keys;
			*n->keys = nullptr;
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

	template<class KeyClass>
	inline void BpTree<KeyClass>::merge(Node<KeyClass>* p)
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
			int pos = p->parent->binSearch(p->data[0], true);
			//if (pos == 0)
			//	break;
			while (pos < p->parent->size && p->parent->keys[pos] != p)
				++pos;
			if (pos >= p->parent->size)
				break;

			Node<KeyClass> *l = pos-1<0 ? nullptr : p->parent->keys[pos - 1];
			Node<KeyClass> *r = pos+1 == p->parent->size ? nullptr : p->parent->keys[pos + 1];
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
					p->push(l->keys[l->size - 1], 0);
					p->push(l->data[l->size - 2], 0);
				}
				else p->push(l->data[l->size - 1], 0);
				l->erase(l->size - 1, false);
				p = p->parent;
			}
			else if (r && r->size > 1) {
				if (!r->leaf)
				{
					p->push(r->keys[0], p->size);
					p->push(r->data[0], p->size - 1);
				}
				else
					p->push(r->data[0], p->size);		
				r->erase(0, false);
				p = p->parent;
			}
		}
	}
	
	template<class KeyClass>
	inline void BpTree<KeyClass>::split(Node<KeyClass>* p)
	{
		
		if (!p->leaf && p->size - 1 < Capacity)
			return;
		if (p->size < Capacity)
			return;
		while (p->parent && ((p->leaf && p->size >= Capacity) || (!p->leaf && p->size > Capacity)))
		{
//			delete p;
			Node<KeyClass> *k = makeSecondNode(p);
			if (!k->leaf)
			{
				int pos = p->parent->binSearch(k->data[0], false);
				if (k->data[0] < p->parent->data[pos]) {
					p->parent->push(k, pos+1);
					p->parent->push(k->data[0], pos);
				}
				else {
					p->parent->push(k, pos + 1);
					p->parent->push(k->data[0], pos + 1);
				}
				k->erase(0, false);
			}
			else {
				int pos = p->parent->binSearch(k->data[0], false);
				if (k->data[0] < p->parent->data[pos]) {
					p->parent->push(k->data[0], pos);
					p->parent->push(k, pos+1);
				}
				else {
					p->parent->push(k->data[0], pos + 1);
					//				p->parent->push(p, pos);
					p->parent->push(k, pos + 1);
				}
			}
			p = p->parent;
//			print();
		}

		createNewRoot();
	}


	
	template<class KeyClass>
	inline Node<KeyClass>* BpTree<KeyClass>::makeSecondNode(Node<KeyClass>* &p)
	{
		Node<KeyClass> *k = nullptr;
		if (p->leaf)
		{
			k = new Node<KeyClass>(true, Capacity, ((p->size % 2 == 0) ? p->size / 2 : p->size / 2 + 1)); //-> p -> k ->
			for (int i = 0; i < k->size; ++i)
				k->data[i] = p->data[p->size-k->size+i];
			p->size -= k->size;
			Node<KeyClass> ** tmp = k->keys;
			k->keys = p->keys;
			p->keys = tmp; 
			*p->keys = k;
		}
		else  {
			k = new Node<KeyClass>(false, Capacity, (p->size % 2 == 1) ? p->size / 2 : p->size/2+1 );
			for (int i = 0; i < k->size; ++i)
			{
				k->keys[i] = p->keys[p->size - k->size+i];
				p->keys[p->size - k->size + i] = nullptr;
				if (i!=0 && k->keys[i])
					k->keys[i]->parent = k;
			}
			//for (int i = k->size-1; i < p->size; ++i)
			//	p->keys[i] = nullptr;
			for (int i = 0; i < k->size - 1; ++i)
				k->data[i] = p->data[p->size-k->size+i];
			p->size -= k->size-1;
		}
		k->parent = p->parent;
		return k;
	}

	template<class KeyClass>
	inline void BpTree<KeyClass>::createNewRoot()
	{
		if (!root->leaf && root->size-1 < Capacity)
			return;
		if (root->size < Capacity)
			return;
		//delete root;


		Node<KeyClass> *k = makeSecondNode(root);
//		delete k;
		
		
		Node<KeyClass> * new_root = new Node<KeyClass>(false, Capacity, 2);
		new_root->keys[0] = root;
		new_root->keys[1] = k;
		if (!root->leaf)
		{
			new_root->data[0] = k->data[0];
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

	template<class KeyClass>
	inline int BpTree<KeyClass>::getCapacity() const
	{
		return Capacity;
	}

	template<class KeyClass>
	inline Node<KeyClass>* BpTree<KeyClass>::getRoot() const
	{
		return root;
	}

	template<class KeyClass>
	inline int BpTree<KeyClass>::getSize() const
	{
		return size;
	}

	template<class KeyClass>
	inline void BpTree<KeyClass>::insert(KeyClass & t)
	{
		Node<KeyClass> * p = search(t);
		if (!p)
		{
			if (root)
				clear();
			root = new Node<KeyClass>(true, Capacity, 0);
			root->push(t, 0);
			size = 1;
			return;
		}

		int pos = p->binSearch(t, false);
		p->push(t, pos);
		++size;

	split(p);
	}

	template<class KeyClass>
	Node<KeyClass>* BpTree<KeyClass>::search(KeyClass & data, int *index /* = nullptr */)
	{
		if (!root)
			return root;
		Node<KeyClass> *p = root;
		int pos = p->binSearch(data, true);
		while (p->leaf == false)
		{
			p = p->keys[pos];
			pos = p->binSearch(data, true);
		}
		if (index)
			*index = pos;
		return p;
	}

	template<class KeyClass>
	inline bool BpTree<KeyClass>::remove(KeyClass & data)
	{
		Node<KeyClass> *p = search(data);
		int pos = p->binSearch(data, true);
		if (p->data[pos] == data)
			p->erase(pos, true);
		else if (pos!=0 && p->data[pos - 1] == data)
			p->erase(pos, true);
		else return 0;
		--size;
		merge(p);
		return 1;
	}

	template<class KeyClass>
	inline void BpTree<KeyClass>::clear()
	{
		delete root;
		root = nullptr;
		size = 0;
	}

	//template<class T>
	//inline void BpTree<T>::print()
	//{
	//	if (!root)
	//		return;
	//	print(root, 1);
	//	cout << endl;
	//	cout << "(" << to_string(root->key) << ")" << endl;
	//}

	//template<class T>
	//inline int BpTree<T>::print(Node<T>* p, bool nextline)
	//{
	//	if (!p->leaf)
	//	{
	//		int total_sum = 0;
	//		int *intervals = new int[p->size]; //remake as C array
	//		for (int i =0; i < p->size-1; ++i)
	//		{
	//			int sum = print(p->keys[i], 0);
	//			total_sum += sum;
	//			intervals[i] = sum;
	//		}
	//		int sum = print(p->keys[p->size-1], nextline);
	//		total_sum += sum;
	//		intervals[p->size-1] = sum;

	//		for (int i = 0; i < p->size; ++i)
	//		{
	//			string s = to_string(p->keys[i]->key);
	//			cout << "(" << s << ")";
	//			for (int j = 0; j < intervals[i] - 2 - s.length(); ++j)
	//				cout << " ";
	//		}
	//		if (nextline)
	//			cout << endl;
	//		delete[] intervals;
	//		return total_sum;
	//	}
	//	else {
	//		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//		SetConsoleTextAttribute(hConsole, 11);
	//		int sum = 0;
	//		for (size_t i = 0; i < p->size; ++i)
	//		{
	//			string s = to_string(p->data[i]);
	//			sum += s.length() + 3;
	//			cout << "[" << s << "]" << " ";
	//		}
	//		cout << " ";
	//		if (nextline)
	//			cout << endl;
	//		++sum;
	//		SetConsoleTextAttribute(hConsole, 7);
	//		return sum;
	//	}
	//}

	template<class KeyClass>
	inline void BpTreeHandler<KeyClass>::getInput(BpTree<KeyClass> *Tr)
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
			Tr = new BpTree<KeyClass>(cap);
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
			KeyClass data;
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
			{
				cout << "Enter data to search for >>>";
				cin >> data;
				int val;
				int *index = &val;
				Node<KeyClass> *found = Tr->search(data, index);
				if (!found || found->data[val] != data)
					message = "Not found " + to_string(data) + "."; //Found instead " << to_string(found->data[*index]) << ".";
				else
					message = "Found " + to_string(data) + ".";
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

	template<class KeyClass>
	inline void BpTreeHandler<KeyClass>::write_proc(Node<KeyClass>* p, ofstream * file, int label, int capacity)
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
				l += "A" + to_string(next + i) + "> |" + to_string(p->data[i]) + "| <";
			}
			l += "A" + to_string(next + p->size - 1) + "> |" + to_string(p->data[p->size - 1]) + "| <A" + to_string(label*capacity + p->size - 1) + "> size=" + to_string(p->size) + " ";

			q.push(label);

			//cluster
			(*file) << "node" + to_string(label) << " [label = \"" << l << "\"];" << endl;


			return;
		}

		//cluster label
		string l = " <";
		for (int i = 0; i < p->size - 2; ++i)
		{
			l += "A" + to_string(label*capacity + i) + "> |" + to_string(p->data[i]) + "| <";
		}
		l += "A" + to_string(label*capacity + p->size - 2) + "> |" + to_string(p->data[p->size - 2]) + "| <A" + to_string(label*capacity+p->size-1) + ">size = " + to_string(p->size) + " ";


		//cluster
		(*file) << "node" + to_string(label) << " [label = \"" << l << "\"];" << endl;
	

		//verges
		for (int i = 0; i < p->size; ++i)
		{
			(*file) << "\"node" << to_string(label) << "\":A" + to_string(label*capacity + i) << " -> \"node" << to_string(label*capacity + i) << "\"  [weight=999];" << endl; // "\":A" << to_string((label*capacity + i)*capacity) << ";" << endl;
			if (p->keys[i] && p->keys[i]->parent == p)
				(*file) << "\"node" << to_string(label) << "\":A" + to_string(label*capacity + i) << " -> \"node" << to_string(label*capacity + i) << "\"  [weight=999, dir=back];" << endl; // "\":A" << to_string((label*capacity + i)*capacity) << ";" << endl;
		}

		for (int i = 0; i < p->size; ++i)
		{
			write_proc(p->keys[i], file, label * capacity + i, capacity);
		}


	}

	template<class KeyClass>
	inline void BpTreeHandler<KeyClass>::writeLabels(ofstream *file)
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

	//template<class T>
	//inline bool BpTreeHandler<T>::convert_dot_to_png(string dot_filename, string png_filename)
	//{
		//string o_arg = png_filename;

		//char* args[] = { "dot", "Tpng",
		//	const_cast<char*>(dot_filename.c_str()),  //DOT_TEXT_FILE is the file path in which the graph is saved as valid DOT syntax
		//	"-o",
		//	const_cast<char*>(o_arg.c_str()) 
		//};

		//const int argc = sizeof(args) / sizeof(args[0]);
		//Agraph_t *g=nullptr, *prev = nullptr;
		//GVC_t *gvc = gvContext();
		//gvParseArgs(gvc, argc, args);
		//
		////while ((g = gvNextInputGraph(gvc)))
		////{
		////	if (prev)
		////	{
		////		gvFreeLayout(gvc, prev);
		////		agclose(prev);
		////	}
		////	gvLayoutJobs(gvc, g);
		////	gvRenderJobs(gvc, g);
		////	prev = g;
		////}

		//return !gvFreeContext(gvc);
	//}

	template<class KeyClass>
	inline void BpTreeHandler<KeyClass>::write_tree_to_image_file(BpTree<KeyClass> *Tr, string filename)
	{
		ofstream fout("tmp.gv", ios::out);
		fout << "digraph " + filename + " {" << endl;
		fout << "node [shape = record,height=.1];" << endl;
		write_proc(Tr->getRoot(), &fout, 1, Tr->getCapacity());
		writeLabels(&fout);
		fout << "}" << endl;
		fout.close();
		string fname = filename + ".png";
//		convert_dot_to_png("C:/Users/morga/source/repos/2_course_2_sem/algo_labs/B+_Tree/B+_Tree/tmp.gv", fname);
		string cd = get_cd();
		string command = "dot -Tpng " + cd + "/tmp.gv > " + cd + "/" + fname;
		SetCurrentDirectory((cd + "/bin").c_str());
		system(command.c_str());
		SetCurrentDirectory(cd.c_str());
		system(fname.c_str());
		system("del tmp.gv");
		_getch();
	}

	template<class KeyClass>
	inline string BpTreeHandler<KeyClass>::get_cd()
	{
		wchar_t * c = nullptr;
		wstring w = _wgetcwd(c, _MAX_PATH);
		return string(w.begin(), w.end());
	}


