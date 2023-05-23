#pragma once

#include "Common.h"

NS_CC_BEGIN

struct SnakeNode
{
    Sprite* visual;
    
    void setHeading(float _angle360)
    {
        visual->setRotation(fmodf(_angle360, 360.0f));
    }
    
    void setHeading(const Vec2& _heading)
    {
        visual->setRotation(common::Get360Angle(SNAKE_BASE_HEADING, _heading));
    }
    
    Vec2 getHeadingDirection() const
    {
        Vec2 result = common::RotateVector(SNAKE_BASE_HEADING, visual->getRotation());
        result.normalize();
        
        return result;
    }
    
    float getHeadingAngle() const
    {
        return visual->getRotation();
    }
};

struct Snake
{
    std::vector<SnakeNode> nodes;
};

NS_CC_END
