#include "PathfindComponent.h"
#include "MazeScene.h"
#include "Transform2D.h"
#include "Agent.h"
#include "MoveComponent.h"
#include "StateMachineComponent.h"

void PathfindComponent::start()
{
	Component::start();
	m_owner = (Agent*)getOwner();
}

void PathfindComponent::update(float deltaTime)
{
	//Don't update if disabled or no target
	if (!m_target)
		return;

	//Find the positions and tiles of the owner and target
	MathLibrary::Vector2 ownerPosition = getOwner()->getTransform()->getWorldPosition();
	MathLibrary::Vector2 destinationPosition = findDestination();
	Maze::Tile ownerTile = m_maze->getTile(ownerPosition);
	Maze::Tile destinationTile = m_maze->getTile(destinationPosition);

	//Update the path if needed
	if (m_needPath)
		updatePath(destinationPosition);

	//Find the position and tile of the next node
	MathLibrary::Vector2 nextPosition = ownerPosition;
	if (!m_path.getLength() <= 0)
		nextPosition = m_path[0]->position;
	Maze::Tile nextTile = m_maze->getTile(nextPosition);

	//If owner is at the front node, go to the following node
	if (ownerTile.x == nextTile.x && ownerTile.y == nextTile.y) {
		if (!m_path.getLength() <= 0)
			m_path.remove(0);
		m_needPath = true;
	}

	//Find the direction
	MathLibrary::Vector2 direction = { 0.0f, 0.0f };
	if (!m_path.getLength() <= 0)
		direction = MathLibrary::Vector2::normalize(m_path[0]->position - ownerPosition);

	//Calculate the force
	MathLibrary::Vector2 desiredVelocity = direction * m_owner->getMaxForce();
	MathLibrary::Vector2 steeringForce = desiredVelocity - m_owner->getMoveComponent()->getVelocity();

	m_owner->getMoveComponent()->setVelocity(steeringForce);
}

void PathfindComponent::draw()
{
	for (int i = 0; i < m_path.getLength(); i++)
	{
		NodeGraph::Node* node = m_path[i];
		NodeGraph::drawNode(node, m_color);
	}
}

void PathfindComponent::updatePath()
{
	updatePath(m_target->getTransform()->getWorldPosition());
}

void PathfindComponent::updatePath(MathLibrary::Vector2 destination)
{
	NodeGraph::Node* ownerNode = m_maze->getTile(getOwner()->getTransform()->getWorldPosition()).node;
	NodeGraph::Node* targetNode = m_maze->getTile(destination).node;
	m_path = NodeGraph::findPath(ownerNode, targetNode);
	//if (!m_path.empty()) m_path.pop_front();
	m_needPath = false;
}

MathLibrary::Vector2 PathfindComponent::findDestination()
{
	StateMachineComponent* state = getOwner()->getComponent<StateMachineComponent>();
	NodeGraph::Node* targetNode = new NodeGraph::Node();
	targetNode->walkable = false;

	//If the current state of the agent is patrol
	if (state->getCurrentState() == PATROL && m_previous)
	{
		//If the path isn't empty, continue to follow the path
		if (m_path.getLength() != 0)
			return m_previous->position;

		//While the target is not a walkable node
		while (!targetNode->walkable)
		{
			//Find a random location
			float randNodeX = rand() % (int)(m_maze->getSize().x * m_maze->TILE_SIZE);
			float randNodeY = rand() % (int)(m_maze->getSize().y * m_maze->TILE_SIZE);

			//Make the target node equal to the node at the random location
			targetNode = m_maze->getTile({ randNodeX, randNodeY }).node;
			targetNode->walkable = m_maze->getTile({ randNodeX, randNodeY }).node->walkable;
		}
	}
	else if(state->getCurrentState() == SEEK || !m_previous)
	{
		targetNode = m_maze->getTile(m_target->getTransform()->getWorldPosition()).node;
	}

	m_previous = targetNode;
	return targetNode->position;
}
