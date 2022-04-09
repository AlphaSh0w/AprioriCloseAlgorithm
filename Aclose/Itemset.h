#pragma once
#include <vector>
#include <string>
#include "rapidcsv.h"
class Itemset
{
public:
	//Constructor to create an itemset from a list of items.
	Itemset(std::vector<std::pair<size_t, std::string>> items, std::vector<size_t> tid);

	//Constructor used to indicate which itemsets were merged when creating this itemset.
	Itemset(std::vector<std::pair<size_t, std::string>> items, const Itemset* first, const Itemset* second);

	//Calculates the TID from a document.
	void CalculateTID(rapidcsv::Document& document);
	//Calculates the TID directly from the TIDs of the merged itemsets.
	void CalculateMergedTID();
private:
	std::vector<std::pair<size_t, std::string>> items;
	std::vector<size_t> tid;

	//Used to keep track of which itemsets were merged to created this itemset.
	const Itemset* first = nullptr;
	const Itemset* second = nullptr;
};