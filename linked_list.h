#include <cstdlib>
#include <cstring>
#include <stdexcept>
#pragma once

template<class T>
class LinkedList
{
private:	
	struct Node
	{
		T item{};
		Node* previous{};
		Node* next{};
	};

	Node* first_;
	Node* last_;
	size_t length_;

	class Iterator;

public:
	~LinkedList();

	LinkedList();
	LinkedList(T* items, size_t count);
	LinkedList(const LinkedList<T>& list);

	T GetFirst() const;
	T GetLast() const;
	T Get(size_t index) const;
	LinkedList<T>* GetSubList(size_t start_index, size_t end_index) const;
	size_t GetLength() const;

	void Set(size_t index, T item);
	void Append(T item);
	void Prepend(T item);
	void InsertAt(T item, size_t index);
	void Delete(size_t index);
	LinkedList<T>* Concat(LinkedList<T>* list);

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	IIterator<T>* GetIterator();
};

template<class T>
LinkedList<T>::LinkedList() {
	first_ = NULL;
	last_ = NULL;
	length_ = 0;
}

template<class T>
LinkedList<T>::LinkedList(T* items, size_t count) : LinkedList()
{
	if (count < 0)
		throw std::invalid_argument("Received negative count");

	for(int i = 0; i < count; i++)
		Append(items[i]);
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) : LinkedList()
{
	Node* node = list.first_;
	for (int i = 0; i < list.length_; i++) {
		Append(node->item);
		node = node->next;
	}
}

template<class T>
LinkedList<T>::~LinkedList()
{
	struct Node* tmp;
	last_ = NULL;
	while (first_ != NULL)
	{
		tmp = first_->next;
		delete first_;
		first_ = tmp;
	}
}

template<class T>
T LinkedList<T>::GetFirst() const
{
	if (!length_)
		throw std::out_of_range("Index out of range");

	return first_->item;
}

template<class T>
T LinkedList<T>::GetLast() const
{
	if (!length_)
		throw std::out_of_range("Index out of range");

	return last_->item;
}

template<class T>
T LinkedList<T>::Get(size_t index) const
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	Node* node = first_;
	for (int i = 0; i < index; i++)
		node = node->next;
	return node->item;
}

template<class T>
LinkedList<T>* LinkedList<T>::GetSubList(size_t start_index, size_t end_index) const
{
	if (start_index < 0 || start_index >= length_ 
		|| end_index < 0 || end_index >= length_)
		throw std::out_of_range("Index out of range");
	if (start_index > end_index)
		throw std::invalid_argument("End index should be greater than or equal to start index");

	size_t length = end_index - start_index + 1;
	T* items = (T*)std::malloc(length * sizeof(T));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	Node* node = first_;
	for (int i = 0; i < start_index; i++)
		node = node->next;
	for (int i = 0; i < length; i++) {
		items[i] = (node->item);
		node = node->next;
	}
	LinkedList<T>* subList = new LinkedList(items, length);
	free(items);
	return subList;
}

template<class T>
size_t LinkedList<T>::GetLength() const
{
	return length_;
}

template<class T>
inline void LinkedList<T>::Set(size_t index, T item)
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	Node* node = first_;
	for (int i = 0; i < index; i++)
		node = node->next;
	node->item = item;
}

template<class T>
void LinkedList<T>::Append(T item)
{
	Node* node = new Node();
	node->item = item;
	node->previous = last_;
	node->next = NULL;
	if (length_)
		last_->next = node;
	else
		first_ = node;
	last_ = node;
	length_++;
}

template<class T>
void LinkedList<T>::Prepend(T item)
{
	Node* node = new Node();
	node->item = item;
	node->previous = NULL;
	node->next = first_;
	if (length_)
		first_->previous = node;
	else
		last_ = node;
	first_ = node;
	length_++;
}

template<class T>
void LinkedList<T>::InsertAt(T item, size_t index)
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	Node* node = first_;
	for (int i = 0; i < index; i++)
		node = node->next;
	Node* newNode = new Node();

	newNode->item = node->item;
	node->item = item;
	newNode->next = node->next;
	newNode->previous = node;
	node->next = newNode;
	length_++;
}

template<class T>
inline void LinkedList<T>::Delete(size_t index)
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	Node* node = first_;
	for (int i = 0; i < index; i++)
		node = node->next;

	if (node->previous)
		node->previous->next = node->next;
	else
		first_ = node->next;
	if (node->next)
		node->next->previous = node->previous;
	else
		last_ = node->previous;

	delete node;
	length_--;
}

template<class T>
LinkedList<T>* LinkedList<T>::Concat(LinkedList<T>* list)
{
	LinkedList<T>* concatedList = new LinkedList<T>(*this);
	Node* node = list->first_;
	for (int i = 0; i < list->length_; i++) {
		concatedList->Append(node->item);
		node = node->next;
	}
	return concatedList;
}

template<class T>
T& LinkedList<T>::operator[](size_t index)
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	Node* node = first_;
	for (int i = 0; i < index; i++)
		node = node->next;
	return node->item;
}

template<class T>
const T& LinkedList<T>::operator[](size_t index) const
{
	if (index < 0 || index >= length_)
		throw std::out_of_range("Index out of range");

	Node* node = first_;
	for (int i = 0; i < index; i++)
		node = node->next;
	return node->item;
}

template<class T>
inline IIterator<T>* LinkedList<T>::GetIterator()
{
	return (IIterator<T>*)(new Iterator(this));
}

template<class T>
class LinkedList<T>::Iterator : IIterator<T> {
	friend class LinkedList<T>;
private:
	LinkedList<T>* list_;
	size_t count_;
	LinkedList<T>::Node* node_;
public:
	Iterator(LinkedList<T>* list)
	{
		list_ = list;
		count_ = 0;
		node_ = list->first_;
	}
	virtual T GetCurrentItem()
	{
		return node_->item;
	}
	virtual size_t GetCurrentIndex()
	{
		return count_;
	}
	virtual bool HasNext()
	{
		return count_ < list_->GetLength();
	}
	bool Next()
	{
		if (node_)
			node_ = node_->next;
		count_++;
		return true;
	}
};