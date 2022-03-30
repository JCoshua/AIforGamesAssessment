#include "Ghost.h"
#include "MazeScene.h"
#include "Wall.h"
#include "raylib.h"
#include "Transform2D.h"
#include "PathfindComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "StateMachineComponent.h"
#include "WanderComponent.h"
#include "AABBCollider.h"

Ghost::Ghost(float x, float y, float maxSpeed, float maxForce, int color, Maze* maze)
	: Agent(x, y, "Ghost", maxSpeed, maxForce)
{
	m_maze = maze;
	getTransform()->setScale({ Maze::TILE_SIZE,Maze::TILE_SIZE });

	m_pathfindComponent = new PathfindComponent(maze);
	m_pathfindComponent->setColor(color);

	addComponent(new WanderComponent(100, 5, 0));
	m_stateMachine = addComponent<StateMachineComponent>();
	addComponent(m_pathfindComponent);
	addComponent(new SpriteComponent("Images/enemy.png"));
	setCollider(new AABBCollider(Maze::TILE_SIZE, Maze::TILE_SIZE, this));
}

Ghost::~Ghost()
{
	delete m_pathfindComponent;
}

void Ghost::update(float deltaTime)
{
	Agent::update(deltaTime);

	//Sets the ghost to Wander Behavior
	if (RAYLIB_H::IsKeyPressed(RAYLIB_H::KEY_TAB) && getComponent<PathfindComponent>()->getEnabled() == true)
	{
		getComponent<PathfindComponent>()->setEnabled(false);
		getComponent<WanderComponent>()->setSteeringForce(200);
		m_collidesWithWalls = true;
	}
	//Sets the ghost to PathfindingBehavior
	else if(RAYLIB_H::IsKeyPressed(RAYLIB_H::KEY_TAB) && getComponent<PathfindComponent>()->getEnabled() == false)
	{
		getComponent<PathfindComponent>()->setEnabled(true);
		getComponent<WanderComponent>()->setSteeringForce(0);
		m_collidesWithWalls = false;
	}
}

void Ghost::draw()
{
	Agent::draw();
}

void Ghost::onCollision(Actor* other)
{
	//If the ghost should not collide with walls, do not preform collision
	if (!m_collidesWithWalls)
		return;

	if (Wall* wall = dynamic_cast<Wall*>(other)) {
		MathLibrary::Vector2 halfTile = { Maze::TILE_SIZE / 2.0f, Maze::TILE_SIZE / 2.0f };
		MathLibrary::Vector2 position = getTransform()->getWorldPosition();
		position = position + halfTile;
		MathLibrary::Vector2 tilePosition = {
			roundf(position.x / Maze::TILE_SIZE) * Maze::TILE_SIZE,
			roundf(position.y / Maze::TILE_SIZE) * Maze::TILE_SIZE
		};
		tilePosition = tilePosition - halfTile;
		getTransform()->setWorldPostion(tilePosition);

		getMoveComponent()->setVelocity({ 0, 0 });
	}
}

void Ghost::setTarget(Actor* target)
{
	m_target = target;
	m_pathfindComponent->setTarget(target);
}

Actor* Ghost::getTarget()
{
	return m_target;
}
