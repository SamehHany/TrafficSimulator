#include "UTurn.h"

UTurn::UTurn(GLfloat len, GLfloat radius, GLfloat width, int np, GLfloat x, GLfloat y, GLfloat scale, GLfloat angle, int direction)
{
	this->x = x;
	this->y = y;
	this->angle = angle;
	this->scale = scale;
	this->radius = radius;
	this->width = width;
	this->direction = direction;
	len1 = len;
	len2 = len;
	num_accidents = 0;
	GLfloat _radius = this->radius + width;
	c_grid = new CollisionGrid(2, ceil( (len1 + len2 + M_PI * _radius) / no_pix_node ) );
	
	flags = _NO_ACTION;
	
	if ( x != 0.0f || y != 0.0f)
		flags |= _TRANSFORM;
	if (scale != 1.0f)
		flags |= _SCALE;
	if (angle != 0.0f)
		flags |= _ROTATE_CENTER;
	
	GLfloat dash_len = 13, dash_width = 4.6, dash_sep = 11.5;
	GLfloat dash_and_sep = dash_len + dash_sep;
	
	int no_dashes1 = len / dash_and_sep + 1;
	int no_dashes2 = M_PI * (radius + (width + dash_width)/2) / dash_and_sep;
	int no_dashes3 = len / dash_and_sep + 1;
	
	no_objects = 3 + no_dashes1 + no_dashes2 + no_dashes3;
	objects = new Object2D* [no_objects];
	Point a = Point(-len/2, radius),	     b = Point(len/2, radius),
	      c = Point(-len/2, radius + width), d = Point(len/2, radius + width);
	objects[0] = new Rectangle(a, b, c, d, gray);
	a = Point(-len/2, -radius),	       b = Point(len/2, -radius),
	c = Point(-len/2, -radius - width), d = Point(len/2, -radius - width);
	objects[1] = new Rectangle(a, b, c, d, gray);
	
	a = Point(-len/2, 0);
	objects[2] = new HollowCircle(a, radius, width, np, M_PI, gray, 0.5 * M_PI);
	
	int start = 3;
	int i = start-1;
	while ( ++i < start + no_dashes1 ) {
		a = Point(len/2 - dash_and_sep * (i-start), radius + (width - dash_width)/2), b = Point(len/2 - dash_and_sep * (i-start) + dash_len, radius + (width - dash_width)/2),
		c = Point(len/2 - dash_and_sep * (i-start), radius + (width + dash_width)/2), d = Point(len/2 - dash_and_sep * (i-start) + dash_len, radius + (width + dash_width)/2);
		objects[i] = new Rectangle(a, b, c, d, white);
	}
	
	start = i;
	i--;
	GLfloat dash_radius = radius + (width - dash_width)/2;
	GLfloat dash_radius2 = radius + (width + dash_width)/2;
	GLfloat dash_angle = dash_len / dash_radius2;
	GLfloat dash_start_angle = 0.5 * M_PI + dash_sep / dash_radius2;
	a = Point(-len/2, 0);
	while ( ++i < start + no_dashes2 ) {
		objects[i] = new HollowCircle (a, dash_radius, dash_width, 500, dash_angle, white, dash_start_angle);
		dash_start_angle += dash_and_sep / dash_radius2;
	}
	
	start = i;
	i--;
	while ( ++i < start + no_dashes3 ) {
		a = Point(len/2 - dash_and_sep * (i-start), -radius - (width - dash_width)/2), b = Point(len/2 - dash_and_sep * (i-start) + dash_len, -radius - (width - dash_width)/2),
		c = Point(len/2 - dash_and_sep * (i-start), -radius - (width + dash_width)/2), d = Point(len/2 - dash_and_sep * (i-start) + dash_len, -radius - (width + dash_width)/2);
		objects[i] = new Rectangle(a, b, c, d, white);
	}
}

Point UTurn::start(char lane, GLfloat offset)
{
	if (lane == 'r')
		return Point(450 + offset, (this->radius + (0.5 - direction * 0.25) * width) * -direction);
	else if (lane == 'l')
		return Point(450 + offset, (this->radius + (0.5 + direction * 0.25) * width) * -direction);
	else return Point(0, 0);
}

int UTurn::move(Car *car, char lane, GLfloat &x, GLfloat &y, GLfloat &angle, GLfloat speed, GLfloat scale)
{
	GLfloat _x = x - this->x;
	GLfloat _y = y - this->y;
	GLfloat _radius;
	GLfloat tmp;
	
	if (lane == 'r')
		_radius = this->radius + (0.5 - direction * 0.25) * width;
	else if (lane == 'l')
		_radius = this->radius + (0.5 + direction * 0.25) * width;
	
	GLfloat dist = get_dist(x, y, angle, _radius);
	int index1 = get_index(dist - _CAR_X * scale);
	int index2 = get_index(dist + _CAR_X * scale);
	GLfloat ent_dist = len1 + len2 + M_PI * _radius; // entire distance
	
	if (index1 < 0)
		index1 = 0;
	else if (index1 > ent_dist)
		index1 = ent_dist;
	if (index2 < 0)
		index2 = 0;
	else if (index2 > ent_dist)
		index2 = ent_dist;
	
	int l_index;
	
	if (lane == 'r')
		l_index = 0;
	else if (lane == 'l')
		l_index = 1;
	
	for (int i = index1; i <= index2; i++) {
		c_grid->lists[l_index][i].remove(car->getID());
	}
	
	if (car->getCentAngle() >= 2 * M_PI)
			return -100;
	
	int ret = 0;
	if (_y * direction < 0) {
		if ( _x > -len1/2) {
			tmp = _x - playspeed * speed * _TIME_FRAC;
			if (tmp < -len1/2) {
				x = this->x - len1/2;
				//tmp += len1/2;
				//angle -= (tmp + len1/2) / _radius * direction;
				angle -= (tmp + len1/2) / _radius;
				ret = 0;
			} else {
				x -= playspeed * speed * _TIME_FRAC;
				ret = -1;
			}
		}
		else {
			if (fabs(angle) >= M_PI) {
				y = -y;
				angle = 0;
				x += playspeed * speed * _TIME_FRAC;
				ret = 1;
			} else {
				tmp = angle - playspeed * speed * _TIME_FRAC / _radius * direction;
				if (tmp < -M_PI) {
					angle = - M_PI;
					x += (tmp + M_PI * direction) * _radius;
				} else {
					angle = tmp;
					//angle -= -playspeed * speed * _TIME_FRAC / _radius * (-direction);
					ret = 0;
				}
			}
		}
	} else {
		if (_x > len2/2)
			ret = -100;
		else {
			x += playspeed * speed * _TIME_FRAC;
			ret = 1;
		}
	}
	
	if (!car->is_crashing) {
		dist = get_dist(x, y, angle, _radius);
		index1 = get_index(dist - _CAR_X * scale);
		index2 = get_index(dist + _CAR_X * scale);
	
		if (index1 < 0)
			index1 = 0;
		else if (index1 > ent_dist)
			index1 = ent_dist;
		if (index2 < 0)
			index2 = 0;
		else if (index2 > ent_dist)
			index2 = ent_dist;
	
		for (int i = index1; i <= index2; i++)
			c_grid->lists[l_index][i].append(car);
	
		for (int i = index1; i <= index2; i++)
			check_crashes(c_grid->lists[l_index][i]);
	}
	
	return ret;
}

bool UTurn::crash(Car* car1, Car* car2)
{
	if (car1->getX() > GLfloat(1)/_hscale || car1->getX() > GLfloat(1)/_hscale)
		return false;
	if (car1->is_crashing || car2->is_crashing)
		return false;
	GLfloat scale1 = car1->getScale();
	GLfloat scale2 = car2->getScale();
	GLfloat radius1, radius2;
	if (car1->getLane() == 'r')
		radius1 = this->radius + (0.5 - direction * 0.25) * width;
	else if (car1->getLane() == 'l')
		radius1 = this->radius + (0.5 + direction * 0.25) * width;
	if (car2->getLane() == 'r')
		radius2 = this->radius + (0.5 - direction * 0.25) * width;
	else if (car2->getLane() == 'l')
		radius2 = this->radius + (0.5 + direction * 0.25) * width;
	//static GLfloat entire_dist1 = len1 + len2 + M_PI * radius1;
	//static GLfloat entire_dist2 = len1 + len2 + M_PI * radius2;
	GLfloat dist1 = get_dist(car1->getX(), car1->getY(), car1->getAngle(), radius1);
	GLfloat dist2 = get_dist(car2->getX(), car2->getY(), car2->getAngle(), radius2);
	if (dist1 >= dist2) {
		if ( dist1 - _CAR_X * scale1 <= dist2 + _CAR_X * scale2 )
			return true;
	} else {
		if ( dist2 - _CAR_X * scale2 <= dist1 + _CAR_X * scale1 )
			return true;
	}
	return false;
}

bool UTurn::check_crashes(CarList list)
{
	bool ret = false;
	if (list.length() < 2)
		return false;
	CarNode *first = list.getRoot();
	if (first == 0)
		return false;
	CarNode *second;
	while (first->next != 0) {
		second = first->next;
		while (second != 0) {
			if ( crash(first->car, second->car) ) {
				first->car->crash(second->car);
				num_accidents++;
				ret = true;
			}
			second = second->next;
		}
		first = first->next;
	}
	return ret;
}

void UTurn::check_crashes()
{
	for (int i = 0; i < c_grid->N; i++) {
		for (int j = 0; j < c_grid->M; j++) {
			check_crashes(c_grid->lists[i][j]);
		}
	}
}

GLfloat UTurn::get_dist(GLfloat x, GLfloat y, GLfloat angle, GLfloat radius)
{
	angle = fabs(angle);
	//GLfloat _x = ceil(len1/2) + x - this->x;
	GLfloat _x = len1/2 - x + this->x;
	//GLfloat _x = x - this->x;
	//_x = _x > 0? len1/2 - _x : len1/2 + _x;
	GLfloat _y = y - this->y;
	GLfloat ret = 0;
	if (angle == 0) {
		if (_y * direction < 0) {
			ret = _x;
		} else {
			ret = len1 + M_PI * radius + len2 - _x;
		}
	} else {
		ret = len1 + radius * angle;
	}
	
	if (ret < 0)
		ret = 0;
	else if (ret > len1 + len2 + M_PI * radius)
		ret = len1 + len2 + M_PI * radius;
	return ret;
}

int UTurn::get_index(GLfloat dist)
{
	return dist / no_pix_node;
}
