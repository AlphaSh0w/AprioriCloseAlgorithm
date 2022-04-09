#pragma once
#include <vector>
#include "Itemset.h"
class ItemsetGenerator
{
public:
	ItemsetGenerator(size_t level);
private:
	std::vector<Itemset> itemsets;
	std::vector<Itemset> closures;
	size_t level;
};