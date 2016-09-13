#include "Road.h"

Road::Road(int no_objects)
{
	x = 0.0f;
	y = 0.0f;
	angle = 0.0f;
	scale = 1.0f;
	this->no_objects = no_objects;
	objects = new Object2D*[no_objects];
	flags = _NO_ACTION;
	c_grid = new CollisionGrid;
	num_accidents = 0;
}

Road::Road(int no_objects, Object2D **objects)
{
	x = 0;
	y = 0;
	angle = 0;
	scale = 0;
	this->no_objects = no_objects;
	objects = new Object2D*[no_objects];
	
	for (int i = 0; i < no_objects; i++)
		this->objects[i] = objects[i]->copy();
	flags = _NO_ACTION;
	c_grid = new CollisionGrid;
	num_accidents = 0;
}

void Road::display()
{
	glUniform1i(__action, flags);
	if ( (flags >> 3) & 0x00000001 ) {
		glUniform1f(__x_trans, x);
		glUniform1f(__y_trans, y);
	}
	if ( (flags >> 1) & 0x00000001 ) {
		glUniform1f(__rot, angle);
		glUniform1f(__x_trans_cent, x);
		glUniform1f(__y_trans_cent, y);
	}
	if ( (flags >> 2) & 0x00000001 ) {
		glUniform1f(__scale, scale);
	}
	for (int i = 0; i < no_objects; i++)
		objects[i]->render();
	glUniform1i(__action, _NO_ACTION);
}

int Road::getNumAcc() {
	return num_accidents;
}

Road::~Road()
{
	for (int i = 0; i < no_objects; i++)
		delete objects[i];
	delete [] objects;
	delete c_grid;
}
