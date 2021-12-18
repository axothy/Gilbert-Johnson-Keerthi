#pragma once
#include "Point.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

enum FigureType {
	RECTANGLE, POLYGON, CIRCLE, EXCEPTION_TYPE
};


using Point = Point2D;

class Figure {
public:
	FigureType type_ = EXCEPTION_TYPE;
	int figureID_;

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
	Circle(double r, double x, double y, int ID);

	void setType(FigureType type) override { type_ = type; }
	FigureType getType() override { return type_; }
	int getFigureID() override { return figureID_; }

	std::vector<Point> getPoints() { return {}; }
	double getRadius() override { return radius_; }
	Point getCenter() override { return { x_, y_ }; }

	const bool isDotInFigure(Point point) override;

	double x_, y_;
};

class Rectangle : public Figure {
	std::vector<Point> points;
	FigureType type_;
	int figureID_;

public:
	Rectangle(double x1, double y1, double x3, double y3, int ID);

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
	Polygon(std::vector<Point>& pts, int ID);

	inline std::vector<Point> getPoints() override { return points; }

	double getRadius() override { return -1; }
	Point getCenter() { return {}; }

	inline void setType(FigureType type) override { type_ = type; }
	inline FigureType getType() override { return type_; }
	inline int getFigureID() override { return figureID_; }

	const bool isDotInFigure(Point point) override;
};

class Reader {
public:
	Reader(std::vector<Figure*>& figures);
	Reader(std::string& initstr);
};

class Initializer {
public:
	void InitFigures(const std::string& initstring, std::vector<Figure*>& figures);
};