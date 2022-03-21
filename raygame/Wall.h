#pragma once
#include "Actor.h"
class Wall :
    public Actor
{
public:
    Wall(float x, float y);
    void draw() override;
};

