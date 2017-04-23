#ifndef _VECTOR
#define _VECTOR
#include <math.h>
namespace GameMath {
	class Vector3D
	{
	private:

	public:

		float x, y, z;

		//default constructor
		Vector3D()
		{
			x = 0;
			y = 0;
			z = 0;
		}


		//default constructor
		Vector3D(float nX, float nY, float nZ)
		{
			x = nX;
			y = nY;
			z = nZ;
		}


		//calculate and return the magnitude of this vector
		float GetMagnitude()
		{
			return sqrtf(x * x + y * y + z * z);
		}

		float GetSquaredMagnitude()
		{
			return x * x + y * y + z * z;
		}
		//multiply this vector by a scalar
		Vector3D operator*(float num) const
		{
			return Vector3D(x * num, y * num, z * num);
		}

		//pass in a vector, pass in a scalar, return the product
		friend Vector3D operator*(float num, Vector3D const &vec)
		{
			return Vector3D(vec.x * num, vec.y * num, vec.z * num);
		}

		//add two vectors
		Vector3D operator+(const Vector3D &vec) const
		{
			return Vector3D(x + vec.x, y + vec.y, z + vec.z);
		}

		//subtract two vectors
		Vector3D operator-(const Vector3D &vec) const
		{
			return Vector3D(x - vec.x, y - vec.y, z - vec.z);
		}

		//normalize this vector
		Vector3D normalize()
		{
			Vector3D vec;
			float magnitude = sqrtf(x * x + y * y + z * z);
			vec.x = x / magnitude;
			vec.y = y /magnitude;
			vec.z = z / magnitude;
			return vec;
		}


	};
}
#endif
