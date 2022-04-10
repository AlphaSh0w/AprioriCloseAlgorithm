#include "ACloseAlgorithm.h"
#include <assert.h>
void ACloseAlgorithm::Run(float minSupport)
{
	assert(kGenerators.empty());//Algorithm should only be ran once.
	
	while (true)
	{
		//Generate k-itemsets.
		ItemsetGenerator generator{currentLevel};
		if (currentLevel == 1)
		{
			//Generate the first itemsets from the document.
			generator.GenerateFirstItemsetsThreaded(document);

		}
		else
		{
			generator.GenerateItemsets(kGenerators.back());

		}
		//if generator is empty, stop algorithm.
		if (generator.IsEmpty())
			break;
		if(currentLevel != 1)
			//Calculate their TIDs (gives us their supports).
			generator.CalculateTIDsThreaded();
		//Prune unfrequent itemsets.
		generator.PruneUnfrequentItemsets(minSupport, document.GetRowCount());
		if(currentLevel != 1)
			//Prune using closures
			generator.PruneUsingClosures(kGenerators.back());
		//Calculate their closure.
		generator.CalculateClosures();
		//Add to list of kGenerators.
		kGenerators.emplace_back(std::move(generator));
		++currentLevel;
	}
	kGenerators.emplace_back(currentLevel);
	
	
	
	
	//Calculate their support (Already done in previous step)
	
	
}

ACloseAlgorithm::ACloseAlgorithm(const rapidcsv::Document& document)
	:
	document(document)
{
}
