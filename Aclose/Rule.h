#pragma once
#include <vector>
#include <string>
#include "Itemset.h"
#include "ItemsetGenerator.h"
class Rule
{
public:
	//Constructor, calculates the confidence using the kGenerators.
	Rule(const Itemset& itemset, const Itemset& closure, const std::vector<ItemsetGenerator>& kGenerators, size_t numRows);
	std::string ToString(const std::vector<std::string>& columnNames) const;
	const std::vector<Itemset::Item>& GetLeftHandSide() const;
	const std::vector<Itemset::Item>& GetRightHandSide() const;
private:
	float confidence;
	float lift;
	std::vector<Itemset::Item> leftHandSide;
	std::vector<Itemset::Item> rightHandSide;
};