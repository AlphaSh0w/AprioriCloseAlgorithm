#include "Rule.h"
#include <assert.h>
Rule::Rule(const Itemset& itemset, const Itemset& closure)
	:
	confidence(1.f)
{
	const auto& itemsetItems = itemset.GetItems();
	const auto& closureItems = closure.GetItems();
	assert(!std::equal(itemsetItems.begin(),itemsetItems.end(),closureItems.begin()));//Itemset and closure should not be equal.
	//Fill the left hand side
	leftHandSide = itemset.GetItems();
	//Fill the right hand side
	std::set_difference(closureItems.begin(), closureItems.end(),
						itemsetItems.begin(),itemsetItems.end(),
						std::back_inserter(rightHandSide));
}
