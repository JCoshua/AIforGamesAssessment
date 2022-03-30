#include "StateMachineComponent.h"
#include "WanderComponent.h"
#include "PathfindComponent.h"
#include "Actor.h"
#include "Transform2D.h"
#include "AABBCollider.h"
#include "Ghost.h"

void StateMachineComponent::start()
{
	Component::start();
	m_currentState = PATROL;
}

void StateMachineComponent::update(float deltaTime)
{
	Component::update(deltaTime);

	Actor* target = ((Ghost*)(getOwner()))->getTarget();

	MathLibrary::Vector2 direction = target->getTransform()->getWorldPosition() - getOwner()->getTransform()->getWorldPosition();
	float fov = MathLibrary::Vector2::dotProduct(direction.getNormalized(), getOwner()->getTransform()->getForward());

	switch (m_currentState)
	{
	case PATROL:
		if (acos(fov) < 0.4f && direction.getMagnitude() < 130)
			m_currentState = SEEK;

		break;
	case SEEK:
		if (acos(fov) > 0.4f || direction.getMagnitude() > 130)
			m_currentState = PATROL;
		break;
	}
}
