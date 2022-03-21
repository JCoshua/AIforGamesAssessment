#include "Wall.h"
#include "MazeScene.h"
#include "AABBCollider.h"
#include "Transform2D.h"
#include <raylib.h>

Wall::Wall(float x, float y) : Actor(x, y, "Wall")
{
	setStatic(true);
	setCollider(new AABBCollider(Maze::TILE_SIZE / 2, Maze::TILE_SIZE / 2, this));
	getTransform()->setScale({ Maze::TILE_SIZE / 2, Maze::TILE_SIZE / 2 }
	);
}

void Wall::draw()
{
	Actor::draw();
	DrawRectangle(getTransform()->getWorldPosition().x - Maze::TILE_SIZE / 2, getTransform()->getWorldPosition().y - Maze::TILE_SIZE / 2, Maze::TILE_SIZE, Maze::TILE_SIZE, BLUE);
}