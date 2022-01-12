#include <cstdlib>
#include <string>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <algorithm>
#include "collection_interface.h"
#include "iterator_interface.h"
#pragma once

template<class T>
struct BNode
{
	T key;
	BNode<T>* left;
	BNode<T>* right;
	size_t height;
	bool left_is_thread;
	bool right_is_thread;

	BNode(T k)
	{
		key = k;
		left = NULL;
		right = NULL;
		height = 1;
		left_is_thread = false;
		right_is_thread = false;
	}
};

template<class T>
class BTree : public ICollection<T>
{
private:
	BNode<T>* root_;
	size_t size_;
	bool is_threaded_;

	void Destructor(BNode<T>* node);

	static size_t GetHeight(BNode<T>* node);
	static void UpdateHeight(BNode<T>* node);

	static int BalanceFactor(BNode<T>* node);
	static BNode<T>* RotateRight(BNode<T>* node);
	static BNode<T>* RotateLeft(BNode<T>* node);
	static BNode<T>* Balance(BNode<T>* node);

	static BNode<T>* Insert(BNode<T>* node, T k);
	static BNode<T>* Insert(BNode<T>* node_a, BNode<T>* node_b);

	static BNode<T>* FindMin(BNode<T>* node);
	static BNode<T>* RemoveMin(BNode<T>* node);
	static BNode<T>* Remove(BNode<T>* node, T k);

	static BNode<T>* Find(BNode<T>* node, T k);
	static bool Equals(BNode<T>* node_a, BNode<T>* node_b);

	static BNode<T>* Copy(BNode<T>* node);
	static size_t GetSize(BNode<T>* node);

	static void Thread(BNode<T>* node, BNode<T>* left_thread, BNode<T>* right_thread);
	void Thread(); 

	static BNode<T>* Compute(std::string* src, std::string sep[4], char type[3]);
	static std::string ToString(BNode<T>* node, std::string format);

	class Iterator;

public:
	~BTree();
	BTree();
	BTree(T* keys, size_t count);
	BTree(const BTree<T>& tree);
	BTree(std::string src, std::string format);

	BNode<T>* GetRoot();
	BNode<T>* Find(T k);
	size_t GetHeight();

	void Insert(T k);
	void Remove(T k);
	bool Contains(T k);
	bool Equals(BTree<T>* tree);
	bool Contains(BTree<T>* sub_tree);

	BTree<T>* GetSubTree(T k);

	BTree<T>* Map(T(*f)(T));
	BTree<T>* Where(bool (*f)(T));
	T Reduce(T(*f)(T, T), T c);

	std::string ToString(std::string format);

	//ICollection
	virtual T GetFirst() const;
	virtual T GetLast() const;
	virtual T Get(size_t index);
	virtual size_t GetSize() const;
	virtual const T& operator[](size_t index);

	//Iterator
	IIterator<T>* GetIterator();
};

template<class T>
inline BTree<T>::~BTree()
{
	if(root_)
		Destructor(root_);
}

template<class T>
BTree<T>::BTree() {
	root_ = NULL;
	size_ = 0;
	is_threaded_ = true;
}

template<class T>
inline BTree<T>::BTree(T* keys, size_t count)
{
	root_ = NULL;
	for (int i = 0; i < count; i++)
		Insert(keys[i]);
	is_threaded_ = true;
	Thread();
}

template<class T>
inline BTree<T>::BTree(const BTree<T>& tree)
{
	if (tree.root_)
		root_ = Copy(tree.root_);
	else
		root_ = NULL;
	size_ = tree.GetSize();
	is_threaded_ = true;
	Thread();
}

template<class T>
inline T TFromString(std::string);

inline int TFromString(std::string s) {
	return std::stoi(s);
}

template<class T>
inline BNode<T>* BTree<T>::Compute (std::string* src, std::string sep[4], char type[3])
{
	BNode<T>* node = new BNode<T>(T());
	node->left_is_thread = true;
	node->right_is_thread = true;

	for (int i = 0; i < 3; i++)
	{
		BNode<T>* leaf = NULL;
		*src = src->substr(sep[i].length(), src->length() - sep[i].length());
		if (src->find(sep[i]) == std::string::npos ||
			src->find(sep[i + 1]) < src->find(sep[i]))
		{
			std::string item = src->substr(0, src->find(sep[i + 1]));
			if (type[i] == 'N')
				node->key = TFromString(item);
			else
			{
				if (item == "")
					leaf = NULL;
				else
				{
					leaf = new BNode<T>(TFromString(item));
					leaf->left_is_thread = true;
					leaf->right_is_thread = true;
				}
			}
		}
		else
			leaf = Compute(src, sep, type);
		if (leaf)
		{
			if (type[i] == 'L')
			{
				node->left = leaf;
				node->left_is_thread = false;
			}
			if (type[i] == 'R')
			{
				node->right = leaf;
				node->right_is_thread = false;
			}
		}
		size_t closing_index = src->find(sep[i + 1]);
		*src = src->substr(closing_index, src->length() - closing_index);
	}
	UpdateHeight(node);
	return node;
}

template<class T>
inline BTree<T>::BTree(std::string src, std::string format)
{
	size_t index[3];
	index[0] = format.find("N");
	index[1] = format.find("L");
	index[2] = format.find("R");

	if (index[0] == std::string::npos ||
		index[1] == std::string::npos ||
		index[2] == std::string::npos)
		throw std::invalid_argument("Wrong format.");

	char type[3] = { 'N', 'L', 'R' };

	if (index[0] > index[2]) {
		std::swap(index[0], index[2]);
		std::swap(type[0], type[2]);
	}
	if (index[0] > index[1]) {
		std::swap(index[0], index[1]);
		std::swap(type[0], type[1]);
	}
	if (index[1] > index[2]) {
		std::swap(index[1], index[2]);
		std::swap(type[1], type[2]);
	}

	std::string sep[4];
	sep[0] = format.substr(0, index[0]);
	sep[1] = format.substr(index[0] + 1, (index[1] - index[0] - 1));
	sep[2] = format.substr(index[1] + 1, (index[2] - index[1] - 1));
	sep[3] = format.substr(index[2] + 1, (format.length() - index[1] - 1));

	root_ = Compute(&src, sep, type);
	size_ = GetSize(root_);
	is_threaded_ = true;
	Thread();
}

template<class T>
inline BNode<T>* BTree<T>::GetRoot()
{
	return root_;
}

template<class T>
inline size_t BTree<T>::GetHeight()
{
	return GetHeight(root_);
}

template<class T>
inline void BTree<T>::Insert(T k)
{
	root_ = Insert(root_, k);
	size_++;
	is_threaded_ = false;
}

template<class T>
inline void BTree<T>::Remove(T k)
{
	if (!Contains(k))
		throw std::invalid_argument("k not in tree.");

	root_ = Remove(root_, k);
	size_--;
	is_threaded_ = false;
}

template<class T>
inline bool BTree<T>::Contains(T k)
{
	return (Find(root_, k) != NULL);
}

template<class T>
inline bool BTree<T>::Equals(BTree<T>* tree)
{
	return Equals(root_, tree->root_);
}

template<class T>
inline bool BTree<T>::Contains(BTree<T>* sub_tree)
{
	if(!is_threaded_)
		Thread();
	BNode<T>* node = root_;
	while (node) {
		while (!node->left_is_thread)
			node = node->left;
		if (Equals(node, sub_tree->root_))
			return true;
		while (node->right_is_thread && node->right)
		{
			node = node->right;
			if (Equals(node, sub_tree->root_))
				return true;
		}
		if (!node->right)
			break;
		node = node->right;
	}
	return false;
}

template<class T>
inline BTree<T>* BTree<T>::GetSubTree(T k)
{
	BNode<T>* node = Find(k);
	if(!node)
		throw std::invalid_argument("k not in tree.");
	BTree<T>* tree = new BTree<T>();
	tree->root_ = Copy(node);
	tree->size_ = GetSize(node);
	tree->Thread();
	return tree;
}

template<class T>
inline BTree<T>* BTree<T>::Map(T(*f)(T))
{
	if (!is_threaded_)
		Thread();
	BTree<T>* tree = new BTree<T>();
	BNode<T>* node = root_;
	while (node) {
		while (!node->left_is_thread)
			node = node->left;
		tree->Insert(f(node->key));
		while (node->right_is_thread && node->right)
		{
			node = node->right;
			tree->Insert(f(node->key));
		}
		if (!node->right)
			break;
		node = node->right;
	}
	return tree;
}

template<class T>
inline BTree<T>* BTree<T>::Where(bool(*f)(T))
{
	if (!is_threaded_)
		Thread();
	BTree<T>* tree = new BTree<T>();
	BNode<T>* node = root_;
	while (node) {
		while (!node->left_is_thread)
			node = node->left;
		if (f(node->key))
			tree->Insert(node->key);
		while (node->right_is_thread && node->right)
		{
			node = node->right;
			if (f(node->key))
				tree->Insert(node->key);
		}
		if (!node->right)
			break;
		node = node->right;
	}
	return tree;
}

template<class T>
inline T BTree<T>::Reduce(T(*f)(T, T), T c)
{
	if (!is_threaded_)
		Thread();
	BNode<T>* node = root_;
	while (node) {
		while (!node->left_is_thread)
			node = node->left;
		c = f(node->key, c);
		while (node->right_is_thread && node->right)
		{
			node = node->right;
			c = f(node->key, c);
		}
		if (!node->right)
			break;
		node = node->right;
	}
	return c;
}

//N, L, R
template<class T>
inline std::string BTree<T>::ToString(std::string format)
{
	return ToString(root_, format);
}

// ICollection
template<class T>
inline T BTree<T>::GetFirst() const
{
	if (!root_)
		throw std::out_of_range("Tree is empty");

	BNode<T>* node = root_;
	while (!node->left_is_thread)
		node = node->left;
	return node->key;
}

template<class T>
inline T BTree<T>::GetLast() const
{
	if (!root_)
		throw std::out_of_range("Tree is empty");
	
	BNode<T>* node = root_;
	while (!node->right_is_thread)
		node = node->right;
	return node->key;
}

template<class T>
inline T BTree<T>::Get(size_t index)
{
	if (index < 0 || index >= size_)
		throw std::out_of_range("Index out of range");

	if (!is_threaded_)
		Thread();
	size_t count = 0;
	BNode<T>* node = root_;
	while (true) {
		while (!node->left_is_thread)
			node = node->left;
		if (count == index)
			return node->key;
		count++;

		while (node->right_is_thread && node->right)
		{
			node = node->right;
			if (count == index)
				return node->key;
			count++;
		}
		node = node->right;
	}
}

template<class T>
inline size_t BTree<T>::GetSize() const
{
	return size_;
}

template<class T>
inline const T& BTree<T>::operator[](size_t index)
{
	if (index < 0 || index >= size_)
		throw std::out_of_range("Index out of range");

	if (!is_threaded_)
		Thread();
	size_t count = 0;
	BNode<T>* node = root_;
	while (true) {
		while (!node->left_is_thread)
			node = node->left;
		if (count == index)
			return node->key;
		count++;

		while (node->right_is_thread && node->right)
		{
			node = node->right;
			if (count == index)
				return node->key;
			count++;
		}
		node = node->right;
	}	
}

//Private methods
template<class T>
inline void BTree<T>::UpdateHeight(BNode<T>* node)
{
	size_t right_height = node->right_is_thread ? 0 : GetHeight(node->right);
	size_t left_height = node->left_is_thread ? 0 : GetHeight(node->left);
	if (right_height >= left_height)
		node->height = right_height + 1;
	else
		node->height = left_height + 1;
}

template<class T>
inline int BTree<T>::BalanceFactor(BNode<T>* node)
{
	size_t right_height = node->right_is_thread ? 0 : GetHeight(node->right);
	size_t left_height = node->left_is_thread ? 0 : GetHeight(node->left);
	return right_height - left_height;
}

template<class T>
inline void BTree<T>::Destructor(BNode<T>* node)
{
	if (!node->left_is_thread)
		Destructor(node->left);
	if (!node->right_is_thread)
		Destructor(node->right);
	delete node;
}

template<class T>
inline size_t BTree<T>::GetHeight(BNode<T>* node)
{
	if (node == NULL)
		return 0;
	else
		return node->height;
}

template<class T>
inline BNode<T>* BTree<T>::RotateRight(BNode<T>* node_a)
{

	BNode<T>* node_b = node_a->left;
	if (node_b->right_is_thread)
	{
		node_a->left_is_thread = true;
		node_b->right_is_thread = false;
		node_a->left = NULL;
	}
	else
		node_a->left = node_b->right;
	node_b->right = node_a;
	UpdateHeight(node_a);
	UpdateHeight(node_b);
	return node_b;
}

template<class T>
inline BNode<T>* BTree<T>::RotateLeft(BNode<T>* node_a)
{
	assert(!node_a->right_is_thread);
	BNode<T>* node_b = node_a->right;
	if (node_b->left_is_thread)
	{
		node_a->right_is_thread = true;
		node_b->left_is_thread = false;
		node_a->right = NULL;
	}
	else
		node_a->right = node_b->left;
	node_b->left = node_a;
	UpdateHeight(node_a);
	UpdateHeight(node_b);
	return node_b;
}

template<class T>
inline BNode<T>* BTree<T>::Balance(BNode<T>* node)
{
	UpdateHeight(node);
	if (BalanceFactor(node) == 2)
	{
		assert(!node->right_is_thread);
		if (BalanceFactor(node->right) < 0)
			node->right = RotateRight(node->right);
		node = RotateLeft(node);
	}
	else if (BalanceFactor(node) == -2)
	{
		assert(!node->left_is_thread);
		if (BalanceFactor(node->left) > 0)
			node->left = RotateLeft(node->left);
		node = RotateRight(node);
	}
	return node;
}

template<class T>
inline BNode<T>* BTree<T>::Insert(BNode<T>* node, T k)
{
	if (node == NULL)
	{
		BNode<T>* new_node = new BNode<T>(k);
		new_node->left_is_thread = true;
		new_node->right_is_thread = true;
		return new_node;
	}
	if (k >= node->key)
	{
		if (node->right_is_thread)
		{
			node->right_is_thread = false;
			node->right = Insert(NULL, k);
		}
		else
			node->right = Insert(node->right, k);
	}
	else
	{
		if (node->left_is_thread)
		{
			node->left_is_thread = false;
			node->left = Insert(NULL, k);
		}
		else
			node->left = Insert(node->left, k);
	}
	return Balance(node);
}

template<class T>
inline BNode<T>* BTree<T>::Insert(BNode<T>* to, BNode<T>* from)
{
	if (from == NULL)
		return to;
	to = Insert(to, from->key);
	if (!from->left_is_thread)
		to = Insert(to, from->left);
	if (!from->right_is_thread)
		to = Insert(to, from->right);
	return to;
}

template<class T>
BNode<T>* BTree<T>::FindMin(BNode<T>* node)
{
	if (node->left == NULL || node->left_is_thread)
		return node;
	else
		return FindMin(node->left);
}

template<class T>
BNode<T>* BTree<T>::RemoveMin(BNode<T>* node)
{
	if (node->left == NULL || node->left_is_thread)
		return node->right_is_thread ? NULL: node->right;
	node->left = RemoveMin(node->left);
	if (!node->left)
		node->left_is_thread = true;
	return Balance(node);
}

template<class T>
inline BNode<T>* BTree<T>::Remove(BNode<T>* node, T k)
{
	if (node == NULL)
		return NULL;
	else if (k > node->key)
	{
		node->right = Remove(node->right_is_thread ? NULL : node->right, k);
		node->right_is_thread = bool(!node->right);
	}
	else if (k < node->key)
	{
		node->left = Remove(node->left_is_thread ? NULL : node->left, k);
		node->left_is_thread = bool(!node->left);
	}
	else
	{
		BNode<T>* node_l = node->left_is_thread ? NULL : node->left;
		BNode<T>* node_r = node->right_is_thread ? NULL : node->right;
		delete node;
		if (node_r == NULL)
			return node_l;
		else
		{
			BNode<T>* node_min;
			node_min = FindMin(node_r);
			node_min->right = RemoveMin(node_r);
			node_min->left = node_l;
			node_min->right_is_thread = bool(!node_min->right);
			node_min->left_is_thread = bool(!node_min->left);
			return Balance(node_min);
		}
	}
	return Balance(node);
}

template<class T>
inline BNode<T>* BTree<T>::Find(BNode<T>* node, T k)
{
	if (node == NULL)
		return NULL;
	else if (k > node->key)
		return Find(node->right_is_thread ? NULL : node->right, k);
	else if (k < node->key)
		return Find(node->left_is_thread ? NULL : node->left, k);
	else
		return node;
}

template<class T>
inline BNode<T>* BTree<T>::Find(T k)
{
	return Find(root_, k);
}

template<class T>
inline bool BTree<T>::Equals(BNode<T>* node_a, BNode<T>* node_b)
{
	if (!node_a || !node_b)
		throw std::invalid_argument("Got NULL pointer argument.");

	if (node_a->key == node_b->key)
	{
		if (node_a->left_is_thread == node_b->left_is_thread &&
			node_a->right_is_thread == node_b->right_is_thread)
		{
			if (!node_a->left_is_thread)
				if (!Equals(node_a->left, node_b->left))
					return false;
			if (!node_a->right_is_thread)
				if (!Equals(node_a->right, node_b->right))
					return false;
		}
		else
			return false;
	}
	else
		return false;

	return true;
}

template<class T>
inline BNode<T>* BTree<T>::Copy(BNode<T>* node)
{
	BNode<T>* copy = new BNode<T>(node->key);
	copy->height = node->height;
	copy->left_is_thread = node->left_is_thread;
	copy->right_is_thread = node->right_is_thread;
	
	if (copy->left_is_thread)
		copy->left = NULL;
	else
		copy->left = Copy(node->left);

	if (copy->right_is_thread)
		copy->right = NULL;
	else
		copy->right = Copy(node->right);

	return copy;
}

template<class T>
inline size_t BTree<T>::GetSize(BNode<T>* node)
{
	size_t size = 1;
	size += node->left_is_thread ? 0 : GetSize(node->left);
	size += node->right_is_thread ? 0 : GetSize(node->right);
	return size;
}

template<class T>
inline void BTree<T>::Thread(BNode<T>* node, BNode<T>* left_thread, BNode<T>* right_thread)
{
	if (node->left_is_thread)
		node->left = left_thread;
	else
		Thread(node->left, left_thread, node);

	if (node->right_is_thread)
		node->right = right_thread;
	else
		Thread(node->right, node, right_thread);
}

template<class T>
inline void BTree<T>::Thread()
{
	if (root_)
		Thread(root_, NULL, NULL);
}

template<class T>
inline std::string BTree<T>::ToString(BNode<T>* node, std::string format)
{
	std::string str_n;
	std::string str_l;
	std::string str_r;
	str_n = std::to_string(node->key);
	if(!node->left_is_thread)
		str_l = ToString(node->left, format);
	if (!node->right_is_thread)
		str_r = ToString(node->right, format);

	std::string str = format;
	size_t n_index = str.find("N");
	str.replace(n_index, 1, str_n);
	size_t l_index = str.find("L");
	str.replace(l_index, 1, str_l);
	size_t r_index = str.find("R");
	str.replace(r_index, 1, str_r);

	return str;
}

//Iterator
template<class T>
inline IIterator<T>* BTree<T>::GetIterator()
{
	if (!is_threaded_)
		Thread();
	return (IIterator<T>*)(new Iterator(this));
}

template<class T>
class BTree<T>::Iterator : IIterator<T> {
	friend class BTree<T>;
private:
	BTree<T>* tree_;
	size_t count_;
	BNode<T>* node_;
public:
	Iterator(BTree<T>* tree)
	{
		tree_ = tree;
		count_ = 0;
		node_ = tree->root_;

		if(node_)
			while (!node_->left_is_thread)
				node_ = node_->left;
	}
	virtual T GetCurrentItem()
	{
		return node_->key;
	}
	virtual size_t GetCurrentIndex()
	{
		return count_;
	}
	virtual bool HasNext()
	{
		return count_ < tree_->GetSize();
	}
	bool Next()
	{
		if (node_->right_is_thread && node_->right)
		{
			node_ = node_->right;
		}
		else
		{
			node_ = node_->right;
			if (node_)
				while (!node_->left_is_thread)
					node_ = node_->left;
		}
		count_++;
		return true;
	}
};