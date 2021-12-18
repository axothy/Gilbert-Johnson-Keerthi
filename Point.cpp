#include "Point.hpp"

inline Point2D subtract(Point2D a, Point2D b) { 
	a.x -= b.x; a.y -= b.y; return a;
}

inline Point2D negate(Point2D v) { 
	v.x = -v.x; v.y = -v.y; return v; 
}

inline Point2D perpendicular(Point2D v) 
{    Point2D p = { v.y, -v.x }; return p; 
}

inline double dotProduct(Point2D a, Point2D b) {
	return a.x * b.x + a.y * b.y; 
}

inline double lengthSquared(Point2D v) {
	return v.x * v.x + v.y * v.y; 
}