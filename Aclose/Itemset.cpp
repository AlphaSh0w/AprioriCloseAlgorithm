#include "Itemset.h"

Itemset::Itemset(std::vector<std::pair<size_t, std::string>> items)
	:
	items(std::move(items))
{
}