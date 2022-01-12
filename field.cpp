#include "field.h"

Field::Field(int vision_range)
{
	cells_ = new Dictionary<int, Dictionary<int, CellType>*>();
	field_of_view_ = new ListSequence<Cell>();
	Cell cell(0, 0);
	field_of_view_->Append(cell);
	vision_range_ = vision_range;
}

Field::~Field()
{
	Sequence<int>* x_keys = cells_->GetKeys();
	IIterator<int>* it_x = x_keys->GetIterator();
	for (; it_x->HasNext(); it_x->Next())
		delete(cells_->Get(it_x->GetCurrentItem()));
	delete(it_x);
	delete(x_keys);
	delete(cells_);
	delete(field_of_view_);
}

CellType Field::Get(int x, int y)
{
	if (cells_->ContainsKey(x))
	{
		Dictionary<int, CellType>* row = cells_->Get(x);
		if (row->ContainsKey(y))
			return row->Get(y);
		else
			return CellType::None;
	}
	else
		return CellType::None;
}

void Field::Set(int x, int y, CellType type)
{
	if (!cells_->ContainsKey(x))
		cells_->Add(x, new Dictionary<int, CellType>);
	Dictionary<int, CellType>* row = cells_->Get(x);
	if (row->ContainsKey(y))
	{
		row->Set(y, type);
	}
	else
		row->Add(y, type);
	UpdateFieldOfView(x, y);
}

void Field::Delete(int x, int y)
{
	if (cells_->ContainsKey(x))
	{
		Dictionary<int, CellType>* row = cells_->Get(x);
		if (row->ContainsKey(y))
			row->Remove(y);
		else
			throw std::invalid_argument("Cell (x, y) is not defined.");
	}
	else
		throw std::invalid_argument("Cell (x, -) is not defined.");
}

Sequence<Cell>* Field::GetFieldOfView()
{
	return field_of_view_;
}

Sequence<Cell>* Field::GetFieldOfViewCopy()
{
	Sequence<Cell>* field_of_view = new ListSequence<Cell>();
	IIterator<Cell>* it = field_of_view_->GetIterator();
	for (; it->HasNext(); it->Next())
		field_of_view->Append(it->GetCurrentItem());
	delete(it);
	return field_of_view;
}

void Field::SetFieldOfView(Sequence<Cell>* field_of_view)
{
	delete(field_of_view_);
	field_of_view_ = field_of_view;
}

void Field::UpdateFieldOfView(int x, int y)
{
	if (Get(x, y) != CellType::None)
	{
		IIterator<Cell>* it = field_of_view_->GetIterator();
		for (; it->HasNext(); it->Next())
		{
			if (it->GetCurrentItem() == Cell(x, y))
			{
				field_of_view_->Delete(it->GetCurrentIndex());
				break;
			}
		}
		delete(it);
	}

	for (int xi = x - vision_range_; xi <= x + vision_range_; xi++)
	{
		for (int yi = y - vision_range_; yi <= y + vision_range_; yi++)
		{
			if (Get(xi, yi) == CellType::None)
			{
				Cell cell(xi, yi);
				if (!field_of_view_->Contains(cell))
					field_of_view_->Append(cell);
			}
		}
	}
}
