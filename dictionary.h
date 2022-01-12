#include "sequence.h"
#include "binary_tree.h"
#include "list_sequence.h"
#pragma once

template<class TKey, class TElement>
class Dictionary
{
private:
	struct Pair;
	BTree<Pair>* items_;
public:
	Dictionary();
	Dictionary(Sequence<TKey>* keys, Sequence<TElement>* values);
	~Dictionary();
	size_t GetCount();
	TElement Get(TKey key);
	void Set(TKey key, TElement element);
	bool ContainsKey(TKey key);
	void Add(TKey key, TElement element);
	void Remove(TKey key);
	Sequence<TKey>* GetKeys();
	Sequence<TElement>* GetValues();
};

template<class TKey, class TElement>
struct Dictionary<TKey, TElement>::Pair
{
	TKey key;
	TElement value;

	Pair(TKey key, TElement value)
	{
		this->key = key;
		this->value = value;
	}

	Pair(TKey key)
	{
		this->key = key;
	}

	Pair()
	{
	}

	friend bool operator == (Pair const& a, Pair const& b)
	{
		return a.key == b.key;
	}
	friend bool operator != (Pair const& a, Pair const& b)
	{
		return a.key != b.key;
	}
	friend bool operator >= (Pair const& a, Pair const& b)
	{
		return a.key >= b.key;
	}
	friend bool operator <= (Pair const& a, Pair const& b)
	{
		return a.key <= b.key;
	}
	friend bool operator > (Pair const& a, Pair const& b)
	{
		return a.key > b.key;
	}
	friend bool operator < (Pair const& a, Pair const& b)
	{
		return a.key < b.key;
	}
};

template<class TKey, class TElement>
inline Dictionary<TKey, TElement>::Dictionary()
{
	items_ = new BTree<Pair>();
}

template<class TKey, class TElement>
inline Dictionary<TKey, TElement>::Dictionary(Sequence<TKey>* keys, Sequence<TElement>* values)
{
	if (keys->GetLength() != values->GetLength())
		throw std::invalid_argument("Keys and values sequences must have equal sizes.");
	items_ = new BTree<Pair>();
	for (int i = 0; i < keys->GetLength(); i++)
	{
		Pair pair(keys->Get(i), values->Get(i));
		items_->Insert(pair);
	}
}

template<class TKey, class TElement>
inline Dictionary<TKey, TElement>::~Dictionary()
{
	delete(items_);
}

template<class TKey, class TElement>
inline size_t Dictionary<TKey, TElement>::GetCount()
{
	return items_->GetSize();
}

template<class TKey, class TElement>
inline TElement Dictionary<TKey, TElement>::Get(TKey key)
{
	Pair tmp(key);
	BNode<Pair>* node = items_->Find(tmp);
	if (!node)
		throw std::invalid_argument("Wrong key.");
	return node->key.value;
}

template<class TKey, class TElement>
inline void Dictionary<TKey, TElement>::Set(TKey key, TElement element)
{
	Pair tmp(key);
	BNode<Pair>* node = items_->Find(tmp);
	if(!node)
		throw std::invalid_argument("Wrong key.");
	node->key.value = element;
}

template<class TKey, class TElement>
inline bool Dictionary<TKey, TElement>::ContainsKey(TKey key)
{
	Pair tmp(key);
	BNode<Pair>* node = items_->Find(tmp);
	return bool(node);
}

template<class TKey, class TElement>
inline void Dictionary<TKey, TElement>::Add(TKey key, TElement element)
{
	Pair tmp(key, element);
	items_->Insert(tmp);
}

template<class TKey, class TElement>
inline void Dictionary<TKey, TElement>::Remove(TKey key)
{
	Pair tmp(key);
	items_->Remove(tmp);
}

template<class TKey, class TElement>
inline Sequence<TKey>* Dictionary<TKey, TElement>::GetKeys()
{
	Sequence<TKey>* keys = new ListSequence<TKey>();
	IIterator<Pair>* it = items_->GetIterator();
	for (; it->HasNext(); it->Next())
		keys->Append(it->GetCurrentItem().key);
	delete(it);
	return keys;
}

template<class TKey, class TElement>
inline Sequence<TElement>* Dictionary<TKey, TElement>::GetValues()
{
	Sequence<TElement>* values = new ListSequence<TElement>();
	IIterator<Pair>* it = items_->GetIterator();
	for (; it->HasNext(); it->Next())
		values->Append(it->GetCurrentItem().value);
	delete(it);
	return values;
}
