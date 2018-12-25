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