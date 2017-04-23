#include "stdafx.h"
#include "Circle.h"
#include <math.h>

namespace GameMath {

	double pi = 3.1415926535897;

	Circle::Circle(const Vector3D& nLocation, const float& nRadius) : mLocation(nLocation), mRadius(nRadius)
	{
		mDiameter = nRadius * 2;
		calculateCircumference(mDiameter);
	}

	void Circle::setDiameter(const float& nDiameter)
	{
		mDiameter = nDiameter;
		mRadius = mDiameter / 2;
		calculateCircumference(mDiameter);
	}

	void Circle::setRadius(const float& nRadius)
	{
		mRadius = nRadius;
		mDiameter = mRadius * 2;
		calculateCircumference(mDiameter);
	}

	void Circle::calculateCircumference(const float& nDiameter)
	{
		mCircumference = nDiameter*pi;
	}


	Circle::~Circle(void)
	{
	}
}