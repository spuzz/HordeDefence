#pragma once

#include <map>
#include <string>
class Wave
{
public:
	Wave(int nTime);
	~Wave();

	int mWaveTime;
	std::map<std::pair<std::string, std::string>, int> mUnits;
	void AddUnit(std::string nName, std::string classType, int nQty);

private:
	
	
};

