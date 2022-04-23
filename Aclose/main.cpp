#include <rapidcsv.h> 
#include <iostream>
#include "DocumentLoader.h"
#include "Timer.h"
#include "ACloseAlgorithm.h"



int main()
{
    std::string fileName;
    std::vector<std::string> discretizeColumns;
    std::vector<int> binSizes;
    float minSup;
    DocumentLoader documentLoader;
    while (true)
    {
        std::cout << "Enter a valid file name : ";
        std::cin >> fileName;
        try
        {
            documentLoader.LoadFile(fileName);
            break;
        }
        catch (std::ios_base::failure e)
        {
            //File not found, do nothing and loop back.
            std::cout << "\n";
        }
    }
    //Display the column names
    std::cout << "\nDetected columns :\n";
    auto documentColumnNames = documentLoader.GetDocumentColumnNames();
    for (const auto& column : documentColumnNames)
    {
        std::cout << column << ", ";
    }
    std::cout << "\nProvide the columns to discretize (type q to finish) :\n";
    std::string columnName;
    //Ask about which columns to discretize
    while (true)
    {
        //Ask about the column name
        while (true)
        {
            std::cout << "Column name : ";
            std::cin >> columnName;
            if (columnName == "q")
                break;
            if (std::find(documentColumnNames.begin(), documentColumnNames.end(), columnName) != documentColumnNames.end())
            {
                discretizeColumns.emplace_back(columnName);
                break;
            }
            std::cout << "\nInvalid column name.\n";

        }
        if (columnName == "q")
        {
            break;
        }
        //Ask about the bin size
        size_t binSize;
        std::cout << "Number of bins : ";
        std::cin >> binSize;
        binSizes.push_back(binSize);
    }
    //Execute with the given parameters
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