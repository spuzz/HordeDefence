#pragma once
#include "Vector3D.h"

namespace GameMath {

	class Circle
	{
	public:
		Circle(const Vector3D& mLocation, const float& nRadius);
		~Circle(void);
		// Setters

		void setLocation(const Vector3D& nLocation) { mLocation = nLocation; }

		// These functions will change related variables ( Diameter/Radius/circum )
		void setDiameter(const float& nDiameter);
		void setRadius(const float& nRadius);

		Vector3D mLocation;
		float mDiameter;
		float mRadius;
		float mCircumference;

	private:

		void calculateCircumference(const float& nDiameter);
	};
}

