#include <rapidcsv.h> 
#include <iostream>
#include "DocumentLoader.h"
#include "Timer.h"
#include "ACloseAlgorithm.h"



int main()
{
    rapidcsv::Document settings{"discretizationSettings.csv"};
    std::vector<std::string> discretizeColumns = settings.GetColumn<std::string>(0);
    std::vector<int> binSizes = settings.GetColumn<int>(1);
    std::string fileName;
  
    float minSup = 0.015f;
    DocumentLoader documentLoader;
    while (true)
    {
        std::cout << "Enter a valid file name : ";
        std::cin >> fileName;
        try
        {
            documentLoader.LoadFile(fileName);
            std::cout << "\ncsv loaded.\n";
            auto temp = documentLoader.GetDocument().GetColumn<std::string>(0);
            break;
        }
        catch (std::ios_base::failure e)
        {
            //File not found, do nothing and loop back.
            std::cout << "\n";
        }
    }
    std::cout << "\nEnter the minSup : ";
    std::cin >> minSup;
    
    //Execute with the given parameters
    
    std::cout << "Discretizing ... ";
    Timer<std::chrono::milliseconds> timer{};
    documentLoader.Discretize<float>(discretizeColumns, binSizes);
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    ACloseAlgorithm aClose{documentLoader.GetDocument()};
    std::cout << "Running AClose Algorithm ... ";
    timer.Mark();
    aClose.Run(minSup);
    long long algorithmTime = timer.Mark();
    std::cout << "\n\nAlgorithm took " << algorithmTime << " miliseconds.\n";
    std::cout << "\nDisplaying " << aClose.GetRules().size() << " rules: \n";
    for (const auto& rule : aClose.GetRules())
    {
        auto columnNames = documentLoader.GetDocumentColumnNames();
        std::cout << rule.ToString(columnNames);
    }
    std::cout << "Displayed " << aClose.GetRules().size() << " rules. \n";
    std::cout << "\n\nAlgorithm took " << algorithmTime << " miliseconds.\n";
    //Save the new document to disk
    //dataset.SaveDocument("ClassData-discretized.csv");
}