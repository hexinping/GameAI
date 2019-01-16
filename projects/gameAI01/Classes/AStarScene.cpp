#include "AStarScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define random(a,b) (rand()%(b-a+1)+a)

float cellW = 30;
float cellH = 30;

//四方的位置 直线行走
BasePoint direction[4] = { BasePoint{ 1, 0 }, BasePoint{ 0, 1 }, BasePoint{ -1, 0 }, BasePoint{ 0, -1 } };

//四角的位置 斜着走
BasePoint corners[4] = { BasePoint{ 1, 1 }, BasePoint{ -1, 1 }, BasePoint{ -1, -1 }, BasePoint{ 1, -1 } };

//比较器，用以优先队列的指针类型比较 ==》open列表用优先队列表示，自定义排序方法，最小的放在首位
struct OpenPointPtrCompare {
	bool operator()(OpenPoint* a, OpenPoint* b){
		return a->pred > b->pred;
	}
};

//使用最大优先队列
std::priority_queue<OpenPoint *, std::vector<OpenPoint*>, OpenPointPtrCompare> openList;


Scene* AStarScene::createScene()
{
	return AStarScene::create();
}


// on "init" you need to initialize your instance
bool AStarScene::init()
{

    if ( !Scene::init() )
    {
        return false;
    }

    m_drawNode = DrawNode::create();
	this->addChild(m_drawNode);

	start();
    return true;
}

void AStarScene::start()
{
	OpenPoint startP(0,0);
	OpenPoint endP(15, 26);

	int centerx = startP.y * cellW + cellW * 0.5;
	int centery = startP.x * cellH + cellH * 0.5;
	Color4F color1(0, 1, 0, 1);
	m_drawNode->drawDot(Vec2(centerx, centery), cellW*0.5, color1);

	centerx = endP.y * cellW + cellW * 0.5;
	centery = endP.x * cellH + cellH * 0.5;
	Color4F color2(0, 0, 1, 1);
	m_drawNode->drawDot(Vec2(centerx, centery), cellW*0.5, color2);

	//保证起点和终点都不是障碍物 也不在闭合列表里
	initMap(startP, endP);

	//根据搜索得到的终点openpoint，一直延路径回溯
	//并将回溯经过的点标记为'O'
	auto rs = findway(startP, endP);
	Color4F color3(1, 1, 1, 1);
	while (rs != nullptr)
	{
		bool isDrawPath = true;
		if (rs->x == startP.x && rs->y == startP.y)
		{
			isDrawPath = false;
		}
		if (isDrawPath)
		{
			float pathx = rs->y * cellW + cellW*0.5;
			float pathy = rs->x * cellH + cellH*0.5;
			m_drawNode->drawDot(Vec2(pathx, pathy), cellW*0.5, color3);
		}
		
		rs = rs->father;
	}

}


void AStarScene::initMap(const OpenPoint & p1, const OpenPoint & p2)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	int row = floor(visibleSize.height / cellH);
	int col = floor(visibleSize.width / cellW);
	m_row = row;
	m_col = col;


	Color4F color(1, 1, 0, 1);
	Color4F color1(1, 0, 0, 1);
	for (int i = 0; i < row;i++)
	{
		for (int j = 0; j < col;j++)
		{
			std::string key = std::to_string(i) + "_" + std::to_string(j);
		
			float x1 = j*cellW;
			float y1 = i*cellH;
			float x2 = x1 + cellW;
			float y2 = y1;
			m_drawNode->drawLine(Vec2(x1, y1), Vec2(x2, y2), color);

			x2 = x1;
			y2 = y1 + cellH;
			m_drawNode->drawLine(Vec2(x1, y1), Vec2(x2, y2), color);


			x1 = x2;
			y1 = y2;
			x2 = x1 + cellW;
			y2 = y1;

			m_drawNode->drawLine(Vec2(x1, y1), Vec2(x2, y2), color);
			x1 = x2;
			y1 = y2;
			x2 = x1;
			y2 = y1 - cellH;
			m_drawNode->drawLine(Vec2(x1, y1), Vec2(x2, y2), color);



			//五分之一概率生成障碍物，不可走
			if (random(1,5) % 5 == 0)
			{
				float centerx = j*cellW + cellW * 0.5;
				float centery = i*cellH + cellH * 0.5;

				//标记地图数据,除起始点和结束点外
				if (p1.x != i && p1.y != j && p2.x != i && p2.y != j)
				{
					m_closeAndBarrierList.insert(std::pair<std::string, bool>(key, true));

					m_drawNode->drawDot(Vec2(centerx, centery), cellW*0.5, color1);

				}
			}
			else
			{
				m_closeAndBarrierList.insert(std::pair<std::string, bool>(key, false));
			}
		}
	}


}

//判断是否在闭合列表或者障碍物里
bool AStarScene::inBarrierAndCloseList(int x,int y)
{
	if (x < 0 || y < 0 || x >= m_row || y >= m_col)
		return true;
	std::string key = std::to_string(x) + "_" + std::to_string(y);
	return m_closeAndBarrierList[key];
}


// 开启检查，检查父节点 
void AStarScene::open(OpenPoint& pointToOpen, const OpenPoint &end)
{
	//每检查一次 深度+1
	m_deepth++;

	//将父节点从openlist里移出
	openList.pop();
	updateOpenlist(&pointToOpen, false);
	m_closeAndBarrierList[std::to_string(pointToOpen.x) + "_" + std::to_string(pointToOpen.y)] = true;

	//检查p点水平和竖直的点
	
	for (int i = 0; i < 4;i++)
	{
		
		int x = pointToOpen.x + direction[i].x;
		int y = pointToOpen.y + direction[i].y;
		
		//如果不是障碍点 也不在闭合列表里
		
		if (!inBarrierAndCloseList(x, y))
		{
			//再次判断是否已经在开启列表，如果已经在open列表，需要比较更新
			std::string k = std::to_string(x) + "_" + std::to_string(y);
			OpenPoint * toCreate = new OpenPoint(x, y);
			//消耗值+10 并且标记父节点为pointToOpen
			toCreate->calulatePoint(end, pointToOpen.cost + 10, &pointToOpen);
			std::map<std::string, OpenPoint*>::iterator iter = m_openComPareList.find(k); //m_openComPareList[k]居然会创建要给null的值
			if (iter == m_openComPareList.end())
			{
				//如果也不在更新列表，直接加入open列表
				openList.push(toCreate);
				updateOpenlist(toCreate, true);
			}
			else
			{
				//如果已经在open列表，需要比较看是否需要更新父节点
				OpenPoint *p = m_openComPareList[k];
				int predOld = p->pred;
				if (predOld > toCreate->pred)
				{
					//如果原有的预测值大于当前的预测值 则更新
					p->cost = pointToOpen.cost + 10;
					p->pred = toCreate->pred;
					p->father = &pointToOpen;
				}

			}
		
		}
		
	}

	//检查p点四角的点 斜边的点
	for (int i = 0; i < 4; ++i) {

		int x = pointToOpen.x + corners[i].x;
		int y = pointToOpen.y + corners[i].y;
	
		//如果不是障碍点 也不在闭合列表里

		if (!inBarrierAndCloseList(x, y))
		{
			std::string k = std::to_string(x) + "_" + std::to_string(y);
			OpenPoint *toCreate = new OpenPoint(x, y);
			//消耗值+14 并且标记父节点为pointToOpen
			toCreate->calulatePoint(end, pointToOpen.cost + 14, &pointToOpen);
			std::map<std::string, OpenPoint*>::iterator iter = m_openComPareList.find(k);
			if (iter == m_openComPareList.end())
			{
				//如果也不在更新列表，直接加入open列表
				openList.push(toCreate);
				updateOpenlist(toCreate, true);
			}
			else
			{
				//如果已经在open列表，需要比较看是否需要更新父节点
				OpenPoint *p = m_openComPareList[k];
				int predOld = p->pred;
				if (predOld > toCreate->pred)
				{
					//如果原有的预测值大于当前的预测值 则更新
					p->cost = pointToOpen.cost + 14;
					p->pred = toCreate->pred;
					p->father = &pointToOpen;
					
				}

			}

		}
		
	}
	

	
}

//开始搜索路径
OpenPoint* AStarScene::findway(OpenPoint& start, const OpenPoint& end)
{
	m_deepth = 0;
	// 创建并开启一个父节点
	// 把起始点加入开启列表

	OpenPoint *s1 = new OpenPoint(start.x, start.y);
	s1->calulatePoint(end,0,nullptr);
	
	openList.push(s1);
	updateOpenlist(s1, true);

	//标记起始点不在列表里
	m_closeAndBarrierList[std::to_string(start.x) + "_" + std::to_string(start.y)] = false;

	// 重复寻找预测和花费之和最小节点开启检查
	while (!openList.empty())
	{
		OpenPoint *toOpen = openList.top(); //openList为优先队列，按照自定义方法排序，首位一定是最小的

		// 找到终点后返还end点
		if (toOpen->x == end.x && toOpen->y == end.y)
		{
			return toOpen->father;
		}
		else if (m_deepth >=1000)
		{
			//若超出一定深度(1000深度)，则失败
			return nullptr;
		}

		open(*toOpen, end);
	}

	return nullptr;

}

void AStarScene::updateOpenlist(OpenPoint *p, bool isAdd)
{
	int x = p->x;
	int y = p->y;
	std::string k = std::to_string(x) + "_" + std::to_string(y);
	if (isAdd)
	{
		m_openComPareList[k] = p;
	}
	else
	{
		std::map<std::string, OpenPoint*>::iterator key = m_openComPareList.find(k);
		m_openComPareList.erase(key);
	}
	
	
}

AStarScene::~AStarScene()
{
	m_closeAndBarrierList.clear();

	std::map<std::string, OpenPoint*>::iterator it;
	for (it = m_openComPareList.begin(); it != m_openComPareList.end();it++)
	{
		if (it->second)
		{
			delete(it->second);
			it->second = nullptr;
		}
		
	}
	m_openComPareList.clear();

	while (!openList.empty()){
		openList.pop();
	}

}

