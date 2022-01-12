#include "sequence.h"
#include <string>
#pragma once

//Dynamic array tests
//Assuming Get and GetSize work correctly
void TestDynamicArrayCreation(std::string prefix = "", bool silent = true);
void TestDynamicArraySet(std::string prefix = "", bool silent = true);
void TestDynamicArrayResize(std::string prefix = "", bool silent = true);
void TestDynamicArrayDelete(std::string prefix = "", bool silent = true);
void TestDynamicArrayOperators(std::string prefix = "", bool silent = true);
void TestDynamicArrayAll(std::string prefix = "", bool silent = true);

//Linked list tests
//Assuming Get and GetLength work correctly
void TestLinkedListCreation(std::string prefix = "", bool silent = true);
void TestLinkedListSet(std::string prefix = "", bool silent = true);
void TestLinkedListAdd(std::string prefix = "", bool silent = true);
void TestLinkedListDelete(std::string prefix = "", bool silent = true);
void TestLinkedListConcat(std::string prefix = "", bool silent = true);
void TestLinkedListSublist(std::string prefix = "", bool silent = true);
void TestLinkedListOperators(std::string prefix = "", bool silent = true);
void TestLinkedListAll(std::string prefix = "", bool silent = true);

//Sequence tests
//Assuming DynamicArray, LinkedList, Get and GetLength work correctly
void TestArraySequenceCreation(std::string prefix = "", bool silent = true);
void TestListSequenceCreation(std::string prefix = "", bool silent = true);
void TestSequenceSet(SequenceType seqType, std::string prefix = "", bool silent = true);
void TestSequenceAdd(SequenceType seqType, std::string prefix = "", bool silent = true);
void TestSequenceDelete(SequenceType seqType, std::string prefix = "", bool silent = true);
void TestSequenceConcat(SequenceType seqType, std::string prefix = "", bool silent = true);
void TestSequenceSubsequence(SequenceType seqType, std::string prefix = "", bool silent = true);
void TestSequenceOperators(SequenceType seqType, std::string prefix = "", bool silent = true);
void TestSequenceAll(std::string prefix = "", bool silent = true);
