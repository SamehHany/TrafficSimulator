#include "Object2D.h"

GLuint Object2D::shaderProgram = 0;

/*Object2D::Object2D(const Object2D &o)
{
	pointsNum = o.pointsNum;
	points = new Point[pointsNum];
	colors = new Color[pointsNum];
	
	for (int i = 0; i < pointsNum; i++) {
		points[i] = o.points[i];
		colors[i] = o.colors[i];
	}
}*/

void Object2D::render() {}

Object2D* Object2D::copy()
{
	Object2D *o = new Object2D;
	o->pointsNum = pointsNum;
	o->points = new Point[pointsNum];
	o->colors = new Color[pointsNum];
	
	for (int i = 0; i < pointsNum; i++) {
		o->points[i] = points[i];
		o->colors[i] = colors[i];
	}
	
	return o;
}

void Object2D::init() {
    // Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // BUFFERS
    // Create and initialize a buffer object
    // NOTE: since both vertices positions and colors are static data
    // (ie. have the same use) we can put them on the same buffer
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, (sizeof(Point) + sizeof(Color)) * pointsNum, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(Point) * pointsNum, points);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(Point) * pointsNum, sizeof(Color) * pointsNum, colors);

    // ATTRIBUTES
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( shaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(0));

    // Initialize the vertex color attribute from the vertex shader
    GLuint loc2 = glGetAttribLocation( shaderProgram, "vColor" );
    glEnableVertexAttribArray( loc2 );
    glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(sizeof(Point) * pointsNum) );
}

Object2D::~Object2D (){
    // delete vertex array object
    glDeleteVertexArrays(1,&vao);

    // delete buffers
    glDeleteBuffers(1,&buffer);

    // disable position and color attrib arays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // unbind vao and buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

GLuint Object2D::getBuffer() {
	return buffer;
}
