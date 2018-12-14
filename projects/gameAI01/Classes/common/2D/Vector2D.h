#pragma once


#include "common/misc/utils.h"

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
	bool isZero()const;


	//返回矢量长度
	inline double    Length()const;

	//返回矢量长度的平方
	inline double    LengthSq()const;

	//返回矢量归一化
	inline void      Normalize();

	//点乘
	inline double    Dot(const Vector2D& v2)const;

	//如果v2在this矢量的顺时针方向返回正值，在逆时针方向返回负值
	//假设Y轴的箭头是指向下面的，X轴指向右边就像一个窗口应用
	inline int       Sign(const Vector2D& v2)const;

	//返回与this矢量正交的矢量
	inline Vector2D  Perp()const;

	//调整x和y使矢量的长度不会超过最大值
	inline void      Truncate(double max);

	//返回this矢量与一个作为参数被传递的矢量之间的距离
	inline double    Distance(const Vector2D &v2)const;

	//返回this矢量与一个作为参数被传递的矢量之间的距离的平方
	inline double    DistanceSq(const Vector2D &v2)const;

	//返回以参数为法向量的反射向量
	inline void      Reflect(const Vector2D& norm);

	//返回与this矢量相反的矢量
	inline Vector2D  GetReverse()const;


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


