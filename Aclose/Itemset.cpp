#include "Itemset.h"

Itemset::Itemset(const std::vector<Item>& items)
	:
	items(items)
{
}

Itemset::Itemset(std::vector<Item> items, std::vector<size_t> tid, size_t rowCount)
	:
	items(std::move(items)),
	tid(std::move(tid))
{
	support = static_cast<float>(this->tid.size()) / rowCount;
}

Itemset::Itemset(std::vector<Item> items, const Itemset* first, const Itemset* second)
	:
	items(items),
	first(first),
	second(second)
{
}

void Itemset::CalculateTID(size_t rowCount)
{
	assert(first != nullptr && second != nullptr); //To calculate its TID, this itemset needs to be created from a union of two other itemsets.
	//Performs an intersection between the TIDs of the two itemsets that were used to create this itemset.
	std::set_intersection(
		first->tid.begin(), first->tid.end(),
		second->tid.begin(), second->tid.end(),
		std::back_inserter(tid)
	);
	support = static_cast<float>(this->tid.size()) / rowCount;
}

bool Itemset::HasSameFirstKItems(const Itemset& other, size_t k) const
{
	assert(items.size() >= k && other.items.size() >= k);//Both itemsets have to be longer or equal to k.
	return std::equal(items.begin(), items.begin()+k,other.items.begin());
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
		[](const Item& first, const Item& second) {
			return first.first == second.first;
		});
	return result == items.end();
}

bool Itemset::hasSameItems(const Itemset& other) const
{
	return HasSameFirstKItems(other, items.size());
}

const std::vector<size_t>& Itemset::GetTID() const
{
	return tid;
}

Itemset Itemset::operator+(const Itemset& rhs) const
{
	std::vector<Item> itemsUnion;
	std::set_union(
		items.begin(), items.end(),
		rhs.items.begin(), rhs.items.end(),
		std::back_inserter(itemsUnion)
	);
	return Itemset(itemsUnion,this,&rhs);
}

Itemset& Itemset::operator+=(const Itemset& rhs)
{
	std::vector<Item> itemsUnion;
	std::set_union(
		items.begin(), items.end(),
		rhs.items.begin(), rhs.items.end(),
		std::back_inserter(itemsUnion)
	);
	items = std::move(itemsUnion);
	return *this;
}

const std::vector<Itemset::Item>& Itemset::GetItems() const
{
	return items;
}

float Itemset::GetSupport() const
{
	assert(support != -1.f);//Support should be calculated before being used.
	return support;
}
