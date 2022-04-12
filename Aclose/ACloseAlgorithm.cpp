#include "ACloseAlgorithm.h"
#include <assert.h>
#include "Rule.h"
void ACloseAlgorithm::Run(float minSupport)
{
	assert(kGenerators.empty());//Algorithm should only be ran once.
	CreateFirstGenerator(minSupport);
	//If we couldn't produce the first generator, stop the algorithm.
	if(kGenerators.size() == 0)
		return;
	//Keep creating generators until we cannot generate anymore.
	while(CreateKGenerator(minSupport));
	GenerateRules();
}

const std::vector<Rule>& ACloseAlgorithm::GetRules() const
{
	return rules;
}

void ACloseAlgorithm::CreateFirstGenerator(float minSupport)
{
	//Create the first generator.
	ItemsetGenerator firstGenerator{ currentLevel };
	firstGenerator.GenerateFirstItemsetsThreaded(document);
	firstGenerator.PruneUnfrequentItemsets(minSupport, document.GetRowCount());
	if (firstGenerator.IsEmpty())
		return;
	firstGenerator.CalculateClosures();
	kGenerators.emplace_back(std::move(firstGenerator));
	++currentLevel;
}

bool ACloseAlgorithm::CreateKGenerator(float minSupport)
{
	assert(kGenerators.size() == currentLevel-1);// Creating the (k) generator requires the (k-1) generator.
	ItemsetGenerator generator{ currentLevel };
	generator.GenerateItemsets(kGenerators.back());
	generator.CalculateTIDsThreaded();
	generator.PruneUnfrequentItemsets(minSupport, document.GetRowCount());
	generator.PruneUsingClosures(kGenerators.back());
	if (generator.IsEmpty())
		return false;
	generator.CalculateClosures();
	kGenerators.emplace_back(std::move(generator));
	++currentLevel;
	return true;
}

void ACloseAlgorithm::GenerateRules()
{
	for (const auto& generator : kGenerators)
	{
		const auto& itemsets = generator.GetItemsets();
		const auto& closureSets = generator.GetClosures();
		for (size_t i = 0; i < itemsets.size(); ++i)
		{
			//Only generate a rule if closure is bigger than itemset.
			if (!itemsets[i].hasSameItems(closureSets[i]))
			{
				rules.emplace_back(itemsets[i],closureSets[i]);
			}
		}
	}
}

ACloseAlgorithm::ACloseAlgorithm(const rapidcsv::Document& document)
	:
	document(document)
{
}
