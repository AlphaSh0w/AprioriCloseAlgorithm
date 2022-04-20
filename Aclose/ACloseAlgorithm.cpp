#include "ACloseAlgorithm.h"
#include <assert.h>
#include <stdio.h>
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
	std::cout << "\n\nGenerating 1-Itemsets...\n";
	ItemsetGenerator firstGenerator{ currentLevel };
	firstGenerator.GenerateFirstItemsetsThreaded(document);
	std::cout << "Prunning unfrequent itemsets (minSupport = " << minSupport << " )...\n";
	firstGenerator.PruneUnfrequentItemsets(minSupport);
	std::cout << firstGenerator.GetItemsets().size() << " itemset(s) left.\n";
	if (firstGenerator.IsEmpty())
	{
		std::cout << "Stopping algorithm.\n";
		return;
	}
	firstGenerator.CalculateClosures();
	kGenerators.emplace_back(std::move(firstGenerator));
	++currentLevel;
}

bool ACloseAlgorithm::CreateKGenerator(float minSupport)
{
	assert(kGenerators.size() == currentLevel-1);// Creating the (k) generator requires the (k-1) generator.
	std::cout << "\nGenerating " << currentLevel << "-Itemsets...\n";
	ItemsetGenerator generator{ currentLevel };
	generator.GenerateItemsets(kGenerators.back());
	std::cout << "Found " << generator.GetItemsets().size() << " itemsets.\n";
	generator.CalculateTIDsThreaded(document.GetRowCount());
	std::cout << "Prunning unfrequent itemsets (minSupport = " << minSupport << " )...\n";
	generator.PruneUnfrequentItemsets(minSupport);
	std::cout << generator.GetItemsets().size() << " itemset(s) left.\n";
	std::cout << "Pruning using closures...\n";
	generator.PruneUsingClosures(kGenerators.back());
	std::cout << generator.GetItemsets().size() << " itemset(s) left.\n";
	if (generator.IsEmpty())
	{
		std::cout << "Stopping algorithm.\n";
		return false;
	}
	generator.CalculateClosuresThreaded();
	kGenerators.emplace_back(std::move(generator));
	++currentLevel;
	return true;
}

void ACloseAlgorithm::GenerateRules()
{
	std::cout << "\nGenerating rules...\n";
	for (const auto& generator : kGenerators)
	{
		const auto& itemsets = generator.GetItemsets();
		const auto& closureSets = generator.GetClosures();
		for (size_t i = 0; i < itemsets.size(); ++i)
		{
			//Only generate a rule if closure is bigger than itemset.
			if (!itemsets[i].hasSameItems(closureSets[i]))
			{
				rules.emplace_back(itemsets[i],closureSets[i], kGenerators);
			}
		}
	}
}

ACloseAlgorithm::ACloseAlgorithm(const rapidcsv::Document& document)
	:
	document(document)
{
}
