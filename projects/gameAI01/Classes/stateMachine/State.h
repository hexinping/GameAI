#pragma once

class Miner;
template<class entity_type>
class State
{
public:
	 virtual ~State() {};
	 virtual void Enter(entity_type*) = 0;
	 virtual void Execute(entity_type*) = 0;
	 virtual void Exit(entity_type*) = 0;
};

