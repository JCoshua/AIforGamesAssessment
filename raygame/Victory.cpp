#include "Victory.h"
#include "MazeScene.h"
#include "AABBCollider.h"
#include "Transform2D.h"
#include <raylib.h>

Victory::Victory(float x, float y) : Actor(x, y, "Victory")
{
	setStatic(true);
	setCollider(new AABBCollider(Maze::TILE_SIZE / 2, Maze::TILE_SIZE / 2, this));
	getTransform()->setScale({ Maze::TILE_SIZE / 2, Maze::TILE_SIZE / 2 }
	);
}
