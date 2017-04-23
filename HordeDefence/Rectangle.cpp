#include "stdafx.h"
#include "Rectangle.h"


namespace GameMath {
	Rectangle::Rectangle(const Vector3D& nLocation, const float& nWidth, const float& nHeight) : mBottomLeft(nLocation), mHeight(nHeight), mWidth(nWidth)
	{
		mTopLeft = mBottomLeft + Vector3D(0, mHeight, 0);
		mTopRight = mBottomLeft + Vector3D(mWidth, mHeight, 0);
		mBottomRight = mBottomLeft + Vector3D(mWidth, 0, 0);
	}


	Rectangle::~Rectangle(void)
	{
	}
}