#include "Itemset.h"

Itemset::Itemset(std::vector<std::pair<size_t, std::string>> items, std::vector<size_t> tid)
	:
	items(std::move(items)),
	tid(std::move(tid))
{
}

Itemset::Itemset(std::vector<std::pair<size_t, std::string>> items, const Itemset* first, const Itemset* second)
	:
	items(items),
	first(first),
	second(second)
{
}

void Itemset::CalculateTID(rapidcsv::Document& document)
{
}
