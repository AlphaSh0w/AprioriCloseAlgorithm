#pragma once
#include <vector>
#include "rapidcsv.h"
#include "ItemsetGenerator.h"
#include "Rule.h"

class ACloseAlgorithm
{
public:
	ACloseAlgorithm(const rapidcsv::Document& document);
	void Run(float minSupport);
	const std::vector<Rule>& GetRules() const;
private:
	void CreateFirstGenerator(float minSupport);
	bool CreateKGenerator(float minSupport);
	void GenerateRules();
private:
	std::vector<ItemsetGenerator> kGenerators;
	std::vector<Rule> rules;
	size_t currentLevel = 1;
	const rapidcsv::Document& document;
};