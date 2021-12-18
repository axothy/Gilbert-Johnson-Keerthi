#pragma once

struct Point2D
{
	double x;
	double y;

	double getX() { return x; }
	double getY() { return y; }

	void setX(double x0) { x = x0; }
	void setY(double y0) { y = y0; }

	bool operator==(const Point2D xy0) {
		if ((*this).x == xy0.x && (*this).y == xy0.y) {
			return true;
		}
		else
			return false;
	}

	bool operator!=(const Point2D xy0);
};

inline Point2D subtract(Point2D a, Point2D b);
inline Point2D negate(Point2D v);
inline Point2D perpendicular(Point2D v);
inline double dotProduct(Point2D a, Point2D b);
inline double lengthSquared(Point2D v);