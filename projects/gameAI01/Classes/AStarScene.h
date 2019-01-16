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

	int cost; //�ķ�ֵ G
	int pred; //Ԥ��ֵ F

	OpenPoint *father; //���ڵ�
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

		//����Ԥ��ֵ �� ֱ����һ��ľ������10��б����һ��ľ������14

		//���λ��x,yȡ����ֵ
		int relativex = std::abs(end.x - x);
		int relativey = std::abs(end.y - y);
		//x,yƫ��ֵn
		int n = relativex - relativey;
		//Ԥ��ֵpred = (max�Cn)*14+n*10+c
		pred = std::max(relativex, relativey) * 14 - std::abs(n) * 4 + c;



		/*
		����㷨�Ҹо������� ��ʱû�����
		c Ϊ����ǰ����Ҫ������ֵ ==��G

		������ˮƽ H = ˮƽ������ * 10 + ��ֱ������ * 10

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

	//	//����Ԥ��ֵ �� ֱ����һ��ľ������10��б����һ��ľ������14

	//	//���λ��x,yȡ����ֵ
	//	int relativex = std::abs(end.x - p.x);
	//	int relativey = std::abs(end.y - p.y);
	//	//x,yƫ��ֵn
	//	int n = relativex - relativey;
	//	//Ԥ��ֵpred = (max�Cn)*14+n*10+c
	//	pred = std::max(relativex, relativey) * 14 - std::abs(n) * 4 + c;



	//	/*
	//	����㷨�Ҹо������� ��ʱû�����
	//	c Ϊ����ǰ����Ҫ������ֵ ==��G

	//	������ˮƽ H = ˮƽ������ * 10 + ��ֱ������ * 10

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
	std::map<std::string, OpenPoint*>m_openComPareList; //���ڼ�¼openlist���Ԫ�أ���ѯ�Ƚ���


	int m_row;
	int m_col;

	//���
	//��ʱҪ�ѵ�·���ǳ���������A*�㷨��һ�θ����Ĵ��ۺܸߣ������Ϸ�Ŀ��١�
	//��ôΪ�˱�֤ÿ���������ᳬ��һ�����ۣ���������������ƣ�ÿ��һ������� + 1���ѵ�һ��������ƻ�û�ѵ��յ㣬�򷵻�ʧ��ֵ
	int m_deepth;

};





#endif // __AStarScene_SCENE_H__
