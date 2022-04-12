#include "Rule.h"
#include <assert.h>
#include <format>

Rule::Rule(const Itemset& itemset, const Itemset& closure)
	:
	confidence(1.f)
{
	const auto& itemsetItems = itemset.GetItems();
	const auto& closureItems = closure.GetItems();
	assert(!std::equal(itemsetItems.begin(),itemsetItems.end(),closureItems.begin()));//Itemset and closure should not be equal.
	//Fill the left hand side
	leftHandSide = itemsetItems;
	//Fill the right hand side
	std::set_difference(closureItems.begin(), closureItems.end(),
						itemsetItems.begin(),itemsetItems.end(),
						std::back_inserter(rightHandSide));
}

std::string Rule::ToString(const std::vector<std::string>& columnNames) const
{
	std::string string;
	//Represent the left hand side.
	for (const auto& item : leftHandSide)
	{
		string += std::format("{}={}, ",columnNames[item.first],item.second);
	}
	string += "==> ";
	for (const auto& item : rightHandSide)
	{
		string += std::format("{}={}, ", columnNames[item.first], item.second);
	}
	string += '\n';
	return std::move(string);
}

const std::vector<Itemset::Item>& Rule::GetLeftHandSide() const
{
	return leftHandSide;
}

const std::vector<Itemset::Item>& Rule::GetRightHandSide() const
{
	return rightHandSide;
}
