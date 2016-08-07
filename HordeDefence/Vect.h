#pragma once
#include <cmath>

#ifndef VECT_CLASS
#define VECT_CLASS

class Vect {
private:
	double x;
	double y;
	double z;

public:
	Vect();
	Vect(double, double, double);

	const double& getX() { return x; }
	const double& getY() { return y; }
	const double& getZ() { return z; }


	void setX(const double& nX) { x = nX; }
	void setY(const double& nY) { y = nY; }
	void setZ(const double& nZ) { z = nZ; }

	double length() {
		return sqrt((x * x) + (y * y) + (z * z));
	}

	Vect normalized() {
		double mag = length();

		return Vect(x / mag, y / mag, z / mag);
	}

	Vect neg() {
		return Vect(-x, -y, -z);
	}

	double dot(Vect other) {
		return x * other.getX() + y * other.getY() + z * other.getZ();
	}

	Vect cross(Vect other) {
		double x_ = y * other.getZ() - z * other.getY();
		double y_ = z * other.getX() - x * other.getZ();
		double z_ = x * other.getY() - y * other.getX();

		return Vect(x_, y_, z_);
	}

	inline 	Vect operator + (const Vect &other) {
		return Vect(x + other.x, y + other.y, z + other.z);
	}

	inline 	Vect operator - (const Vect &other) {
		return Vect(x - other.x, y - other.y, z - other.z);
	}

	inline 	Vect operator * (const Vect &other) {
		return Vect(x * other.x, y * other.y, z * other.z);
	}

	inline 	Vect operator / (const Vect &other) {
		return Vect(x / other.x, y / other.y, z / other.z);
	}
};

#endif VECT_CLASS


