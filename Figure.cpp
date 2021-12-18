#include "Figure.hpp"

Circle::Circle(double r, double x, double y, int ID) {
	setType(CIRCLE);
	figureID_ = ID;
	radius_ = r;
	x_ = x;
	y_ = y;
}

const bool Circle::isDotInFigure(Point point) {
	if ((x_ - point.x) * (x_ - point.x) + (y_ - point.y) * (y_ - point.y) < radius_ * radius_) {
		return true;
	}
	return false;
}

Rectangle::Rectangle(double x1, double y1, double x3, double y3, int ID) {
	setType(RECTANGLE);
	figureID_ = ID;
	points.push_back({ x1, y1 });
	points.push_back({ x3, y1 });
	points.push_back({ x3, y3 });
	points.push_back({ x1, y3 });
}

Polygon::Polygon(std::vector<Point>& pts, int ID) {
	setType(POLYGON);
	figureID_ = ID;
	points = pts;
}

const bool Polygon::isDotInFigure(Point point) {
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

Reader::Reader(std::vector<Figure*>& figures) {
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

Reader::Reader(std::string& initstr) {
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

void Initializer::InitFigures(const std::string& initstring, std::vector<Figure*>& figures) {
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