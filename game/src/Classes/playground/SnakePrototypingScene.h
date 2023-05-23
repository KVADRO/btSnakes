#pragma once

#include "playground/PlaygroundCommon.h"

NS_CC_BEGIN

#define SNAKE_BASE_HEADING Vec2::UNIT_Y

/*
 Goals:
 - Implement snake movement mechanic
 */

struct SnakeNode
{
    Sprite* visual;
    
    void setHeading(float _angle360)
    {
        visual->setRotation(fmodf(_angle360, 360.0f));
    }
    
    void setHeading(const Vec2& _heading)
    {
        visual->setRotation(playground_common::Get360Angle(SNAKE_BASE_HEADING, _heading));
    }
    
    Vec2 getHeadingDirection() const
    {
        Vec2 result = playground_common::RotateVector(SNAKE_BASE_HEADING, visual->getRotation());
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

class SnakePrototypingScene : public Scene
{
    using Parent_t = Scene;
    
    struct DragInfo
    {
        Vec2 touchPosition;
        int eventID{-1};
        
        void activate(int _eventID)
        {
            eventID = _eventID;
        }
        
        void deactivate()
        {
            touchPosition = Vec2::ZERO;
            eventID = -1;
        }
        
        bool isActive() const { return eventID != -1; }
    };
    
public:
    static const std::string SCENE_NAME;
    
public:
    ~SnakePrototypingScene();
    
private:
    bool init() override;
    void update(float _delta) override;
    
    void applyAngularDisplacement(const Vec2& _targetHeading, SnakeNode& _node, float _delta);
    
private:
    DragInfo m_DragInfo;
    Snake m_Snake;
    
    EventListenerTouchOneByOne* m_TouchListener{nullptr};
    DrawNode* m_DebugOverlay{nullptr};
    
    const float SnakeMovementVelocity = 70.0f;
    const float SnakeAngularVelocity = 50.0f;
    const float SnakeNodesOffset = 30.0f;
    
public:
    VARIADIC_CREATE_FUNC(SnakePrototypingScene)
};

NS_CC_END
