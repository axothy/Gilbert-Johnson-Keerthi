#pragma once
#include "Figure.hpp"
#include <set>

class CollisionDetector {
public:
	CollisionDetector();
	CollisionDetector(std::vector<Figure*>& figures);

	std::set<int, int> collided_figures;

	void showCollisions();

	std::vector<Point> Jostle(std::vector<Point> figure_points);
	bool GilbertJohnsonKeerthi(const std::vector<Point> figure1, const std::vector<Point> figure2);
	bool CirclesCollisions(double r1, double r2, Point center1, Point center2);
	bool Circle_PolygonCollisions(Circle& circle, Figure& polygon);

private:
	Point tripleProduct(Point a, Point b, Point c);
	Point averagePoint(const std::vector<Point> figure_points);
	size_t indexOfFurthestPoint(const std::vector<Point> figure_points, Point d);
	Point support(const std::vector<Point> figure_points1, const std::vector<Point> figure_points2, Point d);
	double Perturbation();

};
