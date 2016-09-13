#include "Rectangle.h"

Rectangle::Rectangle() {
	pointsNum = 4;
	points = new Point[pointsNum];
	colors = new Color[pointsNum];
	Point p(0, 0);
	Color color;
	points[0] = p;
	points[1] = p;
	points[2] = p;
	points[3] = p;
	colors[0] = color;
	colors[1] = color;
	colors[2] = color;
	colors[3] = color;
	this->init();
}

Rectangle::Rectangle(Point p1, Point p2, Point p3, Point p4, Color color) {
	pointsNum = 4;
	points = new Point[pointsNum];
	colors = new Color[pointsNum];
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
	points[3] = p4;
	colors[0] = color;
	colors[1] = color;
	colors[2] = color;
	colors[3] = color;
	this->init();
}

Rectangle::Rectangle(const Rectangle &s) {
	pointsNum = 4;
	points = new Point[pointsNum];
	colors = new Color[pointsNum];
	points[0] = s.points[0];
	points[1] = s.points[1];
	points[2] = s.points[2];
	points[3] = s.points[3];
	colors[0] = s.colors[0];
	colors[1] = s.colors[1];
	colors[2] = s.colors[2];
	colors[3] = s.colors[3];
	this->init();
}

void Rectangle::render() {
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, pointsNum);
}

Rectangle* Rectangle::copy()
{
	Rectangle *o = new Rectangle;
	o->pointsNum = pointsNum;
	o->points = new Point[pointsNum];
	o->colors = new Color[pointsNum];
	
	for (int i = 0; i < pointsNum; i++) {
		o->points[i] = points[i];
		o->colors[i] = colors[i];
	}
	
	return o;
}

Rectangle::~Rectangle() {
	delete[] points;
	delete[] colors;
}

