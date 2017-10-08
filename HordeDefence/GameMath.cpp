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
		Vector3D intersec;
		if (pointToRect(circle.mLocation, rect) ||
			lineToCircle(rect.mBottomLeft, rect.mTopLeft, circle, intersec) ||
			lineToCircle(rect.mTopLeft, rect.mTopRight, circle, intersec) ||
			lineToCircle(rect.mTopRight, rect.mBottomRight, circle, intersec) ||
			lineToCircle(rect.mBottomRight, rect.mBottomLeft, circle, intersec))
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

	bool GameMath::lineToCircle(const Vector3D& lineStart, const Vector3D& lineEnd, const Circle& circle, Vector3D& intersection )
	{
		Vector3D d = lineEnd - lineStart;
		Vector3D f = lineStart - circle.mLocation;
		float a = GameMath::dot(d, d);
		float b = 2 * GameMath::dot(f, d);
		float c = GameMath::dot(f, f) - (circle.mRadius*circle.mRadius);
		//Vector3D LocalP1 = lineStart - circle.mLocation;
		//Vector3D LocalP2 = lineEnd - circle.mLocation;
		//// Precalculate this value. We use it often
		//Vector3D P2MinusP1 = LocalP2 - LocalP1;

		//float a = (P2MinusP1.x) * (P2MinusP1.x) + (P2MinusP1.y) * (P2MinusP1.y);
		//float b = 2 * ((P2MinusP1.x * LocalP1.x) + (P2MinusP1.y * LocalP1.y));
		//float c = (LocalP1.x * LocalP1.x) + (LocalP1.y * LocalP1.y) - (circle.mRadius * circle.mRadius);

		float delta = b * b - 4 * a * c;
		if (delta < 0) // No intersection
			return false;

		delta = sqrt(delta);

		// either solution may be on or off the ray so need to test both
		// t1 is always the smaller value, because BOTH discriminant and
		// a are nonnegative.
		float t1 = (-b - delta) / (2 * a);
		float t2 = (-b + delta) / (2 * a);
		if (t1 > 1 && t2 > 1)
			return false;
		if (t1 < 0 && t2 < 0)
		{
			return false;
		}
		intersection = lineStart + ((lineEnd - lineStart)*t1);
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

	Vector3D GameMath::LineToRectPoint(const Vector3D& lineStart, const Vector3D& lineEnd, const Rectangle& rect)
	{
		Vector3D closest(-1, -1, -1);
		Vector3D tmpVec(-1, -1, -1);
		if (get_line_intersection(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, rect.mBottomLeft.x, rect.mBottomLeft.y, rect.mTopLeft.x, rect.mTopLeft.y, tmpVec))
		{
			if (closest.x == -1 || (tmpVec - lineStart).GetSquaredMagnitude() < (closest - lineStart).GetSquaredMagnitude())
			{
				closest = tmpVec;
			}
		}
		if (get_line_intersection(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, rect.mBottomLeft.x, rect.mBottomLeft.y, rect.mBottomRight.x, rect.mBottomRight.y, tmpVec))
		{
			if (closest.x == -1 || (tmpVec - lineStart).GetSquaredMagnitude() < (closest - lineStart).GetSquaredMagnitude())
			{
				closest = tmpVec;
			}
		}
		if (get_line_intersection(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, rect.mBottomRight.x, rect.mBottomRight.y, rect.mTopRight.x, rect.mTopRight.y, tmpVec))
		{
			if (closest.x == -1 || (tmpVec - lineStart).GetSquaredMagnitude() < (closest - lineStart).GetSquaredMagnitude())
			{
				closest = tmpVec;
			}
		}
		if (get_line_intersection(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, rect.mTopLeft.x, rect.mTopLeft.y, rect.mTopRight.x, rect.mTopRight.y, tmpVec))
		{
			if (closest.x == -1 || (tmpVec - lineStart).GetSquaredMagnitude() < (closest - lineStart).GetSquaredMagnitude())
			{
				closest = tmpVec;
			}
		}
		return closest;
	}

	bool GameMath::get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
		float p2_x, float p2_y, float p3_x, float p3_y, Vector3D& vec)
	{
		float s1_x, s1_y, s2_x, s2_y;
		s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
		s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

		float s, t;
		s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
		t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			// Collision detected
			vec.x = p0_x + (t * s1_x);
			vec.y = p0_y + (t * s1_y);
			return true;
		}

		return false; // No collision
	}
}