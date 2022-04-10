#include "Itemset.h"

Itemset::Itemset(const std::vector<std::pair<size_t, std::string>>& items)
	:
	items(items)
{
}

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

void Itemset::CalculateTID()
{
	assert(first != nullptr && second != nullptr); //To calculate its TID, this itemset needs to be created from a union of two other itemsets.
	//Performs an intersection between the TIDs of the two itemsets that were used to create this itemset.
	std::set_intersection(
		first->tid.begin(), first->tid.end(),
		second->tid.begin(), second->tid.end(),
		std::back_inserter(tid)
	);
}

bool Itemset::HasSameFirstKItems(const Itemset& other, size_t k) const
{
	assert(items.size() >= k && other.items.size() >= k);//Both itemsets have to be longer or equal to k.
	bool bSame = true;
	for (size_t i = 0; i < k; ++i)
	{
		if (items[i] != other.items[i])
		{
			bSame = false;
			break;
		}
	}
	return bSame;
}

bool Itemset::IsTIDIncluded(const Itemset& other) const
{
	//The smallest TID needs to go as a first argument.
	return std::includes(other.tid.begin(), other.tid.end(), tid.begin(), tid.end());
}

bool Itemset::Includes(const Itemset& other) const
{
	return std::includes(items.begin(), items.end(), other.items.begin(), other.items.end());
}

bool Itemset::IsValid() const
{
	auto result = std::adjacent_find(items.begin(), items.end(),
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

Itemset& Itemset::operator+=(const Itemset& rhs)
{
	std::vector<std::pair<size_t, std::string>> itemsUnion;
	std::set_union(
		items.begin(), items.end(),
		rhs.items.begin(), rhs.items.end(),
		std::back_inserter(itemsUnion)
	);
	items = std::move(itemsUnion);
	return *this;
}

const std::vector<std::pair<size_t, std::string>>& Itemset::GetItems() const
{
	return items;
}

float Itemset::GetSupport(const size_t rowCount) const
{
	return static_cast<float>(tid.size()) / rowCount;
}
