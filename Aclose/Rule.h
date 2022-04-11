#pragma once
#include <vector>
#include <string>
#include "ItemsetGenerator.h"
class Rule
{
public:
	//Constructor, calculates the confidence and lift using the kGenerators.
	Rule(const Itemset& itemset, const Itemset& closure);
private:
	float confidence;
	std::vector<Itemset::Item> leftHandSide;
	std::vector<Itemset::Item> rightHandSide;
};