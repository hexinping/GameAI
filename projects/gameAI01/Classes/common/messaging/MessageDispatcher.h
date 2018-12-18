#pragma once

#include <set>
#include "common//messaging/Telegram.h"
#include "enity/BaseGameEntity.h"

//to make code easier to read
const double SEND_MSG_IMMEDIATELY = 0.0f;
const int   NO_ADDITIONAL_INFO = 0;

//to make life easier...
#define Dispatch MessageDispatcher::Instance()

class MessageDispatcher
{

private:

	//延迟消息容器
	std::set<Telegram> PriorityQ;

	void Discharge(BaseGameEntity* pReceiver, const Telegram& msg);

	MessageDispatcher(){}

	//copy ctor and assignment should be private
	MessageDispatcher(const MessageDispatcher&);
	MessageDispatcher& operator=(const MessageDispatcher&);

public:

	//this class is a singleton
	static MessageDispatcher* Instance();

	//send a message to another agent. Receiving agent is referenced by ID.
	void DispatchMessages(double  delay,
		int    sender,
		int    receiver,
		int    msg,
		void*  ExtraInfo);

	//send out any delayed messages. This method is called each time through   
	//the main game loop.
	void DispatchDelayedMessages();
};