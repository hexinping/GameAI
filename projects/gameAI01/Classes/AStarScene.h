#ifndef __AStarScene_SCENE_H__
#define __AStarScene_SCENE_H__

#include "cocos2d.h"
#include <map>
using namespace cocos2d;

class BasePoint
{
public:

	int x;
	int y;

	bool operator ==(const BasePoint & otherPoint)
	{
		return x == otherPoint.x && y == otherPoint.y;
	}

};


class OpenPoint :public BasePoint
{
public:

	int cost; //耗费值 G
	int pred; //预测值 F

	OpenPoint *father; //父节点
	OpenPoint() = default;
	OpenPoint(int px, int py)
	{
		x = px;
		y = py;
	}

	void reset()
	{
		cost = 0;
		father = nullptr;
		x = 0;
		y = 0;
	}

	void calulatePoint(const BasePoint &end, int c, OpenPoint *pFather)
	{
		cost = c;
		father = pFather;

		//计算预测值 ： 直着走一格的距离等于10，斜着走一格的距离等于14

		//相对位移x,y取绝对值
		int relativex = std::abs(end.x - x);
		int relativey = std::abs(end.y - y);
		//x,y偏移值n
		int n = relativex - relativey;
		//预测值pred = (max–n)*14+n*10+c
		pred = std::max(relativex, relativey) * 14 - std::abs(n) * 4 + c;



		/*
		这个算法我感觉有问题 暂时没法理解
		c 为到当前点需要的消耗值 ==》G

		按正常水平 H = 水平格子数 * 10 + 竖直格子数 * 10

		int relativex = std::abs(end.x - p.x);
		int relativey = std::abs(end.y - p.y);

		int H = (relativex + relativey)*10;
		pred = c + H;
		*/
	}

	//OpenPoint(const BasePoint &p, const BasePoint &end, int c, OpenPoint *pFather)
	//{
	///*	x = p.x;
	//	y = p.y;*/
	//	BasePoint(p);
	//	cost = c;
	//	father = pFather;

	//	//计算预测值 ： 直着走一格的距离等于10，斜着走一格的距离等于14

	//	//相对位移x,y取绝对值
	//	int relativex = std::abs(end.x - p.x);
	//	int relativey = std::abs(end.y - p.y);
	//	//x,y偏移值n
	//	int n = relativex - relativey;
	//	//预测值pred = (max–n)*14+n*10+c
	//	pred = std::max(relativex, relativey) * 14 - std::abs(n) * 4 + c;



	//	/*
	//	这个算法我感觉有问题 暂时没法理解
	//	c 为到当前点需要的消耗值 ==》G

	//	按正常水平 H = 水平格子数 * 10 + 竖直格子数 * 10

	//	int relativex = std::abs(end.x - p.x);
	//	int relativey = std::abs(end.y - p.y);

	//	int H = (relativex + relativey)*10;
	//	pred = c + H;
	//	*/
	//}


};



class AStarScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	CREATE_FUNC(AStarScene);

	void initMap(const OpenPoint & p1, const OpenPoint & p2);

	DrawNode *m_drawNode;

	bool inBarrierAndCloseList(const BasePoint & pos);

	//OpenPoint* createOpenPoint(OpenPoint * p, const OpenPoint& end, int c, OpenPoint* fatherp);

	
	void open(OpenPoint& pointToOpen, const OpenPoint &end);

	OpenPoint* findway(OpenPoint& start, const OpenPoint& end);

	void start();
	~AStarScene();

	void updateOpenlist(OpenPoint *p, bool isAdd);

public:
	//char ** m_mapBuffer;
	//bool ** m_closeAndBarrierList;

	std::map<std::string, bool>m_closeAndBarrierList;
	std::map<std::string, OpenPoint*>m_openComPareList; //用于记录openlist里的元素，查询比较用


	int m_row;
	int m_col;

	//深度
	//有时要搜的路径非常长，利用A*算法搜一次付出的代价很高，造成游戏的卡顿。
	//那么为了保证每次搜索不会超过一定代价，可以设置深度限制，每搜一次则深度 + 1，搜到一定深度限制还没搜到终点，则返还失败值
	int m_deepth;

};





#endif // __AStarScene_SCENE_H__
