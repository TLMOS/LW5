#include "sequence.h"
#include "binary_tree.h"
#pragma once

template<class TElement>
class SortedSequence
{
private:
	BTree<TElement>* elements_;
public:
	SortedSequence();
	SortedSequence(Sequence<TElement>* elements);
	~SortedSequence();
	size_t GetLength();
	bool IsEmpty();
	TElement Get(size_t index);
	TElement GetFirst();
	TElement GetLast();
	bool Contains(TElement element);
	size_t IndexOf(TElement elements);
	SortedSequence<TElement>* GetSubsequence(size_t startIndex, size_t endIndex);
	void Add(TElement element);
	void Remove(TElement element);
	IIterator<TElement>* GetIterator();
};

template<class TElement>
inline SortedSequence<TElement>::SortedSequence()
{
	elements_ = new BTree<TElement>();
}

template<class TElement>
inline SortedSequence<TElement>::SortedSequence(Sequence<TElement>* elements)
{
	elements_ = new BTree<TElement>();
	for (int i = 0; i < elements->GetLength(); i++)
		elements_->Insert(elements->Get(i));
}

template<class TElement>
inline SortedSequence<TElement>::~SortedSequence()
{
	delete(elements_);
}

template<class TElement>
inline size_t SortedSequence<TElement>::GetLength()
{
	return elements_->GetSize();
}

template<class TElement>
inline bool SortedSequence<TElement>::IsEmpty()
{
	return (elements_->GetSize() == 0);
}

template<class TElement>
inline TElement SortedSequence<TElement>::Get(size_t index)
{
	return elements_->Get(index);
}

template<class TElement>
inline TElement SortedSequence<TElement>::GetFirst()
{
	return elements_->GetFirst();
}

template<class TElement>
inline TElement SortedSequence<TElement>::GetLast()
{
	return elements_->GetLast();
}

template<class TElement>
inline bool SortedSequence<TElement>::Contains(TElement element)
{
	return elements_->Contains(element);
}

template<class TElement>
inline size_t SortedSequence<TElement>::IndexOf(TElement element)
{
	size_t index = 0;
	for (IIterator<int>* it = elements_->GetIterator(); it->HasNext(); it->Next())
	{
		if (it->GetCurrentItem() == element)
		{
			delete(it);
			return index;
		}
		index++;
	}
	throw std::invalid_argument("Elemnt is not in sequence.");
}

template<class TElement>
inline SortedSequence<TElement>* SortedSequence<TElement>::GetSubsequence(size_t startIndex, size_t endIndex)
{
	SortedSequence<TElement>* seq = new SortedSequence();
	for (int i = startIndex; i < endIndex; i++)
		seq->Add(elements_->Get(i));
	return seq;
}

template<class TElement>
inline void SortedSequence<TElement>::Add(TElement element)
{
	elements_->Insert(element);
}

template<class TElement>
inline void SortedSequence<TElement>::Remove(TElement element)
{
	elements_->Remove(element);
}


template<class TElement>
inline IIterator<TElement>* SortedSequence<TElement>::GetIterator()
{
	return elements_->GetIterator();
}
