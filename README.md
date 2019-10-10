# GameAI

简单足球案例启发：分层AI设计
	 {
		1 足球队有自己的状态机==》update里调用队员的update
		2 足球队员有自己状态机
		{
			1 全局状态
			2 各自逻辑状态 ==》 enter excute exit  
			3 消息机制 ==》
			{
				1 队员==》HandleMessage
				2 状态机==》m_pStateMachine->HandleMessage
				3 状态 ==》m_pCurrentState->OnMessage
			}
		}

	 }
	 SoccerPitch --》 球场类，包含2个球门类，2个球队类，1个足球类， 创建窗口的时候就创建球场

--[=====[
	
	主循环
	{
		g_SoccerPitch->Render()

		//update game states
      	g_SoccerPitch->Update();  //会更新render里需要的所有数据
	}


	球队状态--> teamState : 攻击 防守 准备开球
	{
		一般球员状态 	--> FieldPlayerStates  
		{
			GlobalPlayerState     	通用状态
			ChaseBall               追球
			Dribble                 运球
			ReturnToHomeRegion      返回自己区域
			Wait                    等待
			KickBall                踢球
			ReceiveBall             接球
			SupportAttacker         援助攻击队员

			//（状态可以写成单例模式）
			ChaseBall* ChaseBall::Instance()
				{
		  				static ChaseBall instance;

		  				return &instance;
				}

		}

		守门员状态 		--> GoalKeeperStates
		{
			GlobalKeeperState           通用状态
			TendGoal                    朝向目标
			InterceptBall               拦截球
			ReturnHome                  返回自己区域
			PutBallBackInPlay           把球传回到赛场中
		}
	}
	
	球场-->update
	{
		1 球   --> update ：边界判断，更新球位置以及球朝向
		2 球队 --> update
		{
			球队状态机->update  ********
			球员-> update
			{
				一般球员 --> update
				{
					1 一般球员状态机-->update   ******** 也会修改逻辑值
					2 
					{
						计算合力
						修改朝向
						修改速度
						修改位置
					}

				}

				守门员 --> update
				{
					1 守门员状态机-->update  ********
					2 
					{
						计算合力
						计算速度
						计算位置
						更新朝向
					}
				}
			}
		}
	}


	changeState的调用地方--> 1 各种状态的Execute方法里 2 update里 3 消息处理方法里OnMessage

]=====]






一个AI游戏的组成部分
{

	1 游戏类 ==》包含地图环境map   renderUpdate
	2 武器系统
	3 弹药系统
	4 触发器系统==》事件触发
	5 路径规划导航系统 ==》寻路系统
	6 AI角色类 ==》 moveEnity
	7 有限状态机

	游戏类
	{
		包含所有角色的容器==》可以分层标识，环境容器，ai角色容器。。。
		自己有个状态机控制游戏的状态（开始 进行 暂停 结束）
		采用分层状态机思想==》游戏状态机更新 角色状态机更新，其他状态机更新
	}

	AI角色类
	{
		a：继承于一个基类，有一个行为类包含所有行为(靠近，抵达 ，攻击，徘徊。。。。)
		b：有一个状态机的指针，更新状态机==》更新角色的状态 
		c：使用消息机制传递消息参数信息
	}

	导航系统
	{
		利用图这种数据结构组成路点以及边，有一个基本的路点基类和边基类
		利用dijkstra算法和A星算法获得路径==》光滑处理路径（粗略快，精细慢）
		寻路优化：提前存储好路径线路（这个需要好好配合地图路点），时间片段规划（有些方法不必每次都调用）
	}

	触发器系统
	{
		一些特定的节点或者条件满足需要触发一些表现，（寻路点，打死npc，过河变成游泳姿态）
	}

	武器系统
	{
		实现一个基类，所有的武器继承它，每把武器进行虚函数重写进行特质化
	}


	子弹系统
	{
		实现一个基类，所有的子弹继承它，每把武器进行虚函数重写进行特质化
	}

}

------------------------------------------------------------------------------------------

