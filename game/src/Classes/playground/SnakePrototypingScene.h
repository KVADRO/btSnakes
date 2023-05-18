#pragma once

#include "Config.h"
#include "playground/Common.h"

NS_CC_BEGIN

struct SnakeNode
{
    Sprite* visual{nullptr};
};

struct Snake
{
public:
    SnakeNode& pushNode()
    {
        m_Nodes.emplace_back();
        
        SnakeNode& node = m_Nodes.back();
        node.visual = Sprite::create("circle_64x64.png");
        
        const Vec2 halfVisualSize = node.visual->getContentSize() * 0.5f;
        
        DrawNode* debugView = DrawNode::create();
        debugView->drawLine(halfVisualSize, halfVisualSize + GetBaseHeading() * halfVisualSize.y * 1.3f, Color4F::GREEN);
        node.visual->addChild(debugView);

        return node;
    }
    
    std::vector<SnakeNode>& getAllNodes() { return m_Nodes; }
    
    Vec2 getHeading() const
    {
        Vec2 result = common::RotateVector(GetBaseHeading(), m_Nodes[0].visual->getRotation());
        result.normalize();
        
        return result;
    }
    
    void setHeading(float _heading)
    {
        m_Nodes[0].visual->setRotation(_heading);
    }
    
    void setHeading(const Vec2& _heading)
    {
        m_Nodes[0].visual->setRotation(common::Get360Angle(GetBaseHeading(), _heading));
    }
    
    static const cocos2d::Vec2 GetBaseHeading()
    {
        return cocos2d::Vec2::UNIT_Y;
    }
    
private:
    std::vector<SnakeNode> m_Nodes;
};

class SnakePrototypingScene : public Scene
{
    using Parent_t = Scene;
    
    struct DragInfo
    {
        Vec2 heading;
        Vec2 mousePos;
        
        int touchID{-1};
        
        void activate(int _touchID)
        {
            touchID = _touchID;
        }
        
        void deactivate()
        {
            mousePos = Vec2::ZERO;
            heading = Vec2::ZERO;
            touchID = -1;
        }
        
        bool isActive() const { return touchID != -1; }
    };
    
public:
    ~SnakePrototypingScene();
    
private:
    bool init() override;
    void update(float _delta) override;
    
private:
    DragInfo m_DragInfo;
    Snake m_Snake;
    
    EventListenerTouchOneByOne* m_TouchListener{nullptr};
    DrawNode* m_DebugOverlay{nullptr};
    
    const float BaseMovementSpeed = 70.0f;
    const float BaseRotationSpeed = 50.0f;
    const float BaseNodesOffset = 10.0f;
    
public:
    CREATE_FUNC(SnakePrototypingScene)
};

NS_CC_END
