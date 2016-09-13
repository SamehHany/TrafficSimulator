#ifndef OBJECT2D_H
#define OBJECT2D_H

#include "Point.h"
#include "Color.h"

class Object2D
{
	public:
		static GLuint shaderProgram;
		int pointsNum;
		Point* points;
		Color* colors;
		
		//methods
		virtual void render();
		virtual Object2D* copy();
		//destructor
		virtual ~Object2D();
		GLuint getBuffer();

	protected:
		GLuint vao, buffer;
		void init();
};

#include "Rectangle.h"
#include "Circle.h"
#include "HollowCircle.h"

#endif
