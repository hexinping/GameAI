#pragma once

/*
	ʸ����
*/

#include "misc/utils.h"
#include <winsock2.h>
#include <windows.h>
class simpleBall_Vector
{
public:

	double x;
	double y;


	simpleBall_Vector() :x(0.0), y(0.0){};
	simpleBall_Vector(double a, double b) :x(a), y(b){}

	~simpleBall_Vector();


	void Zero(){ x = 0.0; y = 0.0;}
	bool isZero()const{ return (x*x + y*y) < MinDouble; }

	double Length() const;
	double LengthSq() const;

	void Normalize();
	double Dot(const simpleBall_Vector& v2) const;

	//���v2��thisʸ����˳ʱ�뷽�򷵻���ֵ������ʱ�뷽�򷵻ظ�ֵ
	//����Y��ļ�ͷ��ָ������ģ�X��ָ���ұ߾���һ������Ӧ��
	int       Sign(const simpleBall_Vector& v2)const;

	simpleBall_Vector Perp()const;

	//����x��yʹʸ���ĳ��Ȳ��ᳬ�����ֵ
	void      Truncate(double max);

	double    Distance(const simpleBall_Vector &v2)const;

	//����thisʸ����һ����Ϊ���������ݵ�ʸ��֮��ľ����ƽ��
	double    DistanceSq(const simpleBall_Vector &v2)const;
	
	//�����Բ���Ϊ�������ķ�������
	void      Reflect(const simpleBall_Vector& norm);

	//������thisʸ���෴��ʸ��
	simpleBall_Vector  GetReverse()const;


	//we need some overloaded operators
	const simpleBall_Vector& operator+=(const simpleBall_Vector &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const simpleBall_Vector& operator-=(const simpleBall_Vector &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const simpleBall_Vector& operator*=(const double& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const simpleBall_Vector& operator/=(const double& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==(const simpleBall_Vector& rhs)const
	{
		return (isEqual(x, rhs.x) && isEqual(y, rhs.y));
	}

	bool operator!=(const simpleBall_Vector& rhs)const
	{
		return (x != rhs.x) || (y != rhs.y);
	}
};

inline simpleBall_Vector operator*(const simpleBall_Vector &lhs, double rhs);
inline simpleBall_Vector operator*(double lhs, const simpleBall_Vector &rhs);
inline simpleBall_Vector operator-(const simpleBall_Vector &lhs, const simpleBall_Vector &rhs);
inline simpleBall_Vector operator+(const simpleBall_Vector &lhs, const simpleBall_Vector &rhs);
inline simpleBall_Vector operator/(const simpleBall_Vector &lhs, double val);
std::ostream& operator<<(std::ostream& os, const simpleBall_Vector& rhs);
std::ifstream& operator>>(std::ifstream& is, simpleBall_Vector& lhs);



//------------------------------------------------------------------------operator overloads
inline simpleBall_Vector operator*(const simpleBall_Vector &lhs, double rhs)
{
	simpleBall_Vector result(lhs);
	result *= rhs;
	return result;
}

inline simpleBall_Vector operator*(double lhs, const simpleBall_Vector &rhs)
{
	simpleBall_Vector result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline simpleBall_Vector operator-(const simpleBall_Vector &lhs, const simpleBall_Vector &rhs)
{
	simpleBall_Vector result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

//overload the + operator
inline simpleBall_Vector operator+(const simpleBall_Vector &lhs, const simpleBall_Vector &rhs)
{
	simpleBall_Vector result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

//overload the / operator
inline simpleBall_Vector operator/(const simpleBall_Vector &lhs, double val)
{
	simpleBall_Vector result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}



//------------------------------------------------------------------------non member functions

inline simpleBall_Vector Vec2DNormalize(const simpleBall_Vector &v)
{
	simpleBall_Vector vec = v;

	double vector_length = vec.Length();

	if (vector_length > std::numeric_limits<double>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}


inline double Vec2DDistance(const simpleBall_Vector &v1, const simpleBall_Vector &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double Vec2DDistanceSq(const simpleBall_Vector &v1, const simpleBall_Vector &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double Vec2DLength(const simpleBall_Vector& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

inline double Vec2DLengthSq(const simpleBall_Vector& v)
{
	return (v.x*v.x + v.y*v.y);
}


inline simpleBall_Vector POINTStoVector(const POINTS& p)
{
	return simpleBall_Vector(p.x, p.y);
}

inline simpleBall_Vector POINTtoVector(const POINT& p)
{
	return simpleBall_Vector((double)p.x, (double)p.y);
}

inline POINTS VectorToPOINTS(const simpleBall_Vector& v)
{
	POINTS p;
	p.x = (short)v.x;
	p.y = (short)v.y;

	return p;
}

inline POINT VectorToPOINT(const simpleBall_Vector& v)
{
	POINT p;
	p.x = (long)v.x;
	p.y = (long)v.y;

	return p;
}



//treats a window as a toroid
inline void WrapAround(simpleBall_Vector &pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX) { pos.x = 0.0; }

	if (pos.x < 0)    { pos.x = (double)MaxX; }

	if (pos.y < 0)    { pos.y = (double)MaxY; }

	if (pos.y > MaxY) { pos.y = 0.0; }
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(simpleBall_Vector p,
	simpleBall_Vector top_left,
	simpleBall_Vector bot_rgt)
{
	return (p.x < top_left.x) || (p.x > bot_rgt.x) ||
		(p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(simpleBall_Vector p,
	simpleBall_Vector top_left,
	simpleBall_Vector bot_rgt)
{
	return !((p.x < top_left.x) || (p.x > bot_rgt.x) ||
		(p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(simpleBall_Vector p, int left, int top, int right, int bottom)
{
	return !((p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom));
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(simpleBall_Vector posFirst,
	simpleBall_Vector facingFirst,
	simpleBall_Vector posSecond,
	double    fov)
{
	simpleBall_Vector toTarget = Vec2DNormalize(posSecond - posFirst);

	return facingFirst.Dot(toTarget) >= cos(fov / 2.0);
}
