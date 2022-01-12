#include "dictionary.h"
#include "sorted_sequence.h"
#pragma once

enum class CellType
{
	None = 1,
	X,
	O
};

struct Cell
{
	int x;
	int y;

	Cell()
	{
		this->x = 0;
		this->y = 0;
	}

	Cell(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	friend bool operator == (Cell const& a, Cell const& b)
	{
		return (a.x == b.x) && (a.y == b.y);
	}
	friend bool operator != (Cell const& a, Cell const& b)
	{
		return (a.x == b.x) || (a.y == b.y);
	}
};

class Field
{
private:
	Dictionary<int, Dictionary<int, CellType>*>* cells_;
	Sequence<Cell>* field_of_view_;
	int vision_range_;
public:
	Field(int vision_range);
	~Field();
	CellType Get(int x, int y);
	void Set(int x, int y, CellType type);
	void Delete(int x, int y);

	Sequence<Cell>* GetFieldOfView();
	Sequence<Cell>* GetFieldOfViewCopy();
	void SetFieldOfView(Sequence<Cell>* field_of_view);
	void UpdateFieldOfView(int x, int y);
};