#pragma once
#include "Agent.h"
class InputComponent;
class MoveComponent;
class SpriteComponent;
class StateMachineComponent;

class Player :
	public Agent
{
public:
	Player(float x, float y, const char* name, float maxForce, float maxSpeed) : Agent(x, y, name, maxForce, maxSpeed) {}

	void start() override;
	void update(float deltaTime) override;

	void onCollision(Actor* actor) override;

private:
	SpriteComponent* m_spriteComponent;
	StateMachineComponent* m_StateMachine;
	InputComponent* m_input;
};

