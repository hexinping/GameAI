#pragma once


#include "common/misc/utils.h"
#include <iosfwd>
#include <limits>
#include <winsock2.h>
#include <windows.h>

class Vector2D
{

public:
	double x;
	double y;

	Vector2D() :x(0.0), y(0.0){}
	Vector2D(double a, double b) :x(a), y(b){}

	//sets x and y to zero
	void Zero(){ x = 0.0; y = 0.0; }

	//returns true if both x and y are zero
	bool isZero()const{ return (x*x + y*y) < MinDouble; }



	//返回矢量长度
	double    Length()const;

	//返回矢量长度的平方
	double    LengthSq()const;

	//返回矢量归一化
	 void      Normalize();

	//点乘
	 double    Dot(const Vector2D& v2)const;

	//如果v2在this矢量的顺时针方向返回正值，在逆时针方向返回负值
	//假设Y轴的箭头是指向下面的，X轴指向右边就像一个窗口应用
	 int       Sign(const Vector2D& v2)const;

	//返回与this矢量正交的矢量
	 Vector2D  Perp()const;

	//调整x和y使矢量的长度不会超过最大值
	 void      Truncate(double max);

	//返回this矢量与一个作为参数被传递的矢量之间的距离
	 double    Distance(const Vector2D &v2)const;

	//返回this矢量与一个作为参数被传递的矢量之间的距离的平方
	 double    DistanceSq(const Vector2D &v2)const;

	//返回以参数为法向量的反射向量
	 void      Reflect(const Vector2D& norm);

	//返回与this矢量相反的矢量
	 Vector2D  GetReverse()const;


	//we need some overloaded operators
	const Vector2D& operator+=(const Vector2D &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vector2D& operator-=(const Vector2D &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2D& operator*=(const double& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const Vector2D& operator/=(const double& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==(const Vector2D& rhs)const
	{
		return (isEqual(x, rhs.x) && isEqual(y, rhs.y));
	}

	bool operator!=(const Vector2D& rhs)const
	{
		return (x != rhs.x) || (y != rhs.y);
	}
};

inline Vector2D operator*(const Vector2D &lhs, double rhs);
inline Vector2D operator*(double lhs, const Vector2D &rhs);
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator/(const Vector2D &lhs, double val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);

//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D &lhs, double rhs)
{
	Vector2D result(lhs);
	result *= rhs;
	return result;
}

inline Vector2D operator*(double lhs, const Vector2D &rhs)
{
	Vector2D result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D &lhs, double val)
{
	Vector2D result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}



//------------------------------------------------------------------------non member functions

inline Vector2D Vec2DNormalize(const Vector2D &v)
{
	Vector2D vec = v;

	double vector_length = vec.Length();

	if (vector_length > std::numeric_limits<double>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}


inline double Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double Vec2DLength(const Vector2D& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

inline double Vec2DLengthSq(const Vector2D& v)
{
	return (v.x*v.x + v.y*v.y);
}


inline Vector2D POINTStoVector(const POINTS& p)
{
	return Vector2D(p.x, p.y);
}

inline Vector2D POINTtoVector(const POINT& p)
{
	return Vector2D((double)p.x, (double)p.y);
}

inline POINTS VectorToPOINTS(const Vector2D& v)
{
	POINTS p;
	p.x = (short)v.x;
	p.y = (short)v.y;

	return p;
}

inline POINT VectorToPOINT(const Vector2D& v)
{
	POINT p;
	p.x = (long)v.x;
	p.y = (long)v.y;

	return p;
}



//treats a window as a toroid
inline void WrapAround(Vector2D &pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX) { pos.x = 0.0; }

	if (pos.x < 0)    { pos.x = (double)MaxX; }

	if (pos.y < 0)    { pos.y = (double)MaxY; }

	if (pos.y > MaxY) { pos.y = 0.0; }
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(Vector2D p,
	Vector2D top_left,
	Vector2D bot_rgt)
{
	return (p.x < top_left.x) || (p.x > bot_rgt.x) ||
		(p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(Vector2D p,
	Vector2D top_left,
	Vector2D bot_rgt)
{
	return !((p.x < top_left.x) || (p.x > bot_rgt.x) ||
		(p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(Vector2D p, int left, int top, int right, int bottom)
{
	return !((p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom));
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(Vector2D posFirst,
	Vector2D facingFirst,
	Vector2D posSecond,
	double    fov)
{
	Vector2D toTarget = Vec2DNormalize(posSecond - posFirst);

	return facingFirst.Dot(toTarget) >= cos(fov / 2.0);
}