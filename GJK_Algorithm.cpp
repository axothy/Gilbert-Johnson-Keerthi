#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

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

inline Point2D subtract(Point2D a, Point2D b) { a.x -= b.x; a.y -= b.y; return a; }
inline Point2D negate(Point2D v) { v.x = -v.x; v.y = -v.y; return v; }
inline Point2D perpendicular(Point2D v) { Point2D p = { v.y, -v.x }; return p; }
inline double dotProduct(Point2D a, Point2D b) { return a.x * b.x + a.y * b.y; }
inline double lengthSquared(Point2D v) { return v.x * v.x + v.y * v.y; }


using Point = Point2D;

class Figure {
public:
	FigureType type_ = EXCEPTION_TYPE;
	int figureID_;

	virtual void getData() = 0;

	virtual void setType(FigureType type) = 0;
	virtual FigureType getType() = 0;

	virtual std::vector<Point> getPoints() = 0;
	virtual double getRadius() = 0;
	virtual Point getCenter() = 0;
	virtual int getFigureID() = 0;

	virtual const bool isDotInFigure(Point point) = 0;
};

class Circle : public Figure {
	double radius_;
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
	Point getCenter() override { return { x_, y_ }; }

	const bool isDotInFigure(Point point) override {
		if ((x_ - point.x) * (x_ - point.x) + (y_ - point.y) * (y_ - point.y) < radius_ * radius_) {
			return true;
		}
		return false;
	}

	double x_, y_;
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

	inline std::vector<Point> getPoints() override { return points; }
	double getRadius() override { return -1; }
	Point getCenter() { return {}; }

	inline void setType(FigureType type) override { type_ = type; }
	inline FigureType getType() override { return type_; }
	inline int getFigureID() override { return figureID_; }

	const bool isDotInFigure(Point point) override { return -1; }
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

	inline std::vector<Point> getPoints() override { return points; }

	double getRadius() override { return -1; }
	Point getCenter() { return {}; }

	inline void setType(FigureType type) override { type_ = type; }
	inline FigureType getType() override { return type_; }
	inline int getFigureID() override { return figureID_; }

	const bool isDotInFigure(Point point) {
		bool result = false;
		int j = points.size() - 1;
		for (int i = 0; i < points.size(); i++) {
			if ((points[i].y < point.y && points[j].y >= point.y || points[j].y < point.y && points[i].y >= point.y) &&
				(points[i].x + (point.y - points[i].y) / (points[j].y - points[i].y) * (points[j].x - points[i].x) < point.x))
				result = !result;
			j = i;
		}
		return result;
	}
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

	std::map<int, int> collided_figures;

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

int main()
{
	std::vector<Figure*> figures;
	Reader readfile(figures);

	CollisionDetector collisions(figures);
	collisions.showCollisions();

	return 0;
}
