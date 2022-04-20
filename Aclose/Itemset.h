#pragma once
#include <vector>
#include <string>
#include "rapidcsv.h"
class Itemset
{
public:
	typedef std::pair<size_t, std::string> Item;
public:
	Itemset() = default;
	//Constructor to create an itemset from a list of items.
	Itemset(const std::vector<Item>& items);
	//Constructor to create an itemset from a list of items and provide a pre-built TID.
	Itemset(std::vector<Item> items, std::vector<size_t> tid, size_t rowCount);
	//Constructor used to indicate which itemsets were merged when creating this itemset.
	Itemset(std::vector<Item> items, const Itemset* first, const Itemset* second);
	//Calculates the TID directly from the TIDs of the merged itemsets.
	void CalculateTID(size_t rowCount);
	//Indicates if the provided itemset shares the first k items as this one.
	bool HasSameFirstKItems(const Itemset& other, size_t k) const;
	//Indicates if the TID of this itemset is included in the TID of the other itemset.
	bool IsTIDIncluded(const Itemset& other) const;
	//Indicates if this itemset includes all the items of other.
	bool Includes(const Itemset& other) const;
	//Indicates if the itemset is possible (does not contain two values from the same column).
	bool IsValid() const;
	//Indicates if the itemset contains the same items as other.
	bool hasSameItems(const Itemset& other) const;
	//Performs a union between the items and returns the result in an itemset.
	Itemset operator +(const Itemset& rhs) const;
	//Performs a union between the items and stores the result in this itemset.
	Itemset& operator +=(const Itemset& rhs);
	const std::vector<Item>& GetItems() const;
	float GetSupport() const;
private:
	std::vector<Item> items;
	std::vector<size_t> tid;
	float support = -1.f;
	//Used to keep track of which itemsets were merged to create this itemset.
	const Itemset* first = nullptr;
	const Itemset* second = nullptr;
};