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

void ItemsetGenerator::GenerateFirstItemsetsThreaded(const rapidcsv::Document& document)
{
	assert(level == 1);//This function should only be called for generating the first itemsets.
	std::vector<std::map<std::string, std::vector<size_t>>> tids;
	//Allocate the necessarry memory to hold the TIDs
	tids.resize(document.GetColumnCount());
	std::vector<std::thread> workers;
	std::cout << "Generating the TIDs from the document, using " << document.GetColumnCount() << " threads (1 thread for each column)...\n";
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
	std::cout << "Generated " << itemsets.size() << " 1-Itemsets and their TIDs.\n";
}

void ItemsetGenerator::GenerateItemsets(const ItemsetGenerator& previousGenerator)
{
	assert(level != 1); //This function should not be called for generating the first itemsets.
	assert(level == previousGenerator.GetLevel() + 1); //The previous generator should be exactly one (level) lower than this generator.
	assert(previousGenerator.GetItemsets().size() > 0); //The previous itemsets should not be empty.
	//Generate current (level) itemsets by combining the (level - 1) itemsets.
	const std::vector<Itemset>& previousItemsets = previousGenerator.GetItemsets();
	for (size_t i = 0; i < previousItemsets.size() - 1; ++i)
	{
		for (size_t j = i + 1; j < previousItemsets.size(); ++j)
		{
			//Check if they have (level - 2) first items in common.
			if (previousItemsets[i].HasSameFirstKItems(previousItemsets[j], level - 2))
			{
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

void ItemsetGenerator::CalculateTIDsThreaded()
{
	const size_t minItemsPerThread = 100;
	const size_t hardwareThreadCount = std::thread::hardware_concurrency();
	const size_t threadCount = std::min(itemsets.size() / minItemsPerThread, hardwareThreadCount * 2);
	size_t step = 0;
	if (threadCount != 0)
	{
		std::cout << "Calculating TIDs using "<< threadCount << " thread(s)...\n";
		//Divide the itemsets between all threads evenly.
		step = itemsets.size() / threadCount;
		std::vector<std::thread> workers;
		auto worker = [this](size_t start, size_t end) {
			for (size_t i = start; i < end; ++i)
			{
				itemsets[i].CalculateTID();
			}
		};
		for (size_t i = 0; i < threadCount; ++i)
		{
			workers.push_back(std::thread{ worker, step * i, step * (i + 1)});
		}
		for (auto& worker : workers)
		{
			worker.join();
		}
	}
	else
	{
		std::cout << "Calculating TIDs using main thread...\n";
	}
	//Finish the leftover itemsets on the main thread
	for (size_t i = step * threadCount; i < itemsets.size(); ++i)
	{
		itemsets[i].CalculateTID();
	}
}

void ItemsetGenerator::PruneUnfrequentItemsets(float minSupport, size_t rowCount)
{
	auto toErase = std::remove_if(itemsets.begin(), itemsets.end(),
		[minSupport,rowCount](const Itemset& itemset){
			return itemset.GetSupport(rowCount) < minSupport;
		});
	itemsets.erase(toErase, itemsets.end());
}

void ItemsetGenerator::CalculateClosures()
{
	//Reserve the space for closures.
	closures.reserve(itemsets.size());
	for (size_t i = 0; i < itemsets.size(); ++i)
	{
		//The closure of an itemset contains at least itself.
		Itemset closure{itemsets[i].GetItems()};
		//If another itemset has the same TID, add its items to the closure.
		for (size_t j = 0; j < itemsets.size(); ++j)
		{
			if(i == j)
				continue;
			if (itemsets[i].IsTIDIncluded(itemsets[j]))
			{
				closure += itemsets[j];
			}
		}
		closures.emplace_back(std::move(closure));
	}
}

void ItemsetGenerator::CalculateClosuresThreaded()
{
	const size_t minItemsPerThread = 100;
	const size_t hardwareThreadCount = std::thread::hardware_concurrency();
	const size_t threadCount = std::min(itemsets.size() / minItemsPerThread, hardwareThreadCount * 2);
	size_t step = 0;
	if (threadCount != 0)
	{
		std::cout << "Calculating closures for " << itemsets.size() <<" using " << threadCount << " thread(s)...\n";
		//Reserve the space for closures.
		closures.resize(itemsets.size());
		//Divide the closures between all threads evenly.
		step = itemsets.size() / threadCount;
		std::vector<std::thread> workers;
		auto worker = [this](size_t start, size_t end) {
			for (size_t i = start; i < end; ++i)
			{
				//The closure of an itemset contains at least itself.
				Itemset closure{ itemsets[i].GetItems() };
				//If another itemset has the same TID, add its items to the closure.
				for (size_t j = 0; j < itemsets.size(); ++j)
				{
					if (i == j)
						continue;
					if (itemsets[i].IsTIDIncluded(itemsets[j]))
					{
						closure += itemsets[j];
					}
				}
				closures[i] = std::move(closure);
			}
		};
		for (size_t i = 0; i < threadCount; ++i)
		{
			workers.push_back(std::thread{ worker, step * i, step * (i + 1) });
		}
		for (auto& worker : workers)
		{
			worker.join();
		}
	}
	else
	{
		std::cout << "Calculating closures using main thread...\n";
	}
	//Finish the leftover closures on the main thread
	for (size_t i = step * threadCount; i < itemsets.size(); ++i)
	{
		//The closure of an itemset contains at least itself.
		Itemset closure{ itemsets[i].GetItems() };
		//If another itemset has the same TID, add its items to the closure.
		for (size_t j = 0; j < itemsets.size(); ++j)
		{
			if (i == j)
				continue;
			if (itemsets[i].IsTIDIncluded(itemsets[j]))
			{
				closure += itemsets[j];
			}
		}
		closures[i] = std::move(closure);
	}
}

void ItemsetGenerator::PruneUsingClosures(const ItemsetGenerator& previousGenerator)
{
	auto& closures = previousGenerator.GetClosures();
	auto includedInAnyClosure = [closures](const Itemset& itemset) {
		return std::any_of(closures.begin(), closures.end(), [&itemset](const Itemset& closure) {
			return closure.Includes(itemset);
			});
		};
	auto toErase = std::remove_if(itemsets.begin(), itemsets.end(), includedInAnyClosure);
	itemsets.erase(toErase, itemsets.end());
}

const std::vector<Itemset>& ItemsetGenerator::GetItemsets() const
{
	return itemsets;
}

const std::vector<Itemset>& ItemsetGenerator::GetClosures() const
{
	return closures;
}

bool ItemsetGenerator::IsEmpty() const
{
	return itemsets.empty();
}

size_t ItemsetGenerator::GetLevel() const
{
	return level;
}
