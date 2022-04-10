#include <rapidcsv.h> 
#include <iostream>
#include "DocumentLoader.h"
#include "Timer.h"
#include "ItemsetGenerator.h"

int main()
{
    std::cout << "Loading csv ... ";
    Timer<std::chrono::milliseconds> timer{};
    //Load the airlines csv
    DocumentLoader dataset{ "ClassData.csv" };
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    std::cout << "Discretizing ... ";
    timer.Mark();
    dataset.Discretize<float>({"raisedhands","VisITedResources","AnnouncementsView","Discussion"}, {10,10,10,10});
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    ItemsetGenerator oneGenerator{1};
    ItemsetGenerator twoGenerator{2};
    oneGenerator.GenerateFirstItemsetsThreaded(dataset.GetDocument());
    oneGenerator.PruneUnfrequentItemsets(0.05f, dataset.GetDocument().GetRowCount());
    oneGenerator.CalculateClosures();
    twoGenerator.GenerateItemsets(oneGenerator);
    twoGenerator.PruneUsingClosures(oneGenerator);
    twoGenerator.CalculateTIDsThreaded();
    twoGenerator.PruneUnfrequentItemsets(0.05f, dataset.GetDocument().GetRowCount());
    twoGenerator.CalculateClosures();
    //Save the new document to disk
    dataset.SaveDocument("Airlines-discretized.csv");
}