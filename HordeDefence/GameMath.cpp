#include "stdafx.h"
#include "GameMath.h"
#include <math.h> 

namespace GameMath {

	GameMath::GameMath(void)
	{
	}

	bool GameMath::pointToRect(const Vector3D& vec, const Rectangle& rect)
	{
		if (vec.x < rect.mBottomLeft.x) return false;
		if (vec.y < rect.mBottomLeft.y) return false;
		if (vec.x >= rect.mBottomRight.x) return false;
		if (vec.y >= rect.mTopLeft.y) return false;
		return true;
	}

	bool GameMath::pointToCircle(const Vector3D& vec, const Circle& circle)
	{
		if ((vec - circle.mLocation).GetSquaredMagnitude() > pow(circle.mRadius, 2))
		{
			return false;
		}
		return true;
	}

	bool GameMath::circleToCircle(const Circle& circle1, const Circle& circle2)
	{
		float distance = (circle1.mLocation - circle2.mLocation).GetSquaredMagnitude();
		if (pow(circle1.mRadius - circle2.mRadius, 2) >= distance || distance <= pow(circle1.mRadius + circle2.mRadius, 2))
		{
			return true;
		}
		return false;
	}

	bool GameMath::circleToRect(const Circle& circle, const Rectangle& rect)
	{
		if (pointToRect(circle.mLocation, rect) ||
			lineToCircle(rect.mBottomLeft, rect.mTopLeft, circle) ||
			lineToCircle(rect.mTopLeft, rect.mTopRight, circle) ||
			lineToCircle(rect.mTopRight, rect.mBottomRight, circle) ||
			lineToCircle(rect.mBottomRight, rect.mBottomLeft, circle))
		{
			return true;
		}
		return false;
	}

	bool GameMath::rectToRect(const Rectangle& rect1, const Rectangle& rect2)
	{
		if (rect1.mBottomLeft.x <= rect2.mBottomRight.x && rect1.mBottomRight.x >= rect2.mBottomLeft.x  &&
			rect1.mBottomLeft.y <= rect2.mTopLeft.y && rect1.mTopLeft.y >= rect2.mBottomLeft.y)
		{
			return true;
		}
		return false;
	}

	bool GameMath::lineToCircle(const Vector3D& lineStart, const Vector3D& lineEnd, const Circle& circle)
	{
		Vector3D LocalP1 = lineStart - circle.mLocation;
		Vector3D LocalP2 = lineEnd - circle.mLocation;
		// Precalculate this value. We use it often
		Vector3D P2MinusP1 = LocalP2 - LocalP1;

		float a = (P2MinusP1.x) * (P2MinusP1.x) + (P2MinusP1.y) * (P2MinusP1.y);
		float b = 2 * ((P2MinusP1.x * LocalP1.x) + (P2MinusP1.y * LocalP1.y));
		float c = (LocalP1.x * LocalP1.x) + (LocalP1.y * LocalP1.y) - (circle.mRadius * circle.mRadius);
		float delta = b * b - (4 * a * c);
		if (delta < 0) // No intersection
			return false;
		return true;
	}


	//calculate and return dot product
	float GameMath::dot(const Vector3D &vec1, const Vector3D &vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	//calculate and return cross product
	Vector3D GameMath::cross(const Vector3D &vec1, const Vector3D &vec2)
	{
		return Vector3D(vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x);
	}
}