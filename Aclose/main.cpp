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
    DocumentLoader dataset{ "airlines.csv" };
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    std::cout << "Discretizing ... ";
    timer.Mark();
    dataset.Discretize<float>({"Flight","Time","Length"},{10,10,10});
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    ItemsetGenerator oneGenerator{1};
    ItemsetGenerator twoGenerator{2};
    oneGenerator.GenerateFirstItemsets(dataset.GetDocument());
    twoGenerator.GenerateItemsets(oneGenerator);
    twoGenerator.CalculateTIDsMultiThreaded();
    //Save the new document to disk
    dataset.SaveDocument("Airlines-discretized.csv");
}