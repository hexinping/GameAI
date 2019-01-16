#include "AStarScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define random(a,b) (rand()%(b-a+1)+a)

float cellW = 30;
float cellH = 30;

//�ķ���λ�� ֱ������
BasePoint direction[4] = { BasePoint{ 1, 0 }, BasePoint{ 0, 1 }, BasePoint{ -1, 0 }, BasePoint{ 0, -1 } };

//�Ľǵ�λ�� б����
BasePoint corners[4] = { BasePoint{ 1, 1 }, BasePoint{ -1, 1 }, BasePoint{ -1, -1 }, BasePoint{ 1, -1 } };

//�Ƚ������������ȶ��е�ָ�����ͱȽ� ==��open�б������ȶ��б�ʾ���Զ������򷽷�����С�ķ�����λ
struct OpenPointPtrCompare {
	bool operator()(OpenPoint* a, OpenPoint* b){
		return a->pred > b->pred;
	}
};

//ʹ��������ȶ���
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

	//��֤�����յ㶼�����ϰ��� Ҳ���ڱպ��б���
	initMap(startP, endP);

	//���������õ����յ�openpoint��һֱ��·������
	//�������ݾ����ĵ���Ϊ'O'
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



			//���֮һ���������ϰ��������
			if (random(1,5) % 5 == 0)
			{
				float centerx = j*cellW + cellW * 0.5;
				float centery = i*cellH + cellH * 0.5;

				//��ǵ�ͼ����,����ʼ��ͽ�������
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

//�ж��Ƿ��ڱպ��б�����ϰ�����
bool AStarScene::inBarrierAndCloseList(int x,int y)
{
	if (x < 0 || y < 0 || x >= m_row || y >= m_col)
		return true;
	std::string key = std::to_string(x) + "_" + std::to_string(y);
	return m_closeAndBarrierList[key];
}


// ������飬��鸸�ڵ� 
void AStarScene::open(OpenPoint& pointToOpen, const OpenPoint &end)
{
	//ÿ���һ�� ���+1
	m_deepth++;

	//�����ڵ��openlist���Ƴ�
	openList.pop();
	updateOpenlist(&pointToOpen, false);
	m_closeAndBarrierList[std::to_string(pointToOpen.x) + "_" + std::to_string(pointToOpen.y)] = true;

	//���p��ˮƽ����ֱ�ĵ�
	
	for (int i = 0; i < 4;i++)
	{
		
		int x = pointToOpen.x + direction[i].x;
		int y = pointToOpen.y + direction[i].y;
		
		//��������ϰ��� Ҳ���ڱպ��б���
		
		if (!inBarrierAndCloseList(x, y))
		{
			//�ٴ��ж��Ƿ��Ѿ��ڿ����б�����Ѿ���open�б���Ҫ�Ƚϸ���
			std::string k = std::to_string(x) + "_" + std::to_string(y);
			OpenPoint * toCreate = new OpenPoint(x, y);
			//����ֵ+10 ���ұ�Ǹ��ڵ�ΪpointToOpen
			toCreate->calulatePoint(end, pointToOpen.cost + 10, &pointToOpen);
			std::map<std::string, OpenPoint*>::iterator iter = m_openComPareList.find(k); //m_openComPareList[k]��Ȼ�ᴴ��Ҫ��null��ֵ
			if (iter == m_openComPareList.end())
			{
				//���Ҳ���ڸ����б�ֱ�Ӽ���open�б�
				openList.push(toCreate);
				updateOpenlist(toCreate, true);
			}
			else
			{
				//����Ѿ���open�б���Ҫ�ȽϿ��Ƿ���Ҫ���¸��ڵ�
				OpenPoint *p = m_openComPareList[k];
				int predOld = p->pred;
				if (predOld > toCreate->pred)
				{
					//���ԭ�е�Ԥ��ֵ���ڵ�ǰ��Ԥ��ֵ �����
					p->cost = pointToOpen.cost + 10;
					p->pred = toCreate->pred;
					p->father = &pointToOpen;
				}

			}
		
		}
		
	}

	//���p���Ľǵĵ� б�ߵĵ�
	for (int i = 0; i < 4; ++i) {

		int x = pointToOpen.x + corners[i].x;
		int y = pointToOpen.y + corners[i].y;
	
		//��������ϰ��� Ҳ���ڱպ��б���

		if (!inBarrierAndCloseList(x, y))
		{
			std::string k = std::to_string(x) + "_" + std::to_string(y);
			OpenPoint *toCreate = new OpenPoint(x, y);
			//����ֵ+14 ���ұ�Ǹ��ڵ�ΪpointToOpen
			toCreate->calulatePoint(end, pointToOpen.cost + 14, &pointToOpen);
			std::map<std::string, OpenPoint*>::iterator iter = m_openComPareList.find(k);
			if (iter == m_openComPareList.end())
			{
				//���Ҳ���ڸ����б�ֱ�Ӽ���open�б�
				openList.push(toCreate);
				updateOpenlist(toCreate, true);
			}
			else
			{
				//����Ѿ���open�б���Ҫ�ȽϿ��Ƿ���Ҫ���¸��ڵ�
				OpenPoint *p = m_openComPareList[k];
				int predOld = p->pred;
				if (predOld > toCreate->pred)
				{
					//���ԭ�е�Ԥ��ֵ���ڵ�ǰ��Ԥ��ֵ �����
					p->cost = pointToOpen.cost + 14;
					p->pred = toCreate->pred;
					p->father = &pointToOpen;
					
				}

			}

		}
		
	}
	

	
}

//��ʼ����·��
OpenPoint* AStarScene::findway(OpenPoint& start, const OpenPoint& end)
{
	m_deepth = 0;
	// ����������һ�����ڵ�
	// ����ʼ����뿪���б�

	OpenPoint *s1 = new OpenPoint(start.x, start.y);
	s1->calulatePoint(end,0,nullptr);
	
	openList.push(s1);
	updateOpenlist(s1, true);

	//�����ʼ�㲻���б���
	m_closeAndBarrierList[std::to_string(start.x) + "_" + std::to_string(start.y)] = false;

	// �ظ�Ѱ��Ԥ��ͻ���֮����С�ڵ㿪�����
	while (!openList.empty())
	{
		OpenPoint *toOpen = openList.top(); //openListΪ���ȶ��У������Զ��巽��������λһ������С��

		// �ҵ��յ�󷵻�end��
		if (toOpen->x == end.x && toOpen->y == end.y)
		{
			return toOpen->father;
		}
		else if (m_deepth >=1000)
		{
			//������һ�����(1000���)����ʧ��
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

