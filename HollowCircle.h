#ifndef HOLLOWCIRCLE_H
#define HOLLOWCIRCLE_H

#include "Object2D.h"

class HollowCircle : public Object2D
{
	public:

		//constructors
		HollowCircle (Point center, GLfloat radius, GLfloat thickness, int np, GLfloat drawing_angle, Color color, GLfloat angle = 0);

		//methods
		virtual void render();

		//constructor
		~HollowCircle();
};

#endif // HOLLOWCIRCLE_H
