#pragma once
#include <vector>
#include "Itemset.h"
#include "rapidcsv.h"

class ItemsetGenerator
{
public:
	ItemsetGenerator(size_t level);
	void GenerateItemsets(const rapidcsv::Document& document);
private:
	void GenerateFirstItemsets(const rapidcsv::Document& document);
private:
	std::vector<Itemset> itemsets;
	std::vector<Itemset> closures;
	size_t level;
};