#ifndef Rectangle_H
#define Rectangle_H

#include "Object2D.h"

class Rectangle : public Object2D {
	public:
		//constructors
		Rectangle();
		Rectangle(Point p1, Point p2, Point p3, Point p4, Color color);
		Rectangle(const Rectangle &s);

		//methods
		virtual void render();
		Rectangle* copy();

		//destructor
		~Rectangle();
};

#endif
