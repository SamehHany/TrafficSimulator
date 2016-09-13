#ifndef CIRCLE_H
#define CIRCLE_H

#include "Object2D.h"

class Circle : public Object2D {
	public:

		//constructors
		Circle (Point center,GLfloat radius,int np,Color color);

		//methods
		virtual void render();

		//constructor
		~Circle();
};

#endif // CIRCLE_H
