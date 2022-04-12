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
    aClose.Run(0.2f);
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    std::cout << "\nDisplaying " << aClose.GetRules().size() << " rules : \n";
    for (const auto& rule : aClose.GetRules())
    {
        auto columnNames = dataset.GetDocumentColumnNames();
        std::cout << rule.ToString(columnNames);
    }
    //Save the new document to disk
    //dataset.SaveDocument("ClassData-discretized.csv");
}