#pragma once
#include <vector>
#include "Itemset.h"
#include "rapidcsv.h"

class ItemsetGenerator
{
public:
	ItemsetGenerator(size_t level);
	//Generates the level 1 Itemsets from a document.
	void GenerateFirstItemsetsThreaded(const rapidcsv::Document& document);
	//Generates the (level) Itemsets from the (level - 1) generator.
	void GenerateItemsets(const ItemsetGenerator& previousGenerator);
	//Calculates the TIDs of the itemsets using multi-threading.
	void CalculateTIDsThreaded();
	//Removes the unfrequent itemsets.
	void PruneUnfrequentItemsets(float minSupport, size_t rowCount);
	//Calculates the closure of each itemset.
	void CalculateClosures();
	//Removes itemsets that are included in the closures of the previous generator.
	void PruneUsingClosures(const ItemsetGenerator& previousGenerator);
	const std::vector<Itemset>& GetItemsets() const;
	const std::vector<Itemset>& GetClosures() const;
	bool IsEmpty() const;
	size_t GetLevel() const;
	
private:
	std::vector<Itemset> itemsets;
	std::vector<Itemset> closures;
	size_t level;
};