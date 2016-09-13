#include "Car.h"

Car::Car(int id, Rectangle *body, GLfloat scale, time_t arr_time, GLfloat speed, char lane, Road *road)
{
	this->id = id;
	this->body = body;//->copy();
	this->road = road;
	this->scale = scale;
	this->arr_time = arr_time;
	this->speed = speed;
	this->original_speed = speed;
	this->lane = lane;
	this->is_crashing = false;
	this->cent_angle = 0;
	
	rot_matrix[0] = cos(cent_angle); rot_matrix[1] = -sin(cent_angle); rot_matrix[2] = 0;
	rot_matrix[3] = sin(cent_angle); rot_matrix[4] = cos(cent_angle) ; rot_matrix[5] = 0;
	rot_matrix[6] = 0              ; rot_matrix[7] = 0               ; rot_matrix[8] = 1;
	
	if (road != 0) {
		Point p = road->start(lane, _CAR_X * scale);
		x = p.x;
		y = p.y;
	} else {
		x = 0;
		y = 0;
	}
	angle = 0;
}

void Car::setRoad(Road *road)
{
	this->road = road;
	Point p = road->start(lane, _CAR_X * scale);
	x = p.x;
	y = p.y;
}

int Car::display()
{
	time_t t = get_time();
	int _loc_action = _TRANSFORM | _SCALE | _ROTATE_CENTER;
	/*if (t > 20000) {
		cent_angle = 2 * M_PI * _TIME_FRAC * playspeed;
		cent_angle = cent_angle > 2 * M_PI? cent_angle - 2 * M_PI : cent_angle;
		Point *pointp; // Point Pointer
		for (int i = 0; i < body->pointsNum; i++) {
			pointp = &body->points[i];
			pointp->x = GLfloat( (double)pointp->x * cos((double)cent_angle) - (double)pointp->y * sin((double)cent_angle) );
			pointp->y = GLfloat( (double)pointp->x * sin((double)cent_angle) + (double)pointp->y * cos((double)cent_angle) );
		}
		glBindBuffer( GL_ARRAY_BUFFER, body->getBuffer() );
		glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Point) * body->pointsNum, body->points);
		//scale *= 1.2;
		if (speed > 0)
			speed -= original_speed * _TIME_FRAC * GLfloat(1)/1.5;
		if (speed < 0)
			speed = 0;
	}*/
	if (is_crashing) {
		cent_angle += 2 * M_PI * _TIME_FRAC * playspeed * GLfloat(1)/4;
		//cent_angle = cent_angle > 2 * M_PI? cent_angle - 2 * M_PI : cent_angle;
		
		rot_matrix[0] = cos(cent_angle); rot_matrix[1] = -sin(cent_angle);// rot_matrix[2] = -x * cos(cent_angle) + y * sin(cent_angle) - x;
		rot_matrix[3] = sin(cent_angle); rot_matrix[4] = cos(cent_angle) ;// rot_matrix[5] = -x * sin(cent_angle) + y * cos(cent_angle) - y;
		glUniformMatrix3fv(__rot_mat, 1, GL_TRUE, rot_matrix);
		_loc_action |= _ROT_MAT;
		if (speed > 0)
			speed -= original_speed * _TIME_FRAC * GLfloat(6)/4;
		if (speed < 0)
			speed = 0;
		//if (cent_angle >= 2 * M_PI)
		//	return -1;
	}
	if (t >= arr_time) {
		int a = road->move(this, lane, x, y, angle, speed, scale);
		if (a == -100)
			return -1;
		//if (a != 0) {
		//	x += a * playspeed * speed * _TIME_FRAC;
		//} else {
		if (a == 0) {
			//angle -= -playspeed * speed * _TIME_FRAC / radius;
			glUniform1f(__x_trans_cent, -x);
			glUniform1f(__y_trans_cent, 0);
			glUniform1f(__rot, angle);
		}
		glUniform1i(__action, _loc_action);
		glUniform1f(__scale, scale);
		glUniform1f(__x_trans, x);
		glUniform1f(__y_trans, y);
		body->render();
		glUniform1i(__action, _NO_ACTION);
		glUniform1f(__rot, 0.0f);
		glUniform1f(__x_trans_cent, 0.0f);
		glUniform1f(__y_trans_cent, 0.0f);
		return 1;
	}
	return 0;
}

int Car::getID() {
	return id;
}

GLfloat Car::getScale() {
	return scale;
}

void Car::crash() {
	if (!is_crashing)
		is_crashing = true;
}

#include <stdio.h>

void Car::crash(Car *car) {
	if (!is_crashing && !car->is_crashing) {
		car->crash();
		crash();
		printf("Accident between car %d and car %d at %lu ms\n", id, car->getID(), get_time());
	}
}

GLfloat Car::getX() {
	return x;
}

GLfloat Car::getY() {
	return y;
}

GLfloat Car::getAngle() {
	return angle;
}

GLfloat Car::getCentAngle() {
	return cent_angle;
}

char Car::getLane() {
	return lane;
}

time_t Car::arrTime() {
	return arr_time;
}

void deleteCars(Car **cars, size_t size)
{
	for (size_t i = 0; i < size; i++)
		delete cars[i];
	delete [] cars;
}

#include <string>
#include <fstream>

using namespace std;

Color randColor();

Car **readCars(const char *fname, int &size, Road *road1, Road *road2)
{
	ifstream infs (fname);
	/*infs.seekg (0, infs.end);
	int text_len = infs.tellg();
	infs.seekg (0, infs.beg);*/
	
	GLfloat scale;
	time_t arr_time;
	GLfloat speed;
	char lane;
	
	Point a(-_CAR_X, _CAR_Y),  b(_CAR_X, _CAR_Y),
	      c(-_CAR_X, -_CAR_Y), d(_CAR_X, -_CAR_Y);
	Color color;
	Rectangle *body;
	
	int tmp_size = 128;
	int index = 0;
	string in;
	Car **cars = new Car* [tmp_size];
	Car **tmp_cars;
	while ( true ) {
		if ( !(infs >> in) )
			break;
		scale = (float)atof(in.c_str());
		if ( !(infs >> in) )
			break;
		arr_time = atoi(in.c_str());
		if ( !(infs >> in) )
			break;
		speed = (float)atof(in.c_str());
		if ( !(infs >> in) )
			break;
		lane = in.c_str()[0];
		
		body = new Rectangle(a, b, c, d, randColor());
		int decider;
		if (road2 != 0)
			decider = rand() % 2;
		else decider = 0;;
		if (road2 == 0)
			decider = 0;
		if (decider == 0)
			cars[index] = new Car(index, body, scale, arr_time, speed, lane, road1);
		else cars[index] = new Car(index, body, scale, arr_time, speed, lane, road2);
		index++;
		//delete body;
		
		if (index >= tmp_size) {
			tmp_cars = cars;
			tmp_size *= 2;
			cars = new Car* [tmp_size];
			for (int i = 0; i < index; i++)
				cars[i] = tmp_cars[i];
			delete [] tmp_cars;
		}
	}
	
	infs.close();
	
	tmp_cars = cars;
	tmp_size = index;
	cars = new Car* [tmp_size];
	for (int i = 0; i < index; i++)
		cars[i] = tmp_cars[i];
	delete [] tmp_cars;
	
	return cars;
}

Color randColor()
{
	GLfloat red, green, blue;
	red = GLfloat((rand() % 11))/10;
	green = GLfloat((rand() % 11))/10;
	blue = GLfloat((rand() % 11))/10;
	
	return Color(red, green, blue);
}
