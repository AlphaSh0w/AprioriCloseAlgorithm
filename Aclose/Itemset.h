#pragma once
#include <vector>
#include <string>
#include "rapidcsv.h"
class Itemset
{
public:
	//Constructor to create an itemset from a list of items and provide a pre-built TID.
	Itemset(std::vector<std::pair<size_t, std::string>> items, std::vector<size_t> tid);
	//Constructor used to indicate which itemsets were merged when creating this itemset.
	Itemset(std::vector<std::pair<size_t, std::string>> items, const Itemset* first, const Itemset* second);
	//Calculates the TID directly from the TIDs of the merged itemsets.
	void CalculateTID();
	//Indicates if the provided itemset shares the first k items as this one.
	bool HasSameFirstKItems(const Itemset& other, size_t k) const;
	//Indicates if the itemset is possible (does not contain two values from the same column).
	bool IsValid() const;
	//Performs union between the items.
	Itemset operator +(const Itemset& rhs) const;
	float GetSupport(const size_t rowCount) const;
private:
	std::vector<std::pair<size_t, std::string>> items;
	std::vector<size_t> tid;

	//Used to keep track of which itemsets were merged to create this itemset.
	const Itemset* first = nullptr;
	const Itemset* second = nullptr;
};