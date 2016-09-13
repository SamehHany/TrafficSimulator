#include "Circle.h"

Circle::Circle (Point center,GLfloat radius,int np,Color color) {
	pointsNum = np + 1; //np + center point

	points = new Point[pointsNum];
	colors = new Color[pointsNum];
	points[0] = center;
	colors[0] = color;
	for (int i=0;i<np;i++){
		double angle = 360.0/(np-1) * i;
		double rad = angle / 180.0 * M_PI;
		points[i+1] = center + Point(radius*cos(rad),radius*sin(rad));
		colors[i+1] = color;
	}
	this->init();
}

void Circle::render() {
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLE_FAN, 0, pointsNum);
}

Circle::~Circle() {
	delete[] points;
	delete[] colors;
}
