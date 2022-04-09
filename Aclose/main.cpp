#include <rapidcsv.h> 
#include <iostream>
#include "DocumentLoader.h"
#include "Timer.h"
int main()
{
    std::cout << "Loading csv ... ";
    Timer<std::chrono::milliseconds> timer{};
    //Load the airlines csv
    DocumentLoader itemset{ "airlines.csv" };
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    std::cout << "Discretizing ... ";
    timer.Mark();
    itemset.Discretize<float>({"Flight","Time","Length"},{10,10,10});
    std::cout << "took " << timer.Mark() << " miliseconds\n";

    //Save the new document to disk
    itemset.SaveDocument("Airlines-discretized.csv");
}