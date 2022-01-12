#include "dictionary_tests.h"

void TestDictionaryCreation(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Creation:";
	Sequence<int>* keys = new ListSequence<int>();
	Sequence<float>* values = new ListSequence<float>();
	for (int i = 0; i < 10; i++)
	{
		keys->Append(i);
		values->Append(i + 0.1f);
	}
	Dictionary<int, float>* dict = new Dictionary<int, float>(keys, values);
	assert(dict->GetCount() == keys->GetLength());

	for (int i = 0; i < keys->GetLength(); i++)
		assert(dict->Get(keys->Get(i)) == values->Get(i));

	delete(dict);
	delete(keys);
	delete(values);
	if (!silent)
		std::cout << "OK\n";
}

void TestDictionaryGet(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Getters:";
	Sequence<int>* keys = new ListSequence<int>();
	Sequence<float>* values = new ListSequence<float>();
	for (int i = 0; i < 10; i++)
	{
		keys->Append(i);
		values->Append(i + 0.1f);
	}
	Dictionary<int, float>* dict = new Dictionary<int, float>(keys, values);
	assert(dict->GetCount() == keys->GetLength());
	
	for (int i = 0; i < keys->GetLength(); i++)
		assert(dict->Get(keys->Get(i)) == values->Get(i));

	delete(dict);
	delete(keys);
	delete(values);
	if (!silent)
		std::cout << "OK\n";
}

void TestDictionaryAdd(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Addition:";

	const size_t size = 5;
	float* items = (float*)std::malloc(size * sizeof(float));
	if (!items)
		throw std::runtime_error("Cannot allocate memory");
	for (size_t i = 0; i < size; i++)
		items[i] = i + 0.1f;
	Dictionary<int, float>* dict = new Dictionary<int, float>();

	dict->Add(3, items[3]);
	dict->Add(4, items[4]);
	dict->Add(1, items[1]);
	dict->Add(0, items[0]);
	dict->Add(2, items[2]);

	assert(dict->GetLength() == size);
	for (int i = 0; i < size; i++)
		assert(dict->Get(i) == items[i]);

	free(items);
	delete(dict);

	if (!silent)
		std::cout << "OK\n";
}

void TestDictionarySet(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Set:";

	if (!silent)
		std::cout << "OK\n";
}

void TestDictionaryRemove(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Remove:";

	const size_t size = 6;
	Dictionary<int, float>* dict = new Dictionary<int, float>();
	for (size_t i = 0; i < size; i++)
		dict->Add(i, i + 0.1f);

	dict->Remove(0);
	dict->Remove(size - 2);
	dict->Remove(1);
	dict->Remove(size - 4);
	assert(dict->GetLength() == size - 4);

	Sequence<int>* keys = dict->GetKeys();
	for (int i = 0; i < size - 4; i++)
		dict->Remove(keys->Get(i));
	assert(dict->GetLength() == 0);
	
	delete(keys);
	delete(dict);

	if (!silent)
		std::cout << "OK\n";
}

void TestDictionaryContainsKey(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "ContaintsKey:";

	Dictionary<int, float>* dict = new Dictionary<int, float>();
	for (int i = 0; i < 10; i++)
		dict->Add(i, i + 0.1f);
	for (int i = 0; i < 20; i++)
		assert(dict->Contains(i) == (i < 10));
	delete(dict);

	if (!silent)
		std::cout << "OK\n";
}

void TestDictionaryAll(std::string prefix, bool silent)
{
	if (!silent)
		std::cout << prefix << "Dictionary tests:\n";
	prefix += "----";

	TestDictionaryCreation(prefix, silent);
	TestDictionaryGet(prefix, silent);
	TestDictionaryAdd(prefix, silent);
	TestDictionarySet(prefix, silent);
	TestDictionaryRemove(prefix, silent);
	TestDictionaryContainsKey(prefix, silent);
}
