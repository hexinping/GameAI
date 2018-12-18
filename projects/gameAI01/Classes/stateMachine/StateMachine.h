#pragma once
#include "stateMachine/State.h"
#include <cassert>

template<class entity_type>
class StateMachine
{
public:
	StateMachine(entity_type* owner) :m_pOwner(owner),
		m_pCurrentState(nullptr),
		m_pPreviousState(nullptr),
		m_pGlobalState(nullptr)
	{}

	virtual ~StateMachine(){}
	//use these methods to initialize the FSM
	void SetCurrentState(State<entity_type>* s){ m_pCurrentState = s; }
	void SetGlobalState(State<entity_type>* s) { m_pGlobalState = s; }
	void SetPreviousState(State<entity_type>* s){ m_pPreviousState = s; }

	void  RevertToPreviousState()
	{
		ChangeState(m_pPreviousState);
	}

	void  Update(float t)const
	{
		//if a global state exists, call its execute method, else do nothing
		if (m_pGlobalState)   m_pGlobalState->Execute(m_pOwner);

		//same for the current state
		if (m_pCurrentState) m_pCurrentState->Execute(m_pOwner);
	}

	//change to a new state
	void  ChangeState(State<entity_type>* pNewState)
	{
		assert(pNewState &&
			"<StateMachine::ChangeState>: trying to change to NULL state");

		//keep a record of the previous state
		m_pPreviousState = m_pCurrentState;

		//call the exit method of the existing state
		m_pCurrentState->Exit(m_pOwner);

		//change state to the new state
		m_pCurrentState = pNewState;

		//call the entry method of the new state
		m_pCurrentState->Enter(m_pOwner);
	}


	//returns true if the current state's type is equal to the type of the
	//class passed as a parameter. 
	bool  isInState(const State<entity_type>& st)const
	{
		return typeid(*m_pCurrentState) == typeid(st);
	}

	bool  HandleMessage(const Telegram& msg)const
	{
		//first see if the current state is valid and that it can handle
		//the message
		if (m_pCurrentState && m_pCurrentState->OnMessage(m_pOwner, msg))
		{
			return true;
		}

		//if not, and if a global state has been implemented, send 
		//the message to the global state
		if (m_pGlobalState && m_pGlobalState->OnMessage(m_pOwner, msg))
		{
			return true;
		}

		return false;
	}

	State<entity_type>*  CurrentState()  const{ return m_pCurrentState; }
	State<entity_type>*  GlobalState()   const{ return m_pGlobalState; }
	State<entity_type>*  PreviousState() const{ return m_pPreviousState; }

private:
	entity_type* m_pOwner;
	State<entity_type> * m_pCurrentState;
	State<entity_type> * m_pPreviousState;
	State<entity_type> * m_pGlobalState;

};

