#pragma once
#include "Rectangle.h"
#include "Circle.h"

namespace GameMath {

	const float DEG2RAD = 3.14159 / 180;

	class GameMath
	{
	public:
		GameMath(void);

		// Intersection functions;
		static bool pointToRect(const Vector3D& vec, const Rectangle& rect);
		static bool pointToCircle(const Vector3D& vec, const Circle& circle);
		static bool circleToCircle(const Circle& circle1, const Circle& circle2);
		static bool circleToRect(const Circle& circle, const Rectangle& rect);
		static bool rectToRect(const Rectangle& rect1, const Rectangle& rect2);
		static bool lineToCircle(const Vector3D& lineStart, const Vector3D& lineEnd, const Circle& circle, Vector3D& intersection);
		static bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, Vector3D& vec);
		static Vector3D LineToRectPoint(const Vector3D& lineStart, const Vector3D& lineEnd, const Rectangle& rect);
		// Vector Math Functions
		static float dot(const Vector3D &vec1, const Vector3D &vec2);
		static Vector3D cross(const Vector3D &vec1, const Vector3D &vec2);

	};

}