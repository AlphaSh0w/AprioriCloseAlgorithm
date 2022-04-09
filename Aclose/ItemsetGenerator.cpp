#include "ItemsetGenerator.h"
#include <map>
#include <thread>

ItemsetGenerator::ItemsetGenerator(size_t level)
	:
	level(level)
{
}

void ItemsetGenerator::GenerateItemsets(const rapidcsv::Document& document)
{
	if(level == 1)
		GenerateFirstItemsets(document);
}

void ItemsetGenerator::GenerateFirstItemsets(const rapidcsv::Document& document)
{
	std::vector<std::map<std::string, std::vector<size_t>>> tids;
	//Allocate the necessarry memory to hold the TIDs
	tids.resize(document.GetColumnCount());
	//Create a worker thread for each column
	std::vector<std::thread> workers;
	//Go through the document and generate the columns values and the TIDs.
	for (size_t iColumn = 0; iColumn < document.GetColumnCount(); ++iColumn)
	{
		const auto worker = [&tids](const rapidcsv::Document& document, const size_t iColumn) {
			//Generate the column values for this column and fill its part in the TIDs.
			auto column = document.GetColumn<std::string>(iColumn);
			for (size_t iRow = 0; iRow < column.size(); ++iRow)
			{
				//Fill the TID of this cell
				tids[iColumn][column[iRow]].emplace_back(iRow);
			}
		};
		workers.push_back(std::thread{ worker, std::ref(document), iColumn });
	}
	//Wait for all workers to finish
	for (auto& worker : workers)
	{
		worker.join();
	}
	//Figure out and reserve the required space to hold all itemsets
	size_t requiredSpace = 0;
	for (const auto& column : tids)
	{
		requiredSpace += column.size();
	}
	itemsets.reserve(requiredSpace);
	//Fill the itemsets and give them their tids.
	for (size_t iColumn = 0; iColumn < document.GetColumnCount(); ++iColumn)
	{
		for (auto& valueTIDpair : tids[iColumn])
		{
			itemsets.emplace_back(std::vector{std::pair{iColumn,valueTIDpair.first}},std::move(valueTIDpair.second));
		}
	}
}
