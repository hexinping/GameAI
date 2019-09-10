#include "simpleBall_Vector.h"



////------------------------- Length ---------------------------------------
////
////  returns the length of a 2D vector
////------------------------------------------------------------------------
double simpleBall_Vector::Length()const
{
	return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
double simpleBall_Vector::LengthSq()const
{
	return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
double simpleBall_Vector::Dot(const simpleBall_Vector &v2)const
{
	return x*v2.x + y*v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum { clockwise = 1, anticlockwise = -1 };

int simpleBall_Vector::Sign(const simpleBall_Vector& v2)const
{
	if (y*v2.x > x*v2.y)
	{
		return anticlockwise;
	}
	else
	{
		return clockwise;
	}
}

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
simpleBall_Vector simpleBall_Vector::Perp()const
{
	return simpleBall_Vector(-y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
double simpleBall_Vector::Distance(const simpleBall_Vector &v2)const
{
	double ySeparation = v2.y - y;
	double xSeparation = v2.x - x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}



//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
double simpleBall_Vector::DistanceSq(const simpleBall_Vector &v2)const
{
	double ySeparation = v2.y - y;
	double xSeparation = v2.x - x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
void simpleBall_Vector::Truncate(double max)
{
	if (this->Length() > max)
	{
		this->Normalize();

		*this *= max;
	}
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
void simpleBall_Vector::Reflect(const simpleBall_Vector& norm)
{
	//double a = 2.0 * this->Dot(norm);
	//simpleBall_Vector t = norm.GetReverse();
	//t.x = a * t.x;
	//t.y = a * t.y;
	//*this += t;

	*this += 2.0 * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
simpleBall_Vector simpleBall_Vector::GetReverse()const
{
	return simpleBall_Vector(-this->x, -this->y);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
void simpleBall_Vector::Normalize()
{
	double vector_length = this->Length();

	if (vector_length > std::numeric_limits<double>::epsilon())
	{
		this->x /= vector_length;
		this->y /= vector_length;
	}
}

