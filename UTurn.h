#ifndef UTURN_H
#define UTURN_H

#include "Road.h"

#define no_pix_node 10

class UTurn: public Road
{
	private:
		GLfloat width;
		GLfloat len1;
		GLfloat len2;
		GLfloat radius;
		bool crash(Car* car1, Car* car2);
		bool check_crashes(CarList list);
		void check_crashes();
		
		GLfloat get_dist(GLfloat x, GLfloat y, GLfloat angle, GLfloat radius);
		int get_index(GLfloat dist);
	public:
		UTurn(GLfloat len, GLfloat radius, GLfloat width, int np, GLfloat x = 0.0f, GLfloat y = 0.0f, GLfloat scale = 1.0f, GLfloat angle = 0.0f, int direction = -1);
		Point start(char lane, GLfloat offset);
		int move(Car *car, char lane, GLfloat &x, GLfloat &y, GLfloat &angle, GLfloat speed, GLfloat scale);
};

#endif  // UTURN_H
