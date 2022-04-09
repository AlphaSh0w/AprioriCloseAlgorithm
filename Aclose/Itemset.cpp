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

bool Itemset::HasSameFirstKItems(const Itemset& other, size_t k) const
{
	assert(items.size() >= k && other.items.size() >= k);//Both itemsets have to be longer or equal to k.
	bool inCommon = true;
	for (size_t i = 0; i < k; ++i)
	{
		if (items[i] != other.items[i])
		{
			inCommon = false;
			break;
		}
	}
	return inCommon;
}

bool Itemset::IsValid() const
{
	auto result = std::adjacent_find(items.begin(),
		items.end(),
		[](const std::pair<size_t, std::string>& first, const std::pair<size_t, std::string>& second) {
			return first.first == second.first;
		});

	return result == items.end();
}

Itemset Itemset::operator+(const Itemset& rhs) const
{
	std::vector<std::pair<size_t, std::string>> itemsUnion;
	std::set_union(
		items.begin(), items.end(),
		rhs.items.begin(), rhs.items.end(),
		std::back_inserter(itemsUnion)
	);
	return Itemset(itemsUnion,this,&rhs);
}
