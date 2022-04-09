#pragma once
#include <vector>
#include "Itemset.h"
#include "rapidcsv.h"

class ItemsetGenerator
{
public:
	ItemsetGenerator(size_t level);
	//Generates the level 1 Itemsets from a document.
	void GenerateFirstItemsets(const rapidcsv::Document& document);
	//Generates the (level) Itemsets from the (level - 1) generator.
	void GenerateItemsets(const ItemsetGenerator& previousGenerator);

	size_t GetLevel() const;
private:
	const std::vector<Itemset>& GetItemsets() const;
private:
	std::vector<Itemset> itemsets;
	std::vector<Itemset> closures;
	size_t level;
};