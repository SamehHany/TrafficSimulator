#ifndef COMMON_H
#define COMMON_H

//----------------------------------------------------------------------------
//
// --- Include system headers ---
//

#include <cmath>
#include <iostream>
#include <stdio.h>

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

//----------------------------------------------------------------------------
//
// --- Include OpenGL header files and helpers ---
//
//   The location of these files vary by operating system.  We've included
//     copies of open-soruce project headers in the "GL" directory local
//     this this "include" directory.
//

#ifdef __APPLE__  // include Mac OS X verions of headers
#  include <OpenGL/OpenGL.h>
#  include <GLUT/glut.h>
#else // non-Mac OS X operating systems
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/freeglut_ext.h>
#endif  // __APPLE__

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

extern const GLfloat WindowWidth;
extern const GLfloat WindowHeight;

extern const GLfloat _hscale;
extern const GLfloat _vscale;

extern GLuint __action;
extern GLuint __hscale;
extern GLuint __vscale;
extern GLuint __scale;
extern GLuint __rot;
extern GLuint __x_trans_cent;
extern GLuint __y_trans_cent;
extern GLuint __x_trans;
extern GLuint __y_trans;
extern GLuint __rot_mat;

extern GLfloat playspeed;

#include <sys/time.h>
#include <math.h>

extern time_t start_time;
extern bool time_frozen;

inline time_t get_time() {
	static timeval tm;
	static time_t last_time = 0;
	static time_t _time = 0;
	static time_t time_elapsed = 0;
	static bool frozen = false;
	static time_t frozen_time = 0;
	gettimeofday(&tm, 0);
	if (time_frozen && !frozen) {
		frozen_time = tm.tv_sec * 1000 + tm.tv_usec * 0.001;
		frozen = true;
		time_elapsed = tm.tv_sec * 1000 + tm.tv_usec * 0.001 - start_time - last_time;
		_time += ceil(playspeed * time_elapsed);
		last_time += time_elapsed;
		return _time;
	}
	else if (time_frozen) {
		return _time;
	}
	if (frozen) {
		last_time += tm.tv_sec * 1000 + tm.tv_usec * 0.001 - frozen_time;
		frozen = false;
	}
	time_elapsed = tm.tv_sec * 1000 + tm.tv_usec * 0.001 - start_time - last_time;
	_time += ceil(playspeed * time_elapsed);
	last_time += time_elapsed;
	
	return _time;
}

inline void toggle_freeze() {
	
	static timeval tm;
	gettimeofday(&tm, 0);
	static time_t last_time = 0;
	static time_t time_elapsed = 0;
	time_elapsed = tm.tv_sec * 1000 + tm.tv_usec * 0.001 - start_time - last_time;
	last_time += time_elapsed;
	if (time_frozen) {
		//get_time(-time_elapsed);
		time_frozen = false;
	}
	else time_frozen = true;
}


#define _NO_ACTION 0
#define _ROTATE 1
#define _ROTATE_CENTER 2
#define _SCALE 4
#define _TRANSFORM 8
#define _ROT_MAT 16

#define _TIME_FRAC 0.025

#endif
