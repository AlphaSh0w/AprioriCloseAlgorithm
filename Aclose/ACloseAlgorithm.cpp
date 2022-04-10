#include "ACloseAlgorithm.h"
#include <assert.h>
void ACloseAlgorithm::Run(float minSupport)
{
	assert(kGenerators.empty());//Algorithm should only be ran once.
	//Create the first generator.
	ItemsetGenerator firstGenerator{currentLevel};
	firstGenerator.GenerateFirstItemsetsThreaded(document);
	firstGenerator.PruneUnfrequentItemsets(minSupport, document.GetRowCount());
	if (firstGenerator.IsEmpty())
		return;
	firstGenerator.CalculateClosures();
	kGenerators.emplace_back(std::move(firstGenerator));
	++currentLevel;
	//Keep creating generators until we cannot generate anymore.
	while(true)
	{
		ItemsetGenerator generator{currentLevel};
		generator.GenerateItemsets(kGenerators.back());
		generator.CalculateTIDsThreaded();
		generator.PruneUnfrequentItemsets(minSupport, document.GetRowCount());
		generator.PruneUsingClosures(kGenerators.back());
		if (generator.IsEmpty())
			break;
		generator.CalculateClosures();
		kGenerators.emplace_back(std::move(generator));
		++currentLevel;
	}
}

ACloseAlgorithm::ACloseAlgorithm(const rapidcsv::Document& document)
	:
	document(document)
{
}
