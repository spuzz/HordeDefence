#pragma once
#include "Vector3D.h"

namespace GameMath {

	class Rectangle
	{
	public:
		Rectangle(const Vector3D& nLocation, const float& nWidth, const float& nHeight);
		~Rectangle(void);

		float mWidth;
		float mHeight;

		// Corners
		Vector3D mBottomLeft;
		Vector3D mTopLeft;
		Vector3D mTopRight;
		Vector3D mBottomRight;
	private:

	};

}