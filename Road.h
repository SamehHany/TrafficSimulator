#ifndef ROAD_H
#define ROAD_H

class Road;

#include "Object2D.h"
#include "CollisionGrid.h"

class Road
{
	protected:
		GLfloat x;
		GLfloat y;
		GLfloat angle;
		GLfloat scale;
		int no_objects;
		Object2D **objects;
		int flags;
		int num_accidents;
		CollisionGrid *c_grid;
	public:
		int direction;
		Road(int no_objects = 1);
		Road(int no_objects, Object2D **objects);
		
		int getNumAcc();
		
		virtual void display();
		//virtual Point displace(Point p) = 0;
		virtual Point start(char lane, GLfloat offset) = 0;
		
		virtual ~Road();
		virtual int move(Car *car, char lane, GLfloat &x, GLfloat &y, GLfloat &angle, GLfloat speed, GLfloat scale) = 0;
};

#include "Car.h"
#include "UTurn.h"

#endif // ROAD_H
