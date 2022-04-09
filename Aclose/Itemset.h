#pragma once
#include <vector>
#include <string>

class Itemset
{
public:
	Itemset(std::vector<std::pair<size_t, std::string>> items);

private:
	std::vector<std::pair<size_t, std::string>> items;
	std::vector<size_t> tid;
};