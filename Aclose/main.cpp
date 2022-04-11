#include <rapidcsv.h> 
#include <iostream>
#include "DocumentLoader.h"
#include "Timer.h"
#include "ACloseAlgorithm.h"

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

    ACloseAlgorithm aClose{dataset.GetDocument()};
    std::cout << "Running AClose Algorithm ... ";
    timer.Mark();
    aClose.Run(0.1f);
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    //Save the new document to disk
    //dataset.SaveDocument("ClassData-discretized.csv");
}