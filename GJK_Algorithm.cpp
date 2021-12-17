#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

enum FigureType {
	RECTANGLE, POLYGON, CIRCLE, EXCEPTION_TYPE
};

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

	bool operator!=(const Point2D xy0) {
		return !((*this) == xy0);
	}
};

using Point = Point2D;

class Figure {
public:
	FigureType type_ = EXCEPTION_TYPE;
	int figureID;

	virtual void getData() = 0;

	virtual void setType(FigureType type) = 0;
	virtual FigureType getType() = 0;

	virtual std::vector<Point> getPoints() = 0;
	virtual double getRadius() = 0;
	virtual double getXCenter() = 0;
	virtual double getYCenter() = 0;
	virtual int getFigureID() = 0;
};

class Circle : public Figure {
	double radius_, x_, y_;
	FigureType type_;
	int figureID_;
public:
	Circle(double r, double x, double y, int ID) {
		setType(CIRCLE);
		figureID_ = ID;
		radius_ = r;
		x_ = x;
		y_ = y;
	}

	void getData() { std::cout << "CIRCLE" << std::endl; }

	void setType(FigureType type) override { type_ = type; }
	FigureType getType() override { return type_; }
	int getFigureID() override { return figureID_; }

	std::vector<Point> getPoints() { return {}; }
	double getRadius() override { return radius_; }
	double getXCenter() override { return x_; }
	double getYCenter() override { return y_; }

};

class Rectangle : public Figure {
	std::vector<Point> points;
	FigureType type_;
	int figureID_;

public:
	Rectangle(double x1, double y1, double x3, double y3, int ID) {
		setType(RECTANGLE);
		figureID_ = ID;
		points.push_back({ x1, y1 });
		points.push_back({ x3, y1 });
		points.push_back({ x3, y3 });
		points.push_back({ x1, y3 });
	}

	void getData() { std::cout << "RECT" << std::endl; }

	std::vector<Point> getPoints() override { return points; }
	double getRadius() override { return -1; }
	double getXCenter() override { return -1; }
	double getYCenter() override { return -1; }

	void setType(FigureType type) override { type_ = type; }
	FigureType getType() override { return type_; }
	int getFigureID() override { return figureID_; }
};

class Polygon : public Figure {
	std::vector<Point> points;
	FigureType type_;
	int figureID_;

public:
	Polygon(std::vector<Point>& pts, int ID) {
		setType(POLYGON);
		figureID_ = ID;
		points = pts;
	}
	void getData() { std::cout << "POLY" << std::endl; }

	std::vector<Point> getPoints() override { return points; }

	double getRadius() override { return -1; }
	double getXCenter() override { return -1; }
	double getYCenter() override { return -1; }

	void setType(FigureType type) override { type_ = type; }
	FigureType getType() override { return type_; }
	int getFigureID() override { return figureID_; }
};

class Reader {
public:
	Reader(std::vector<Figure*>& figures) {
		std::ifstream input("input.txt");

		if (input.is_open() == false || input.eof()) {
			exit(0);
		}

		std::string inputstr;
		int ID = 0;

		input >> inputstr;

		while (!input.eof()) {
			if (inputstr == "RECTANGLE") {
				double x1, y1, x3, y3;
				input >> x1 >> y1 >> x3 >> y3;
				figures.push_back(new Rectangle(x1, y1, x3, y3, ID));
				input >> inputstr;
				ID++;
			}
			else if (inputstr == "CIRCLE") {
				double r, x, y;
				input >> r >> x >> y;
				figures.push_back(new Circle(r, x, y, ID));
				input >> inputstr;
				ID++;
			}
			else if (inputstr == "POLYGON") {
				std::vector<Point> points;
				Point point;
				std::string checkStr;

				while (!input.eof()) {
					input >> checkStr;
					if (checkStr == "CIRCLE" || checkStr == "RECTANGLE") {
						break;
					}

					std::stringstream ss(checkStr);
					ss >> point.x;
					input >> point.y;
					points.push_back(point);
				}
				figures.push_back(new Polygon(points, ID));
				inputstr = checkStr;
				ID++;

			}


		}
	}

	Reader(std::string& initstr) {
		std::ifstream input("input.txt");

		if (input.is_open() == false) {
			exit(0);
		}

		std::string inputstr;

		while (!input.eof()) {
			input >> inputstr;
			initstr = inputstr + ' ';
			if (inputstr == "RECTANGLE") {
				for (int i = 0; i < 4; i++) {
					input >> inputstr;
					initstr += inputstr + ' ';
				}
			}
			else if (inputstr == "CIRCLE") {
				for (int i = 0; i < 3; i++) {
					input >> inputstr;
					initstr += inputstr + ' ';
				}
			}
			else if (inputstr == "POLYGON") {

			}
		}

	}
};

class Initializer {
public:
	void InitFigures(const std::string& initstring, std::vector<Figure*>& figures) {
		std::stringstream datastream(initstring);
		std::string datastring;
		int ID = 0;

		datastream >> datastring;
		if (datastring == "RECTANGLE") {
			double x1, y1, x3, y3;
			datastream >> x1 >> y1 >> x3 >> y3;
			figures.push_back(new Rectangle(x1, y1, x3, y3, ID));
			ID++;
		}
		else if (datastring == "CIRCLE") {
			double r, x, y;
			datastream >> r >> x >> y;
			figures.push_back(new Circle(r, x, y, ID));
			ID++;

		}
		else if (datastring == "POLYGON") {

		}
	}
};

class CollisionDetector {
public:
	CollisionDetector();
	CollisionDetector(std::vector<Figure*>& figures);

	std::vector<std::pair<int, int>> collided_figures;

	void showCollisions();

	std::vector<Point> Jostle(std::vector<Point> figure_points);
	int gjk(const std::vector<Point> figure1, const std::vector<Point> figure2);

private:

	//Basic vector arithmetic operations
	Point subtract(Point a, Point b) { a.x -= b.x; a.y -= b.y; return a; }
	Point negate(Point v) { v.x = -v.x; v.y = -v.y; return v; }
	Point perpendicular(Point v) { Point p = { v.y, -v.x }; return p; }
	double dotProduct(Point a, Point b) { return a.x * b.x + a.y * b.y; }
    double lengthSquared(Point v) { return v.x * v.x + v.y * v.y; }
	Point tripleProduct(Point a, Point b, Point c);
	Point averagePoint(const std::vector<Point> figure_points);
	size_t indexOfFurthestPoint(const std::vector<Point> figure_points, Point d);
	Point support(const std::vector<Point> figure_points1,
		const std::vector<Point> figure_points2, Point d);
	double Perturbation();

};


// Triple product expansion is used to calculate perpendicular normal vectors
// which kinda 'prefer' pointing towards the Origin in Minkowski space

Point CollisionDetector::tripleProduct(Point a, Point b, Point c) {

	Point r;

	float ac = a.x * c.x + a.y * c.y; // perform a.dot(c)
	float bc = b.x * c.x + b.y * c.y; // perform b.dot(c)

	// perform b * a.dot(c) - a * b.dot(c)
	r.x = b.x * ac - a.x * bc;
	r.y = b.y * ac - a.y * bc;
	return r;
}

// This is to compute average center (roughly). It might be different from
// Center of Gravity, especially for bodies with nonuniform density,
// but this is ok as initial direction of simplex search in GJK.
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


// Get furthest vertex along a certain direction
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

// Minkowski sum support function for GJK
Point CollisionDetector::support(const std::vector<Point> figure_points1,
	const std::vector<Point> figure_points2, Point d) {

	// get furthest point of first body along an arbitrary direction
	size_t i = indexOfFurthestPoint(figure_points1, d);

	// get furthest point of second body along the opposite direction
	size_t j = indexOfFurthestPoint(figure_points2, negate(d));

	// subtract (Minkowski sum) the two points to see if bodies 'overlap'
	return subtract(figure_points1.at(i), figure_points2.at(j));
}

double CollisionDetector::Perturbation()
{
	return ((double)rand() / (double)RAND_MAX) * FLT_EPSILON * 100.0f * ((rand() % 2) ? 1.0f : -1.0f);
}

std::vector<Point> CollisionDetector::Jostle(std::vector<Point> figure_points)
{
	std::vector<Point> result;
	Point b;

	for (auto i = figure_points.begin(); i != figure_points.end(); ++i)
		result.push_back({ i->x + Perturbation(), i->y + Perturbation() });
	return result;
}

int CollisionDetector::gjk(const std::vector<Point> figure1, const std::vector<Point> figure2) {

	int itercount = 0;
	size_t index = 0; // index of current vertex of simplex
	Point a, b, c, d, ao, ab, ac, abperp, acperp, simplex[3];

	Point position1 = averagePoint(figure1); // not a CoG but
	Point position2 = averagePoint(figure2); // it's ok for GJK )

	// initial direction from the center of 1st body to the center of 2nd body
	d = subtract(position1, position2);

	// if initial direction is zero � set it to any arbitrary axis (we choose X)
	if ((d.x == 0) && (d.y == 0))
		d.x = 1.f;

	// set the first support as initial point of the new simplex
	a = simplex[0] = support(figure1, figure2, d);

	if (dotProduct(a, d) <= 0)
		return 0; // no collision

	d = negate(a); // The next search direction is always towards the origin, so the next search direction is negate(a)

	while (1) {
		itercount++;

		a = simplex[++index] = support(figure1, figure2, d);

		if (dotProduct(a, d) <= 0)
			return 0; // no collision

		ao = negate(a); // from point A to Origin is just negative A

		// simplex has 2 points (a line segment, not a triangle yet)
		if (index < 2) {
			b = simplex[0];
			ab = subtract(b, a); // from point A to B
			d = tripleProduct(ab, ao, ab); // normal to AB towards Origin
			if (lengthSquared(d) == 0)
				d = perpendicular(ab);
			continue; // skip to next iteration
		}

		b = simplex[1];
		c = simplex[0];
		ab = subtract(b, a); // from point A to B
		ac = subtract(c, a); // from point A to C

		acperp = tripleProduct(ab, ac, ac);

		if (dotProduct(acperp, ao) >= 0) {

			d = acperp; // new direction is normal to AC towards Origin

		}
		else {

			abperp = tripleProduct(ac, ab, ab);

			if (dotProduct(abperp, ao) < 0)
				return 1; // collision

			simplex[0] = simplex[1]; // swap first element (point C)

			d = abperp; // new direction is normal to AB towards Origin
		}

		simplex[1] = simplex[2]; // swap element in the middle (point B)
		--index;
	}

	return 0;
}

CollisionDetector::CollisionDetector(std::vector<Figure*>& figures) {

	for (int k = 0; k < size(figures); ++k) {
		for (int i = 0; i < size(figures); ++i) {
			if (k != i) {
				int collisionDetected = gjk(Jostle(figures[k]->getPoints()), Jostle(figures[i]->getPoints()));
				if (collisionDetected)
				{
					collided_figures.push_back({ figures[k]->getFigureID(), figures[i]->getFigureID() });
				}
			}
		}
	}

}

void CollisionDetector::showCollisions() {
	for (auto i = collided_figures.begin(); i != collided_figures.end(); ++i)
		std::cout << i->first << " " << i->second << std::endl;
}

int main()
{
	std::vector<Figure*> figures;
	Reader readfile(figures);

	CollisionDetector collisions(figures);
	collisions.showCollisions();

	return 0;
}