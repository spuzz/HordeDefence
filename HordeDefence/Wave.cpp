#include "Wave.h"



Wave::Wave(int nTime) : mWaveTime(nTime)
{
}

void Wave::AddUnit(std::string nName, std::string classType, int nQty)
{
	mUnits[std::pair<std::string, std::string>(nName, classType)] = nQty;
}

Wave::~Wave()
{
}
