#include "GJK_Algorithm.hpp"
#include "Point.cpp"

Point CollisionDetector::tripleProduct(Point a, Point b, Point c) {

	Point r;

	double ac = a.x * c.x + a.y * c.y;
	double bc = b.x * c.x + b.y * c.y;

	r.x = b.x * ac - a.x * bc;
	r.y = b.y * ac - a.y * bc;
	return r;
}

Point CollisionDetector::averagePoint(const std::vector<Point> figure_points) {
	Point avg = { 0, 0 };
	for (auto i = figure_points.begin(); i != figure_points.end(); ++i) {
		avg.x += i->x;
		avg.y += i->y;
	}

	avg.x /= size(figure_points);
	avg.y /= size(figure_points);
	return avg;
}

size_t CollisionDetector::indexOfFurthestPoint(const std::vector<Point> figure_points, Point d) {

	double maxProduct = dotProduct(d, *figure_points.begin());
	size_t index = 0;
	int i = 0;
	for (auto it = std::next(figure_points.begin()); it < figure_points.end(); ++it) {
		i++;
		double product = dotProduct(d, *it);
		if (product > maxProduct) {
			maxProduct = product;
			index = i;
		}
	}
	return index;
}

Point CollisionDetector::support(const std::vector<Point> figure_points1,
	const std::vector<Point> figure_points2, Point d) {

	size_t i = indexOfFurthestPoint(figure_points1, d);
	size_t j = indexOfFurthestPoint(figure_points2, negate(d));

	return subtract(figure_points1.at(i), figure_points2.at(j));
}

double CollisionDetector::Perturbation()
{
	return ((double)rand() / (double)RAND_MAX) * DBL_EPSILON * 100.0f * ((rand() % 2) ? 1.0f : -1.0f);
}

std::vector<Point> CollisionDetector::Jostle(std::vector<Point> figure_points)
{
	std::vector<Point> result;
	Point b;

	for (auto i = figure_points.begin(); i != figure_points.end(); ++i)
		result.push_back({ i->x + Perturbation(), i->y + Perturbation() });
	return result;
}

bool CollisionDetector::GilbertJohnsonKeerthi(const std::vector<Point> figure1, const std::vector<Point> figure2) {

	int itercount = 0;
	size_t index = 0;
	Point a, b, c, d, ao, ab, ac, abperp, acperp, simplex[3];

	Point position1 = averagePoint(figure1);
	Point position2 = averagePoint(figure2);

	d = subtract(position1, position2);

	if ((d.x == 0) && (d.y == 0))
		d.x = 1.f;

	a = simplex[0] = support(figure1, figure2, d);

	if (dotProduct(a, d) <= 0)
		return 0;

	d = negate(a);

	while (1) {
		itercount++;

		a = simplex[++index] = support(figure1, figure2, d);

		if (dotProduct(a, d) <= 0)
			return 0;

		ao = negate(a);

		if (index < 2) {
			b = simplex[0];
			ab = subtract(b, a);
			d = tripleProduct(ab, ao, ab);
			if (lengthSquared(d) == 0)
				d = perpendicular(ab);
			continue;
		}

		b = simplex[1];
		c = simplex[0];
		ab = subtract(b, a);
		ac = subtract(c, a);

		acperp = tripleProduct(ab, ac, ac);

		if (dotProduct(acperp, ao) >= 0) {

			d = acperp;

		}
		else {

			abperp = tripleProduct(ac, ab, ab);

			if (dotProduct(abperp, ao) < 0)
				return 1;

			simplex[0] = simplex[1];

			d = abperp;
		}

		simplex[1] = simplex[2];
		--index;
	}

	return 0;
}

bool CollisionDetector::CirclesCollisions(double r1, double r2, Point center1, Point center2) {

	double distance = sqrt((center2.x - center1.x) * (center2.x - center1.x) +
		(center2.y - center1.y) * (center2.y - center1.y));

	if (distance <= (r1 + r2)) return 1;
	else return 0;
}

bool CollisionDetector::Circle_PolygonCollisions(Circle& circle, Figure& polygon)
{

	std::vector<Point> test = polygon.getPoints();

	for (auto i = test.begin(); i != test.end(); ++i)
	{
		if (circle.isDotInFigure(*i)) return 1;
	}

	Point center = circle.getCenter();

	if (polygon.isDotInFigure(center)) return 1;
	
	return 0;

}

CollisionDetector::CollisionDetector(std::vector<Figure*>& figures) {

	bool collisionDetected;

	for (int k = 0; k < size(figures); ++k) {
		for (int i = 0; i < size(figures); ++i) {
			if (k != i) {
				if ((figures[k]->getType() == RECTANGLE || figures[k]->getType() == POLYGON) &&
					(figures[i]->getType() == RECTANGLE || figures[i]->getType() == POLYGON)) {
					collisionDetected = GilbertJohnsonKeerthi(Jostle(figures[k]->getPoints()), Jostle(figures[i]->getPoints()));
				}

				if ((figures[k]->getType() == RECTANGLE || figures[k]->getType() == POLYGON) &&
					(figures[i]->getType() == CIRCLE)) {

					collisionDetected = Circle_PolygonCollisions(static_cast<Circle&>(*figures[i]), *figures[k]);

				}

				if ((figures[k]->getType() == CIRCLE) && (figures[i]->getType() == RECTANGLE || figures[i]->getType() == POLYGON)) {

					collisionDetected = Circle_PolygonCollisions(static_cast<Circle&>(*figures[k]), *figures[i]);
				}

				if ((figures[k]->getType() == CIRCLE) && (figures[i]->getType() == CIRCLE)) {
					collisionDetected = CirclesCollisions(figures[k]->getRadius(), figures[i]->getRadius(),
						figures[k]->getCenter(), figures[i]->getCenter());
				}

				if (collisionDetected)
				{

					if (figures[i]->getFigureID() > figures[k]->getFigureID()) {
						collided_figures.insert({ figures[k]->getFigureID(), figures[i]->getFigureID() });
					}

					if (figures[i]->getFigureID() < figures[k]->getFigureID()) {
						collided_figures.insert({ figures[i]->getFigureID(), figures[k]->getFigureID() });
					}
				}
			}
		}
	}
}

void CollisionDetector::showCollisions() {
	for (auto i = collided_figures.begin(); i != collided_figures.end(); ++i)
		std::cout << i->first << " " << i->second << std::endl;
}
