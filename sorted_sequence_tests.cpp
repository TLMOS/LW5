#include "sorted_sequence_tests.h"

void TestSortedSequenceCreation(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Creation:";

	SortedSequence<float>* sequence = new SortedSequence<float>();
	assert(sequence->IsEmpty() == 0);
	assert(sequence->GetLength() == 0);
	delete(sequence);

	if (!silent)
		std::cout << "OK\n";
}

void TestSortedSequenceAdd(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Addition:";

	const size_t size = 5;
	float* items = (float*)std::malloc(size * sizeof(float));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	for (size_t i = 0; i < size; i++)
		items[i] = i + 0.1f;
	SortedSequence<float>* sequence = new SortedSequence<float>();

	sequence->Add(items[3]);
	sequence->Add(items[4]);
	sequence->Add(items[1]);
	sequence->Add(items[0]);
	sequence->Add(items[2]);

	assert(sequence->GetLength() == size);
	for (int i = 0; i < size; i++)
		assert(sequence->Get(i) == items[i]);

	free(items);
	delete(sequence);

	if (!silent)
		std::cout << "OK\n";
}

void TestSortedSequenceRemove(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Remove:";

	const size_t size = 6;

	SortedSequence<float>* sequence = new SortedSequence<float>();
	for (size_t i = 0; i < size; i++)
		sequence->Add(i);

	sequence->Remove(0);
	sequence->Remove(size - 2);
	sequence->Remove(1);
	sequence->Remove(size - 4);
	assert(sequence->GetLength() == size - 4);

	delete(sequence);

	if (!silent)
		std::cout << "OK\n";
}

void TestSortedSequenceContains(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Containts:";

	SortedSequence<float>* sequence = new SortedSequence<float>();
	for (int i = 0; i < 10; i++)
		sequence->Add(i);
	for (int i = 0; i < 20; i++)
		assert(sequence->Contains(i) == (i < 10));
	delete(sequence);

	if (!silent)
		std::cout << "OK\n";
}

void TestSortedSequenceSubsequence(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Subsequence:";

	for (size_t size = 2; size < 5; size++)
	{
		SortedSequence<float>* sequence_1 = new SortedSequence<float>();
		SortedSequence<float>* sequence_2 = new SortedSequence<float>();
		for (size_t i = 0; i < size; i++)
		{
			sequence_1->Add(i + 0.1f);
			sequence_2->Add(i + 0.1f);
		}

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

		delete(sequence_1);
	}

	if (!silent)
		std::cout << "OK\n";
}

void TestSortedSequenceIndexOf(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "IndexOf:";

	SortedSequence<float>* sequence = new SortedSequence<float>();
	for (int i = 0; i < 10; i++)
		sequence->Add(i);
	for (int i = 0; i < 10; i++)
		assert(sequence->IndexOf(i) == i);
	delete(sequence);

	if (!silent)
		std::cout << "OK\n";
}

void TestSortedSequenceIterator(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Iterator:";

	SortedSequence<float>* sequence = new SortedSequence<float>();
	for (int i = 0; i < 10; i++)
		sequence->Add(i);
	size_t count = 0;
	IIterator<float>* it = sequence->GetIterator();
	for (; it->HasNext(); it->Next())
	{
		assert(count == it->GetCurrentIndex());
		assert(sequence->Get(i) == it->GetCurrentItem());
		count++;
	}
	assert(count == sequence->GetLength());
	delete(it);
	delete(sequence);

	if (!silent)
		std::cout << "OK\n";
}

void TestSortedSequenceAll(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Sorted sequence tests:\n";
	prefix += "----";

	TestSortedSequenceCreation(prefix, silent);
	TestSortedSequenceAdd(prefix, silent);
	TestSortedSequenceRemove(prefix, silent);
	TestSortedSequenceContains(prefix, silent);
	TestSortedSequenceSubsequence(prefix, silent);
	TestSortedSequenceIndexOf(prefix, silent);
	TestSortedSequenceIterator(prefix, silent);
}
