#pragma once
#include <vector>
#include "ItemsetGenerator.h"

class ACloseAlgorithm
{
public:
	ACloseAlgorithm() = default;
	void Run();
private:
	std::vector<ItemsetGenerator> kGenerators;
};