#include "Vector2D.h"

////------------------------- Length ---------------------------------------
////
////  returns the length of a 2D vector
////------------------------------------------------------------------------
 double Vector2D::Length()const
{
	return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
double Vector2D::LengthSq()const
{
	return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
 double Vector2D::Dot(const Vector2D &v2)const
{
	return x*v2.x + y*v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum { clockwise = 1, anticlockwise = -1 };

 int Vector2D::Sign(const Vector2D& v2)const
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
 Vector2D Vector2D::Perp()const
{
	return Vector2D(-y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
 double Vector2D::Distance(const Vector2D &v2)const
{
	double ySeparation = v2.y - y;
	double xSeparation = v2.x - x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}



//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
 double Vector2D::DistanceSq(const Vector2D &v2)const
{
	double ySeparation = v2.y - y;
	double xSeparation = v2.x - x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
 void Vector2D::Truncate(double max)
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
 void Vector2D::Reflect(const Vector2D& norm)
{
	//double a = 2.0 * this->Dot(norm);
	//Vector2D t = norm.GetReverse();
	//t.x = a * t.x;
	//t.y = a * t.y;
	//*this += t;

	*this += 2.0 * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
 Vector2D Vector2D::GetReverse()const
{
	return Vector2D(-this->x, -this->y);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
 void Vector2D::Normalize()
{
	double vector_length = this->Length();

	if (vector_length > std::numeric_limits<double>::epsilon())
	{
		this->x /= vector_length;
		this->y /= vector_length;
	}
}




