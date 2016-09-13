#include "HollowCircle.h"

HollowCircle::HollowCircle (Point center, GLfloat radius, GLfloat thickness, int np, GLfloat drawing_angle, Color color, GLfloat angle) {
	pointsNum = (np * 2) * (drawing_angle / (2 * M_PI)) + 2; //np + center point
	pointsNum = pointsNum % 2 == 0 ? pointsNum : pointsNum + 1;

	points = new Point[pointsNum];
	colors = new Color[pointsNum];
	for (int i=0;i<pointsNum;i+=2){
		double tmp_angle = 360.0/(np*2) * i;
		double rad = angle + tmp_angle / 180.0 * M_PI;
		points[i] = center + Point( radius*cos(rad),radius*sin(rad) );
		colors[i] = color;
		points[i+1] = center + Point( (radius + thickness) * cos(rad), (radius + thickness) * sin(rad) );
		colors[i+1] = color;
	}
	this->init();
}

void HollowCircle::render() {
	glBindVertexArray( vao );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, pointsNum);
}

HollowCircle::~HollowCircle() {
	delete[] points;
	delete[] colors;
}
