#pragma once

template<class TElement>
class IIterator
{
public:
	virtual TElement GetCurrentItem() = 0;
	virtual size_t GetCurrentIndex() = 0;
	virtual bool HasNext() = 0;
	virtual bool Next() = 0;
};