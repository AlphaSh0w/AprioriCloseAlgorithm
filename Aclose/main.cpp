#include <rapidcsv.h> 
#include <iostream>
#include "DocumentLoader.h"
#include "Timer.h"
#include "ACloseAlgorithm.h"

std::string fileName = "ClassData.csv";
std::vector<std::string> discretizeColumns = { "raisedhands","VisITedResources","AnnouncementsView","Discussion" };
std::vector<int> binSizes = { 10,10,10,10 };
const float minSup = 0.15f;

int main()
{
    std::cout << "Loading csv ... ";
    Timer<std::chrono::milliseconds> timer{};
    //Load the airlines csv
    DocumentLoader dataset{ fileName };
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    std::cout << "Discretizing ... ";
    timer.Mark();
    dataset.Discretize<float>(discretizeColumns, binSizes);
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    ACloseAlgorithm aClose{dataset.GetDocument()};
    std::cout << "Running AClose Algorithm ... ";
    timer.Mark();
    aClose.Run(minSup);
    std::cout << "\n\nAlgorithm took " << timer.Mark() << " miliseconds.\n";
    std::cout << "\nDisplaying " << aClose.GetRules().size() << " rules: \n";
    for (const auto& rule : aClose.GetRules())
    {
        auto columnNames = dataset.GetDocumentColumnNames();
        std::cout << rule.ToString(columnNames);
    }
    std::cout << "Displayed " << aClose.GetRules().size() << " rules. \n";
    //Save the new document to disk
    //dataset.SaveDocument("ClassData-discretized.csv");
}