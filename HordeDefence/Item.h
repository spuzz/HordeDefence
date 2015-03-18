#pragma once
#include <iostream>

using std::string;
class Item
{
public:
	Item();
	virtual ~Item();

	const string getName() const { return mName; }
	const string getType() const { return mType; }

	void setName(string inName) { mName = inName; }
	void setType(string inType) { mType = inType; }
private:
	string mName;
	string mType;
};

