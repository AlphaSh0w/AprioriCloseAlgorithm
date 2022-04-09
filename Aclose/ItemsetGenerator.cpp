#include "ItemsetGenerator.h"
#include <map>
#include <thread>
#include <assert.h>

ItemsetGenerator::ItemsetGenerator(size_t level)
	:
	level(level)
{
	assert(level != 0); //Level cannot be 0.
}

void ItemsetGenerator::GenerateFirstItemsets(const rapidcsv::Document& document)
{
	assert(level == 1);//This function should only be called for generating the first itemsets.
	std::vector<std::map<std::string, std::vector<size_t>>> tids;
	//Allocate the necessarry memory to hold the TIDs
	tids.resize(document.GetColumnCount());
	std::vector<std::thread> workers;
	//Go through the document and generate the TIDs.
	for (size_t iColumn = 0; iColumn < document.GetColumnCount(); ++iColumn)
	{
		const auto worker = [&tids](const rapidcsv::Document& document, const size_t iColumn) {
			//Generate the values for this column and fill their TIDs.
			auto column = document.GetColumn<std::string>(iColumn);
			for (size_t iRow = 0; iRow < column.size(); ++iRow)
			{
				tids[iColumn][column[iRow]].emplace_back(iRow);
			}
		};
		workers.push_back(std::thread{ worker, std::ref(document), iColumn });
	}
	for (auto& worker : workers)
	{
		worker.join();
	}
	//Figure out and reserve the required space to hold all itemsets.
	size_t requiredSpace = 0;
	for (const auto& column : tids)
	{
		requiredSpace += column.size();
	}
	itemsets.reserve(requiredSpace);
	//Fill the itemsets and give them their TIDs.
	for (size_t iColumn = 0; iColumn < document.GetColumnCount(); ++iColumn)
	{
		for (auto& valueTIDpair : tids[iColumn])
		{
			itemsets.emplace_back(std::vector{std::pair{iColumn,valueTIDpair.first}},std::move(valueTIDpair.second));
		}
	}
}

void ItemsetGenerator::GenerateItemsets(const ItemsetGenerator& previousGenerator)
{
	assert(level != 1); //This function should not be called for generating the first itemsets.
	assert(level == previousGenerator.GetLevel() + 1); //The previous generator should be exactly one (level) lower than this generator.
	//Generate current (level) itemsets by combining the (level - 1) itemsets.
	const std::vector<Itemset>& previousItemsets = previousGenerator.GetItemsets();
	for (size_t i = 0; i < previousItemsets.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < previousItemsets.size(); ++j)
		{
			//Check if they have (level - 2) first items in common.
			if (previousItemsets[i].HasSameFirstKItems(previousItemsets[j], level - 2))
			{
				//Get their union.
				Itemset itemsetUnion = previousItemsets[i] + previousItemsets[j];
				//Check if this combination of items is possible.
				if (itemsetUnion.IsValid())
				{
					itemsets.emplace_back(itemsetUnion);
				}
			}
		}
	}
}

size_t ItemsetGenerator::GetLevel() const
{
	return level;
}

const std::vector<Itemset>& ItemsetGenerator::GetItemsets() const
{
	return itemsets;
}
