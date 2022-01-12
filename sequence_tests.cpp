#include <iostream>
#include <algorithm>
#include "sequence_tests.h"
#include "dynamic_array.h"
#include "linked_list.h"
#include "sequence.h"
#include "list_sequence.h"
#include "array_sequence.h"
#include <string>
#include <complex>
#include <cassert> 

//Example functions
bool CompareFloat(float a, float b) {
	return a > b;
}

bool IsEven(int x) {
	return (x % 2 == 0);
}

float Sum(float a, float b) {
	return a + b;
}

//Dynamic array tests
//Assuming Get and GetSize work correctly
void TestDynamicArrayCreation(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Creation: ";

	//Empty
	DynamicArray<float>* a_float = new DynamicArray<float>();
	DynamicArray<std::complex<float>>* a_complex = new DynamicArray<std::complex<float>>();
	assert(a_float->GetSize() == 0);
	assert(a_complex->GetSize() == 0);
	delete(a_float);
	delete(a_complex);

	//By size
	for (size_t size = 0; size < 5; size++) {
		a_float = new DynamicArray<float>(size);
		a_complex = new DynamicArray<std::complex<float>>(size);
		assert(a_float->GetSize() == size);
		assert(a_complex->GetSize() == size);
		delete(a_float);
		delete(a_complex);
	}

	//By items and count, copying
	for (size_t size = 0; size < 3; size++) {
		float items_float[3];
		std::complex<float> items_complex[3];
		for (size_t i = 0; i < 3; i++) {
			items_float[i] = i + 0.1f;
			items_complex[i] = std::complex<float>((float)i, (float)(size - i));
		}
		for (size_t count = 0; count < 5; count++) {
			
			a_float = new DynamicArray<float>(items_float, count);
			a_complex = new DynamicArray<std::complex<float>>(items_complex, count);
			DynamicArray<float>* a_float_copy = new DynamicArray<float>(*a_float);
			DynamicArray<std::complex<float>>* a_complex_copy 
				= new DynamicArray<std::complex<float>>(*a_complex);
			for (int i = 0; i < std::min(count, size); i++) {
				assert(a_float->Get(i) == items_float[i]);
				assert(a_complex->Get(i) == items_complex[i]);
				assert(a_float_copy->Get(i) == items_float[i]);
				assert(a_complex_copy->Get(i) == items_complex[i]);
			}
			assert(a_float->GetSize() == count);
			assert(a_complex->GetSize() == count);
			assert(a_float_copy->GetSize() == count);
			assert(a_complex_copy->GetSize() == count);
			delete(a_float);
			delete(a_complex);
			delete(a_float_copy);
			delete(a_complex_copy);
		}
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestDynamicArraySet(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Set: ";

	const size_t size = 3;
	float items_float[3];
	std::complex<float> items_complex[3];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	DynamicArray<float>* a_float = new DynamicArray<float>(size);
	DynamicArray<std::complex<float>>* a_complex =
		new DynamicArray<std::complex<float>>(size);
	for (int i = 0; i < size; i++) {
		a_float->Set(i, items_float[i]);
		a_complex->Set(i, items_complex[i]);
	}
	for (int i = 0; i < size; i++) {
		assert(a_float->Get(i) == items_float[i]);
		assert(a_complex->Get(i) == items_complex[i]);
	}
	delete(a_float);
	delete(a_complex);

	if (!silent)
		std::cout << "OK\n";
}

void TestDynamicArrayResize(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Resize: ";

	const size_t size = 3;
	float items_float[3];
	std::complex<float> items_complex[3];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	DynamicArray<float>* a_float = new DynamicArray<float>(items_float, size);
	DynamicArray<std::complex<float>>* a_complex =
		new DynamicArray<std::complex<float>>(items_complex, size);

	for (int i = (int)size + 3; i >= 0; i--) {
		a_float->Resize(i);
		a_complex->Resize(i);
		assert(a_float->GetSize() == i);
		assert(a_complex->GetSize() == i);
		for (size_t j = 0; j < std::min((size_t)i, size); j++) {
			assert(a_float->Get(j) == items_float[j]);
			assert(a_complex->Get(j) == items_complex[j]);
		}
	}
	delete(a_float);
	delete(a_complex);

	if (!silent)
		std::cout << "OK\n";
}

void TestDynamicArrayDelete(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Delete: ";

	size_t size = 6;
	float items_float[6];
	std::complex<float> items_complex[6];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	DynamicArray<float>* a_float = new DynamicArray<float>(items_float, size);
	DynamicArray<std::complex<float>>* a_complex =
		new DynamicArray<std::complex<float>>(items_complex, size);

	a_float->Delete(0);
	a_float->Delete(size - 2);
	a_float->Delete(1);
	a_float->Delete(size - 4);
	assert(a_float->Get(0) == items_float[1]);
	assert(a_float->Get(1) == items_float[size - 3]);
	assert(a_float->GetSize() == size - 4);

	a_complex->Delete(0);
	a_complex->Delete(size - 2);
	a_complex->Delete(1);
	a_complex->Delete(size - 4);
	assert(a_complex->Get(0) == items_complex[1]);
	assert(a_complex->Get(1) == items_complex[size - 3]);
	assert(a_complex->GetSize() == size - 4);
	
	for (int i = 0; i < size - 4; i++) {
		a_float->Delete(0);
		a_complex->Delete(0);
	}
	assert(a_float->GetSize() == 0);
	assert(a_complex->GetSize() == 0);

	delete(a_float);
	delete(a_complex);

	if (!silent)
		std::cout << "OK\n";
}

void TestDynamicArrayOperators(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Operators: ";

	const size_t size = 3;
	float items_float[3];
	std::complex<float> items_complex[3];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	DynamicArray<float>* a_float = new DynamicArray<float>(size);
	DynamicArray<std::complex<float>>* a_complex =
		new DynamicArray<std::complex<float>>(size);
	for (int i = 0; i < size; i++) {
		(*a_float)[i] = items_float[i];
		(*a_complex)[i] = items_complex[i];
	}
	for (int i = 0; i < size; i++) {
		assert((*a_float)[i] == items_float[i]);
		assert((*a_complex)[i] == items_complex[i]);
	}
	delete(a_float);
	delete(a_complex);

	if (!silent)
		std::cout << "OK\n";
}

void TestDynamicArrayAll(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Dynamic array tests:\n";
	prefix += "----";
	TestDynamicArrayCreation(prefix, silent);
	TestDynamicArraySet(prefix, silent);
	TestDynamicArrayResize(prefix, silent);
	TestDynamicArrayDelete(prefix, silent);
	TestDynamicArrayOperators(prefix, silent);
}

//Linked list tests
//Assuming Get and GetSize work correctly
void TestLinkedListCreation(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Creation: ";

	//Empty
	LinkedList<float>* l_float = new LinkedList<float>();
	LinkedList<std::complex<float>>* l_complex = new LinkedList<std::complex<float>>();
	assert(l_float->GetLength() == 0);
	assert(l_complex->GetLength() == 0);
	delete(l_float);
	delete(l_complex);

	//By items and count, copying
	for (size_t size = 0; size < 3; size++) {
		float items_float[3];
		std::complex<float> items_complex[3];
		for (size_t i = 0; i < 3; i++) {
			items_float[i] = i + 0.1f;
			items_complex[i] = std::complex<float>((float)i, (float)(size - i));
		}
		for (size_t count = 0; count < 5; count++) {

			l_float = new LinkedList<float>(items_float, count);
			l_complex = new LinkedList<std::complex<float>>(items_complex, count);
			LinkedList<float>* l_float_copy = new LinkedList<float>(*l_float);
			LinkedList<std::complex<float>>* l_complex_copy
				= new LinkedList<std::complex<float>>(*l_complex);
			for (int i = 0; i < std::min(count, size); i++) {
				assert(l_float->Get(i) == items_float[i]);
				assert(l_complex->Get(i) == items_complex[i]);
				assert(l_float_copy->Get(i) == items_float[i]);
				assert(l_complex_copy->Get(i) == items_complex[i]);
			}
			assert(l_float->GetLength() == count);
			assert(l_complex->GetLength() == count);
			assert(l_float_copy->GetLength() == count);
			assert(l_complex_copy->GetLength() == count);
			delete(l_float);
			delete(l_complex);
			delete(l_float_copy);
			delete(l_complex_copy);
		}
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestLinkedListSet(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Set: ";

	const size_t size = 3;
	float items_float[3];
	std::complex<float> items_complex[3];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	LinkedList<float>* l_float = new LinkedList<float>();
	LinkedList<std::complex<float>>* l_complex =
		new LinkedList<std::complex<float>>();
	for (int i = 0; i < size; i++) {
		l_float->Append(0.f);
		l_complex->Append(std::complex<float>(0.f, 0.f));
	}
	for (int i = 0; i < size; i++) {
		l_float->Set(i, items_float[i]);
		l_complex->Set(i, items_complex[i]);
	}
	for (int i = 0; i < size; i++) {
		assert(l_float->Get(i) == items_float[i]);
		assert(l_complex->Get(i) == items_complex[i]);
	}
	delete(l_float);
	delete(l_complex);

	if (!silent)
		std::cout << "OK\n";
}

void TestLinkedListAdd(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Additions: ";
	
	const size_t size = 5;
	float items_float[5];
	std::complex<float> items_complex[5];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	LinkedList<float>* l_float = new LinkedList<float>();
	LinkedList<std::complex<float>>* l_complex =
		new LinkedList<std::complex<float>>();
	
	l_float->Append(items_float[3]);
	l_float->Append(items_float[4]);
	l_float->Prepend(items_float[1]);
	l_float->Prepend(items_float[0]);
	l_float->InsertAt(items_float[2], 2);

	l_complex->Append(items_complex[3]);
	l_complex->Append(items_complex[4]);
	l_complex->Prepend(items_complex[1]);
	l_complex->Prepend(items_complex[0]);
	l_complex->InsertAt(items_complex[2], 2);

	assert(l_float->GetLength() == size);
	assert(l_complex->GetLength() == size);
	for (int i = 0; i < size; i++) {
		assert(l_float->Get(i) == items_float[i]);
		assert(l_complex->Get(i) == items_complex[i]);
	}

	delete(l_float);
	delete(l_complex);

	if (!silent)
		std::cout << "OK\n";
}

void TestLinkedListDelete(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Delete: ";

	size_t size = 6;
	float items_float[6];
	std::complex<float> items_complex[6];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	LinkedList<float>* l_float = new LinkedList<float>(items_float, size);
	LinkedList<std::complex<float>>* l_complex =
		new LinkedList<std::complex<float>>(items_complex, size);

	l_float->Delete(0);
	l_float->Delete(size - 2);
	l_float->Delete(1);
	l_float->Delete(size - 4);
	assert(l_float->Get(0) == items_float[1]);
	assert(l_float->Get(1) == items_float[size - 3]);
	assert(l_float->GetLength() == size - 4);

	l_complex->Delete(0);
	l_complex->Delete(size - 2);
	l_complex->Delete(1);
	l_complex->Delete(size - 4);
	assert(l_complex->Get(0) == items_complex[1]);
	assert(l_complex->Get(1) == items_complex[size - 3]);
	assert(l_complex->GetLength() == size - 4);

	for (int i = 0; i < size - 4; i++) {
		l_float->Delete(0);
		l_complex->Delete(0);
	}
	assert(l_float->GetLength() == 0);
	assert(l_complex->GetLength() == 0);

	delete(l_float);
	delete(l_complex);

	if (!silent)
		std::cout << "OK\n";
}

void TestLinkedListConcat(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Concat: ";

	const size_t size = 6;
	float items_float[6];
	std::complex<float> items_complex[6];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	LinkedList<float>* l_float_1 = new LinkedList<float>();
	LinkedList<std::complex<float>>* l_complex_1 =
		new LinkedList<std::complex<float>>();
	LinkedList<float>* l_float_2 = new LinkedList<float>();
	LinkedList<std::complex<float>>* l_complex_2 =
		new LinkedList<std::complex<float>>();
	LinkedList<float>* l_float_3;
	LinkedList<std::complex<float>>* l_complex_3;

	for (int i = 0; i < size / 2; i++) {
		l_float_1->Append(items_float[i]);
		l_complex_1->Append(items_complex[i]);
	}
	for (int i = size/2; i < size; i++) {
		l_float_2->Append(items_float[i]);
		l_complex_2->Append(items_complex[i]);
	}
	l_float_3 = l_float_1->Concat(l_float_2);
	l_complex_3 = l_complex_1->Concat(l_complex_2);

	assert(l_float_3->GetLength() == size);
	assert(l_complex_3->GetLength() == size);
	for (int i = 0; i < size; i++) {
		assert(l_float_3->Get(i) == items_float[i]);
		assert(l_complex_3->Get(i) == items_complex[i]);
	}

	delete(l_float_1);
	delete(l_float_2);
	delete(l_float_3);
	delete(l_complex_1);
	delete(l_complex_2);
	delete(l_complex_3);

	if (!silent)
		std::cout << "OK\n";
}

void TestLinkedListSublist(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "SubSublist: ";

	const size_t size = 6;
	float items_float[6];
	std::complex<float> items_complex[6];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	LinkedList<float>* l_float_1 = new LinkedList<float>(items_float, size);
	LinkedList<std::complex<float>>* l_complex_1 =
		new LinkedList<std::complex<float>>(items_complex, size);
	LinkedList<float>* l_float_2;
	LinkedList<std::complex<float>>* l_complex_2;

	l_float_2 = l_float_1->GetSubList(0, size / 2);
	l_complex_2 = l_complex_1->GetSubList(0, size / 2);
	size_t sub_size = size / 2 + 1;
	assert(l_float_2->GetLength() == sub_size);
	assert(l_complex_2->GetLength() == sub_size);
	for (int i = 0; i < sub_size; i++) {
		assert(l_float_2->Get(i) == items_float[i]);
		assert(l_complex_2->Get(i) == items_complex[i]);
	}
	delete(l_float_2);
	delete(l_complex_2);

	l_float_2 = l_float_1->GetSubList(size / 2, size - 1);
	l_complex_2 = l_complex_1->GetSubList(size / 2, size - 1);
	sub_size = size - size / 2;
	assert(l_float_2->GetLength() == sub_size);
	assert(l_complex_2->GetLength() == sub_size);
	for (int i = 0; i < sub_size; i++) {
		assert(l_float_2->Get(i) == items_float[size/2 + i]);
		assert(l_complex_2->Get(i) == items_complex[size/2 + i]);
	}
	delete(l_float_2);
	delete(l_complex_2);

	delete(l_float_1);
	delete(l_complex_1);
	

	if (!silent)
		std::cout << "OK\n";
}

void TestLinkedListOperators(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Operators: ";

	const size_t size = 3;
	float items_float[3];
	std::complex<float> items_complex[3];
	for (size_t i = 0; i < size; i++) {
		items_float[i] = i + 0.1f;
		items_complex[i] = std::complex<float>((float)i, (float)(size - i));
	}
	LinkedList<float>* l_float = new LinkedList<float>();
	LinkedList<std::complex<float>>* l_complex =
		new LinkedList<std::complex<float>>();
	for (int i = 0; i < size; i++) {
		l_float->Append(0.f);
		l_complex->Append(std::complex<float>(0.f, 0.f));
	}
	for (int i = 0; i < size; i++) {
		(*l_float)[i] = items_float[i];
		(*l_complex)[i] = items_complex[i];
	}
	for (int i = 0; i < size; i++) {
		assert((*l_float)[i] == items_float[i]);
		assert((*l_complex)[i] == items_complex[i]);
	}
	delete(l_float);
	delete(l_complex);

	if (!silent)
		std::cout << "OK\n";
}

void TestLinkedListAll(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Linked list tests:\n";
	prefix += "----";
	TestLinkedListCreation(prefix, silent);
	TestLinkedListSet(prefix, silent);
	TestLinkedListAdd(prefix, silent);
	TestLinkedListDelete(prefix, silent);
	TestLinkedListConcat(prefix, silent);
	TestLinkedListSublist(prefix, silent);
	TestLinkedListOperators(prefix, silent);
}

//Sequence tests
//Assuming DynamicArray, LinkedList, Get and GetLength work correctly
void TestArraySequenceCreation(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Creation (Array): ";

	//Empty
	Sequence<float>* sequence = new ArraySequence<float>();
	assert(sequence->GetLength() == 0);
	delete(sequence);

	//By items and count, copying
	for (size_t size = 0; size < 3; size++) {
		float* items = (float*)std::malloc(size * sizeof(float));
		if (!items)
			throw std::runtime_error("Cannot allocate memory");
		for (size_t i = 0; i < size; i++) {
			items[i] = i + 0.1f;
		}
		for (size_t count = 0; count < 5; count++) {

			sequence = new ArraySequence<float>(items, count);
			Sequence<float>* sequence_copy = new ArraySequence<float>(*sequence);
			for (int i = 0; i < std::min(count, size); i++) {
				assert(sequence->Get(i) == items[i]);
				assert(sequence_copy->Get(i) == items[i]);
			}
			assert(sequence->GetLength() == count);
			assert(sequence_copy->GetLength() == count);
			delete(sequence);
			delete(sequence_copy);
		}
		free(items);
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestListSequenceCreation(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Creation (List): ";

	//Empty
	Sequence<float>* sequence = new ListSequence<float>();
	assert(sequence->GetLength() == 0);
	delete(sequence);

	//By items and count, copying
	for (size_t size = 0; size < 3; size++) {
		float* items = (float*)std::malloc(size * sizeof(float));
		if (!items)
			throw std::runtime_error("Cannot allocate memory");
		for (size_t i = 0; i < size; i++) {
			items[i] = i + 0.1f;
		}
		for (size_t count = 0; count < 5; count++) {

			sequence = new ListSequence<float>(items, count);
			Sequence<float>* sequence_copy = new ListSequence<float>(*sequence);
			for (int i = 0; i < std::min(count, size); i++) {
				assert(sequence->Get(i) == items[i]);
				assert(sequence_copy->Get(i) == items[i]);
			}
			assert(sequence->GetLength() == count);
			assert(sequence_copy->GetLength() == count);
			delete(sequence);
			delete(sequence_copy);
		}
		free(items);
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestSequenceSet(SequenceType seqType, std::string prefix, bool silent)
{
	if (!silent) {
		std::cout << prefix << "Set (";
		if (seqType == SequenceType::List)
			std::cout << "List";
		else if (seqType == SequenceType::Array)
			std::cout << "Array";
		std::cout << "): ";
	}

	for (size_t size = 0; size < 5; size++)
	{
		float* items = (float*)std::malloc(size * sizeof(float));
		if (!items)
			throw std::runtime_error("Cannot allocate memory");
		for (size_t i = 0; i < size; i++)
			items[i] = i + 0.1f;
		Sequence<float>* sequence;
		if (seqType == SequenceType::List)
			sequence = new ListSequence<float>();
		else if (seqType == SequenceType::Array)
			sequence = new ArraySequence<float>();
		else
			throw std::invalid_argument("Got wrong sequence type.");

		for (int i = 0; i < size; i++) {
			sequence->Append(0.f);
		}
		for (int i = 0; i < size; i++) {
			sequence->Set(i, items[i]);
		}
		for (int i = 0; i < size; i++) {
			assert(sequence->Get(i) == items[i]);
		}

		free(items);
		delete(sequence);
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestSequenceAdd(SequenceType seqType, std::string prefix, bool silent)
{
	if (!silent) {
		std::cout << prefix << "Addition (";
		if (seqType == SequenceType::List)
			std::cout << "List";
		else if (seqType == SequenceType::Array)
			std::cout << "Array";
		std::cout << "): ";
	}

	const size_t size = 5;
	float* items = (float*)std::malloc(size * sizeof(float));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	for (size_t i = 0; i < size; i++)
		items[i] = i + 0.1f;
	Sequence<float>* sequence;
	if (seqType == SequenceType::List)
		sequence = new ListSequence<float>();
	else if (seqType == SequenceType::Array)
		sequence = new ArraySequence<float>();
	else
		throw std::invalid_argument("Got wrong sequence type.");

	sequence->Append(items[3]);
	sequence->Append(items[4]);
	sequence->Prepend(items[1]);
	sequence->Prepend(items[0]);
	sequence->InsertAt(items[2], 2);

	assert(sequence->GetLength() == size);
	for (int i = 0; i < size; i++)
		assert(sequence->Get(i) == items[i]);
	
	free(items);
	delete(sequence);

	if (!silent)
		std::cout << "OK\n";
}

void TestSequenceDelete(SequenceType seqType, std::string prefix, bool silent)
{
	if (!silent) {
		std::cout << prefix << "Delete (";
		if (seqType == SequenceType::List)
			std::cout << "List";
		if (seqType == SequenceType::Array)
			std::cout << "Array";
		std::cout << "): ";
	}

	const size_t size = 6;
	float* items = (float*)std::malloc(size * sizeof(float));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	for (size_t i = 0; i < size; i++)
		items[i] = i + 0.1f;
	Sequence<float>* sequence;
	if (seqType == SequenceType::List)
		sequence = new ListSequence<float>(items, size);
	else if (seqType == SequenceType::Array)
		sequence = new ArraySequence<float>(items, size);
	else
		throw std::invalid_argument("Got wrong sequence type.");

	sequence->Delete(0);
	sequence->Delete(size - 2);
	sequence->Delete(1);
	sequence->Delete(size - 4);
	assert(sequence->Get(0) == items[1]);
	assert(sequence->Get(1) == items[size - 3]);
	assert(sequence->GetLength() == size - 4);

	for (int i = 0; i < size - 4; i++)
		sequence->Delete(0);
	assert(sequence->GetLength() == 0);

	free(items);
	delete(sequence);

	if (!silent)
		std::cout << "OK\n";
}

void TestSequenceConcat(SequenceType seqType, std::string prefix, bool silent)
{
	if (!silent) {
		std::cout << prefix << "Concat (";
		if (seqType == SequenceType::List)
			std::cout << "List";
		if (seqType == SequenceType::Array)
			std::cout << "Array";
		std::cout << "): ";
	}

	for (size_t size = 0; size < 5; size++)
	{
		float* items = (float*)std::malloc(size * sizeof(float));
		if (!items)
			throw std::runtime_error("Cannot allocate memory");
		for (size_t i = 0; i < size; i++)
			items[i] = i + 0.1f;
		Sequence<float>* sequence_1;
		Sequence<float>* sequence_2;
		Sequence<float>* sequence_3;
		if (seqType == SequenceType::List) {
			sequence_1 = new ListSequence<float>();
			sequence_2 = new ListSequence<float>();
		}
		else if (seqType == SequenceType::Array) {
			sequence_1 = new ArraySequence<float>();
			sequence_2 = new ArraySequence<float>();
		}
		else
			throw std::invalid_argument("Got wrong sequence type.");

		for (int i = 0; i < size / 2; i++)
			sequence_1->Append(items[i]);
		for (int i = size / 2; i < size; i++)
			sequence_2->Append(items[i]);
		sequence_3 = sequence_1->Concat(sequence_2);

		assert(sequence_3->GetLength() == size);
		for (int i = 0; i < size; i++)
			assert(sequence_3->Get(i) == items[i]);

		free(items);
		delete(sequence_1);
		delete(sequence_2);
		delete(sequence_3);
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestSequenceSubsequence(SequenceType seqType, std::string prefix, bool silent)
{
	if (!silent) {
		std::cout << prefix << "Subsequence (";
		if (seqType == SequenceType::List)
			std::cout << "List";
		if (seqType == SequenceType::Array)
			std::cout << "Array";
		std::cout << "): ";
	}

	for (size_t size = 2; size < 5; size++)
	{
		float* items = (float*)std::malloc(size * sizeof(float));
		if (!items)
			throw std::runtime_error("Cannot allocate memory");
		for (size_t i = 0; i < size; i++)
			items[i] = i + 0.1f;
		Sequence<float>* sequence_1;
		Sequence<float>* sequence_2;
		if (seqType == SequenceType::List)
			sequence_1 = new ListSequence<float>(items, size);
		else if (seqType == SequenceType::Array)
			sequence_1 = new ArraySequence<float>(items, size);
		else
			throw std::invalid_argument("Got wrong sequence type.");

		sequence_2 = sequence_1->GetSubsequence(0, size / 2);
		size_t sub_size = size / 2 + 1;
		assert(sequence_2->GetLength() == sub_size);
		for (int i = 0; i < sub_size; i++)
			assert(sequence_2->Get(i) == items[i]);
		delete(sequence_2);

		sequence_2 = sequence_1->GetSubsequence(size / 2, size - 1);
		sub_size = size - size / 2;
		assert(sequence_2->GetLength() == sub_size);
		for (int i = 0; i < sub_size; i++)
			assert(sequence_2->Get(i) == items[size / 2 + i]);
		delete(sequence_2);

		free(items);
		delete(sequence_1);
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestSequenceOperators(SequenceType seqType, std::string prefix, bool silent)
{
	if (!silent) {
		std::cout << prefix << "Operators (";
		if (seqType == SequenceType::List)
			std::cout << "List";
		if (seqType == SequenceType::Array)
			std::cout << "Array";
		std::cout << "): ";
	}

	for (size_t size = 0; size < 5; size++)
	{
		float* items = (float*)std::malloc(size * sizeof(float));
		if (!items)
			throw std::runtime_error("Cannot allocate memory");
		for (size_t i = 0; i < size; i++)
			items[i] = i + 0.1f;
		Sequence<float>* sequence;
		if (seqType == SequenceType::List)
			sequence = new ListSequence<float>();
		else if (seqType == SequenceType::Array)
			sequence = new ArraySequence<float>();
		else
			throw std::invalid_argument("Got wrong sequence type.");

		for (int i = 0; i < size; i++)
			sequence->Append(0.f);
		for (int i = 0; i < size; i++)
			(*sequence)[i] = items[i];
		for (int i = 0; i < size; i++)
			assert((*sequence)[i] == items[i]);

		free(items);
		delete(sequence);
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestSequenceAll(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Sequence tests:\n";
	prefix += "----";

	TestListSequenceCreation(prefix, silent);
	TestSequenceSet(SequenceType::List, prefix, silent);
	TestSequenceAdd(SequenceType::List, prefix, silent);
	TestSequenceDelete(SequenceType::List, prefix, silent);
	TestSequenceConcat(SequenceType::List, prefix, silent);
	TestSequenceSubsequence(SequenceType::List, prefix, silent);
	TestSequenceOperators(SequenceType::List, prefix, silent);
	TestArraySequenceCreation(prefix, silent);
	TestSequenceSet(SequenceType::Array, prefix, silent);
	TestSequenceAdd(SequenceType::Array, prefix, silent);
	TestSequenceDelete(SequenceType::Array, prefix, silent);
	TestSequenceConcat(SequenceType::Array, prefix, silent);
	TestSequenceSubsequence(SequenceType::Array, prefix, silent);
	TestSequenceOperators(SequenceType::Array, prefix, silent);
	TestDynamicArrayAll(prefix, silent);
	TestLinkedListAll(prefix, silent);
}