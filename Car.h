#ifndef CAR_H
#define CAR_H

class Car;
#include "Road.h"

#define _CAR_X 10
#define _CAR_Y 5

class Car
{
	private:
		int id;
		GLfloat scale;
		time_t arr_time;
		GLfloat speed;
		GLfloat original_speed;
		char lane;
		Rectangle *body;
		Road *road;
		
		GLfloat x;
		GLfloat y;
		GLfloat angle;
		GLfloat cent_angle;
		
		GLfloat rot_matrix[9];
	public:
		bool is_crashing;
		Car(int id, Rectangle *body, GLfloat scale = 1.0f, time_t arr_time = 0, GLfloat speed = 100, char lane = 'r', Road *road = 0);
		void setRoad(Road *road);
		
		int display();
		int getID();
		GLfloat getX();
		GLfloat getY();
		GLfloat getScale();
		GLfloat getAngle();
		GLfloat getCentAngle();
		char getLane();
		time_t arrTime();
		void crash();
		void crash(Car *car);
};

void deleteCars(Car **cars, size_t size);

Car **readCars(const char *fname, int &size, Road *road1 = 0, Road *road2 = 0);

#endif // CAR_H
