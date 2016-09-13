#ifndef COLOR_H
#define COLOR_H

#include "common.h"

struct Color
{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
	
	//
	//  --- Constructors and Destructors ---
	//

	Color( GLfloat s = GLfloat(0.0) ) : red(s), green(s), blue(s) {}

	Color( GLfloat red, GLfloat green, GLfloat blue ) : red(red), green(green), blue(blue) {}

	Color( const Color& v ) { red = v.red;  green = v.green;  blue = v.blue; }

	//
	//  --- Indexing Operator ---
	//

	GLfloat& operator [] ( int i ) { return *(&red + i); }
	const GLfloat operator [] ( int i ) const { return *(&red + i); }

	//
	//  --- (non-modifying) Arithematic Operators ---
	//

	Color operator - () const  // unary minus operator
		{ return Color( 1-red, 1-green, 1-blue ); }

	Color operator + ( const Color& v ) const
		{ return Color( red + v.red, green + v.green, blue + v.blue ); }

	Color operator - ( const Color& v ) const
		{ return Color( red - v.red, green - v.green, blue - v.blue ); }

	Color operator * ( const GLfloat s ) const
		{ return Color( s*red, s*green, s*blue ); }

	Color operator * ( const Color& v ) const
		{ return Color( red*v.red, green*v.green, blue*v.blue ); }

	friend Color operator * ( const GLfloat s, const Color& v )
		{ return v * s; }

	Color operator / ( const GLfloat s ) const {
#ifdef DEBUG
		if ( std::fabs(s) < DivideByZeroTolerance ) {
			std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
				  << "Division by zero" << std::endl;
			return Color();
		}
#endif // DEBUG

		GLfloat r = GLfloat(1.0) / s;
		return *this * r;
	}

	//
	//  --- (modifying) Arithematic Operators ---
	//

	Color& operator += ( const Color& v )
		{ red += v.red;  green += v.green;  blue += v.blue;  return *this; }

	Color& operator -= ( const Color& v )
		{ red -= v.red;  green -= v.green;  blue -= v.blue;  return *this; }

	Color& operator *= ( const GLfloat s )
		{ red *= s;  green *= s;  blue *= s;  return *this; }

	Color& operator *= ( const Color& v )
		{ red *= v.red;  green *= v.green;  blue *= v.blue;  return *this; }

	Color& operator /= ( const GLfloat s ) {
#ifdef DEBUG
		if ( std::fabs(s) < DivideByZeroTolerance ) {
			std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
				  << "Division by zero" << std::endl;
		}
#endif // DEBUG

		GLfloat r = GLfloat(1.0) / s;
		*this *= r;

		return *this;
	}
	
	//
	//  --- Insertion and Extraction Operators ---
	//

	friend std::ostream& operator << ( std::ostream& os, const Color& v ) {
		return os << "( " << v.red << ", " << v.green << ", " << v.blue <<  " )";
	}

	friend std::istream& operator >> ( std::istream& is, Color& v )
		{ return is >> v.red >> v.green >> v.blue ; }

	//
	//  --- Conversion Operators ---
	//

	operator const GLfloat* () const
		{ return static_cast<const GLfloat*>( &red ); }

	operator GLfloat* ()
		{ return static_cast<GLfloat*>( &red ); }
};

const Color blue(0,0,1), red(1,0,0), green(0,1,0), yellow(1,1,0), white(1,1,1), gray(0.423,0.423,0.423), dark_green(0,0.39,0);

#endif
