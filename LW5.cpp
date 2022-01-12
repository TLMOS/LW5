#include "sequence_tests.h"
#include "tree_tests.h"
#include "sorted_sequence_tests.h"
#include "dictionary_tests.h"
#include "tic_tac_toe.h"

int main()
{
	TestSequenceAll("", false);
	TestTreeAll("", false);
	TestSortedSequenceAll("", false);
	TestDictionaryAll("", false);

	TicTacToe* app = TicTacToe::GetInstance("config.json");
	app->Start();
}