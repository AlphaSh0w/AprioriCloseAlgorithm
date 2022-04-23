#include "Rule.h"
#include <assert.h>
#include <format>

Rule::Rule(const Itemset& itemset, const Itemset& closure, const std::vector<ItemsetGenerator>& kGenerators, size_t numRows)
	:
	confidence(1.f),
	lift(-1.f)
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
	//We need the support of the right hand side to calculate the lift, so we search for it in its corresponding kGenerator.
	size_t k = rightHandSide.size()-1;
	if (k <= kGenerators.size() - 1)
	{
		//it has already been calculated, get its TID
		const auto& kGenerator = kGenerators[k];
		const Itemset& rItemset = kGenerator.GetItemset(rightHandSide);
		lift = itemset.GetSupport() / (itemset.GetSupport() * rItemset.GetSupport());
	}
	else
	{
		//We won't find it in the generators, gotta calculate the TID.
		const auto& firstKGenerator = kGenerators[0];
		const Itemset& item = firstKGenerator.GetItemset(std::vector{rightHandSide[0]});
		std::vector<size_t> intersectedTID = item.GetTID();
		std::vector<size_t> temp;
		for (size_t i = 1; i <= k; ++i)
		{
			const Itemset& otherItem = firstKGenerator.GetItemset(std::vector{rightHandSide[i]});
			std::set_intersection(intersectedTID.begin(),intersectedTID.end(),
								  otherItem.GetTID().begin(),otherItem.GetTID().end(),
								  std::back_inserter(temp));
			intersectedTID = std::move(temp);
		}
		lift = itemset.GetSupport() / (itemset.GetSupport() * (intersectedTID.size()/numRows));
	}
	
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
	string += std::format("\t lift = {}\n", lift);
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
