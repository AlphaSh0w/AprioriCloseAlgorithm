#pragma once
#include <vector>
#include "rapidcsv.h"
#include "ItemsetGenerator.h"

class ACloseAlgorithm
{
public:
	ACloseAlgorithm(const rapidcsv::Document& document);
	void Run(float minSupport);
private:
	std::vector<ItemsetGenerator> kGenerators;
	size_t currentLevel = 1;

	const rapidcsv::Document& document;
};