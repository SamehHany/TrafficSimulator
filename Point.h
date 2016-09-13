#ifndef POINT_H
#define POINT_H

#include "common.h"

struct Point
{
	GLfloat x;
	GLfloat y;
	
	//
	//  --- Constructors and Destructors ---
	//
	
	Point( GLfloat s = GLfloat(0.0) ) : x(s), y(s) {}

	Point( GLfloat x, GLfloat y ) : x(x), y(y) {}

	Point( const Point& v ) { x = v.x;  y = v.y; }
	
	//
	//  --- Indexing Operator ---
	//
	
	GLfloat& operator [] ( int i ) { return *(&x + i); }
	const GLfloat operator [] ( int i ) const { return *(&x + i); }
	
	//
	//  --- (non-modifying) Arithematic Operators ---
	//
	
	Point operator - () const // unary minus operator
		{ return Point( -x, -y ); }

	Point operator + ( const Point& v ) const
		{ return Point( x + v.x, y + v.y ); }

	Point operator - ( const Point& v ) const
		{ return Point( x - v.x, y - v.y ); }

	Point operator * ( const GLfloat s ) const
		{ return Point( s*x, s*y ); }

	Point operator * ( const Point& v ) const
		{ return Point( x*v.x, y*v.y ); }

	friend Point operator * ( const GLfloat s, const Point& v )
		{ return v * s; }

	Point operator / ( const GLfloat s ) const {
#ifdef DEBUG
		if ( std::fabs(s) < DivideByZeroTolerance )
		{
			std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
				  << "Division by zero" << std::endl;
			return Point();
		}
#endif // DEBUG

		GLfloat r = GLfloat(1.0) / s;
		return *this * r;
	}
	
	//
	//  --- (modifying) Arithematic Operators ---
	//
	
	Point& operator += ( const Point& v )
		{ x += v.x;  y += v.y;   return *this; }

	Point& operator -= ( const Point& v )
		{ x -= v.x;  y -= v.y;  return *this; }

	Point& operator *= ( const GLfloat s )
		{ x *= s;  y *= s;   return *this; }

	Point& operator *= ( const Point& v )
		{ x *= v.x;  y *= v.y; return *this; }

	Point& operator /= ( const GLfloat s ) {
#ifdef DEBUG
		if ( std::fabs(s) < DivideByZeroTolerance )
		{
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

	friend std::ostream& operator << ( std::ostream& os, const Point& v ) {
		return os << "( " << v.x << ", " << v.y <<  " )";
	}

	friend std::istream& operator >> ( std::istream& is, Point& v )
		{ return is >> v.x >> v.y ; }

	//
	//  --- Conversion Operators ---
	//

	operator const GLfloat* () const
		{ return static_cast<const GLfloat*>( &x ); }

	operator GLfloat* ()
		{ return static_cast<GLfloat*>( &x ); }
};

//----------------------------------------------------------------------------
//
//  Non-class Point Methods
//

inline GLfloat dot( const Point& u, const Point& v ) {
	return u.x * v.x + u.y * v.y;
}

inline GLfloat length( const Point& v ) {
	return std::sqrt( dot(v,v) );
}

inline Point normalize( const Point& v ) {
	return v / length(v);
}

#endif
