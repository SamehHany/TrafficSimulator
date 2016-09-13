#include <iostream>
#include <vector>
#include <sstream>
#include "Object2D.h"
#include "Road.h"
#include "Car.h"
#include "CarList.h"

using namespace std;

const GLfloat WindowWidth = 900;
const GLfloat WindowHeight = 600;

const GLfloat _hscale = (GLfloat)1/450; // horizontal scale
const GLfloat _vscale = (GLfloat)1/300; // vertical scale

const char *file_path = "traffic_flow.txt";

GLuint __action;
GLuint __hscale;
GLuint __vscale;
GLuint __scale;
GLuint __rot;
GLuint __x_trans_cent;
GLuint __y_trans_cent;
GLuint __x_trans;
GLuint __y_trans;
GLuint __rot_mat;

//GLuint action, rotation;

vector<Object2D*> objects;

Circle *sign, *green_sign, *red_sign;
Road *road1, *road2;
Car **cars;
int no_cars;

GLfloat playspeed = 1;

bool time_frozen = false;

void init( void )
{
	// Load shaders and use the resulting shader program
	Object2D::shaderProgram = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( Object2D::shaderProgram );
	
	__action = glGetUniformLocation(Object2D::shaderProgram, "action");
	__hscale = glGetUniformLocation(Object2D::shaderProgram, "hscale");
	__vscale = glGetUniformLocation(Object2D::shaderProgram, "vscale");
	__scale = glGetUniformLocation(Object2D::shaderProgram, "scale");
	__rot = glGetUniformLocation(Object2D::shaderProgram, "rot");
	__x_trans_cent = glGetUniformLocation(Object2D::shaderProgram, "x_trans_cent");
	__y_trans_cent = glGetUniformLocation(Object2D::shaderProgram, "y_trans_cent");
	__x_trans = glGetUniformLocation(Object2D::shaderProgram, "x_trans");
	__y_trans = glGetUniformLocation(Object2D::shaderProgram, "y_trans");
	__rot_mat = glGetUniformLocation(Object2D::shaderProgram, "rot_mat");
	
	glUniform1i(__action, _NO_ACTION);
	glUniform1f(__hscale, _hscale);
	glUniform1f(__vscale, _vscale);
	glUniform1f(__scale, 1.0f);
	glUniform1f(__rot, 0.0f);
	glUniform1f(__x_trans_cent, 0.0f);
	glUniform1f(__y_trans_cent, 0.0f);
	glUniform1f(__x_trans, 0.0f);
	glUniform1f(__y_trans, 0.0f);
	
	Point center(-400.0, 250.0);
	green_sign = new Circle(center, 30, 30, green);
	red_sign = new Circle(center, 30, 30, red);
	sign = green_sign;
	center = Point(0, 0);
	
	road1 = new UTurn(590, 200, 75, 75, 155);
	road2 = new UTurn(590, 120, 75, 75, 155);
	road2->direction = 1;
	//road->direction = 1;
	
	cars = readCars(file_path, no_cars, road1, road2);

	glClearColor( 0.0, 0.39, 0.0, 1.0 ); // green background
}

int frameCount = 0;

CarList disp_cars;
int index = 0;
void display()
{
	if (!time_frozen) {
		frameCount++;

		glClear( GL_COLOR_BUFFER_BIT );     // clear the window
	
		sign->render();	
		road1->display();
		road2->display();
		while (cars[index]? get_time() >= cars[index]->arrTime() : 0) {
			disp_cars.append(cars[index++]);
		}
		CarNode *n = disp_cars.getRoot();
		while (n != 0) {
			if ( n->car->display() == -1 )
				disp_cars.remove( n->car->getID() );
			n = n->next;
		}
		//cars[3]->display(get_time());
	
		glFlush();
	}
}

void cleanup() {
	//destroy shaders
	glUseProgram(0);
	glDeleteProgram(Object2D::shaderProgram);
	//destroy shapes and its vertices
	delete red_sign;
	delete green_sign;
	delete road1;
	delete road2;
	deleteCars(cars, no_cars);
}

//void idle() {
//    glutPostRedisplay();
//}
//----------------------------------------------------------------------------

time_t start_time = 0;

void timerFunc(int value) {

    if (value != 0){ //not the first call
        stringstream ss;
        ss << "FPS: " << frameCount  * 4 << " -- time: " << get_time() << " -- play speed: " << playspeed;
        glutSetWindowTitle(ss.str().c_str());
    }
    
    glutPostRedisplay();

    //reset frame count to 0
    frameCount = 0;
    //call the function again after 250 milli-seconds
    //if (get_time() > 19900)
    //	playspeed = 0.5;
    glutTimerFunc ( 1000 * _TIME_FRAC, timerFunc, 1 );
}

void keyboardNormal( unsigned char key, int x, int y )
{
	time_t t; 
	if (key == ' ') {
		if (!time_frozen)
			sign = red_sign;
		else sign = green_sign;
		display();
		time_frozen = !time_frozen;
		t = get_time();
		int accidents = road1->getNumAcc() + road2->getNumAcc();
		cout << "Simulation time : " << t << endl
		     << "Number of Cars : " << disp_cars.length() << endl
		     << "Number of accidents : " << accidents << endl
		     << "Rate of accidents : " << (double)accidents * 1000 / t << " accident per second"
		     << endl;
	}
}

void keyboardSpecial( int key, int x, int y )
{

	switch ( key ) {
	case GLUT_KEY_UP:
		if (playspeed < 128)
			playspeed = playspeed < 32 ? playspeed*2 : playspeed;
		break;
	case GLUT_KEY_DOWN:
		if (playspeed > 0.0625)
			playspeed /= 2;
		break;
	}

}

int main(int argc, char **argv)
{
	// Randome seeding
	FILE *fp = fopen("/dev/urandom", "r");
	long long seed;
	fread(&seed, sizeof(seed), 1, fp);
	fclose(fp);
	srand(seed);
	// End of random seeding
	
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_RGBA );
	glutInitWindowSize( WindowWidth, WindowHeight);
	glutCreateWindow( "Assignment 1" );
	glewInit();
	init();
	//glutIdleFunc( idle );
	glutTimerFunc( 1000 * _TIME_FRAC, timerFunc, 0 );
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboardNormal );
	glutSpecialFunc( keyboardSpecial );
	//glutKeyboardFunc( keyboard );
	glutCloseFunc(cleanup);
	
	timeval tm;
	gettimeofday(&tm, 0);
	start_time = tm.tv_sec * 1000 + tm.tv_usec * 0.001;
	
	glutMainLoop();
	
	return 0;
}
