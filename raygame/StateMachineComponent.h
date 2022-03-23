#pragma once
#include "Component.h"

enum State
{
	WANDER,
	PATROL,
	SEEK
};

class SeekComponent;
class WanderComponent;

class StateMachineComponent :
	public Component
{
public:
	void start() override;

	State getCurrentState() { return m_currentState; }
	void setCurrentState(State state) { m_currentState = state; }

	void update(float deltaTime) override;

private:
	State m_currentState;
	float m_patrolRange = 300;
	float m_patrolTime = 0;
};

