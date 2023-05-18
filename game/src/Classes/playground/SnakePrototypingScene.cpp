#include "SnakePrototypingScene.h"

using namespace cocos2d;

SnakePrototypingScene::~SnakePrototypingScene()
{
    if(m_TouchListener)
    {
        getEventDispatcher()->removeEventListener(m_TouchListener);
    }
}

bool SnakePrototypingScene::init()
{
    if(!Parent_t::init())
    {
        return false;
    }
    
    const int InitialNodesCount = 5;
    for(int i = 0; i <= InitialNodesCount; ++i)
    {
        SnakeNode& node = m_Snake.pushNode();
        addChild(node.visual);
    }
    
    m_Snake.setHeading(common::Get360Angle(Snake::GetBaseHeading(), Vec2{-1.0f, 0.0f}));
    SnakeNode& head = m_Snake.getAllNodes()[0];
    
    // Align
    const Vec2 screenCenter = Director::getInstance()->getOpenGLView()->getVisibleSize() * 0.5f;
    const Vec2 buildDirection = m_Snake.getHeading() * -1.0f;
    
    Vec2 position = screenCenter;
    for(SnakeNode& node : m_Snake.getAllNodes())
    {
        node.visual->setPosition(position);
        
        Size nodeSize = node.visual->getContentSize();
        position += buildDirection * (nodeSize.height + BaseNodesOffset); // Considering that all node textures are squares, we don't care what dimension to take
    }
    
    m_TouchListener = EventListenerTouchOneByOne::create();
    m_TouchListener->onTouchBegan = [this](Touch* _touch, Event* _event)
    {
        if(m_DragInfo.isActive())
            return false;
        
        m_DragInfo.activate(_touch->getID());
        m_DragInfo.mousePos = _touch->getLocation();
        m_DragInfo.heading = _touch->getLocation() - Director::getInstance()->getOpenGLView()->getVisibleSize() * 0.5f;
        m_DragInfo.heading.normalize();
        
        return true;
    };
    
    m_TouchListener->onTouchMoved = [this](Touch* _touch, Event* _event)
    {
        if(!m_DragInfo.isActive() || (_touch->getID() != m_DragInfo.touchID))
            return;
        
        m_DragInfo.mousePos = _touch->getLocation();
        m_DragInfo.heading = _touch->getLocation() - Director::getInstance()->getOpenGLView()->getVisibleSize() * 0.5f;
        m_DragInfo.heading.normalize();
    };
    
    m_TouchListener->onTouchEnded = m_TouchListener->onTouchCancelled = [this](Touch* _touch, Event* _event)
    {
        if(!m_DragInfo.isActive() || (_touch->getID() != m_DragInfo.touchID))
            return;
        
        m_DragInfo.deactivate();
    };
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_TouchListener, this);
    
    m_DebugOverlay = DrawNode::create();
    addChild(m_DebugOverlay);
    
    scheduleUpdate();
    return true;
}

void SnakePrototypingScene::update(float _delta)
{
    Parent_t::update(_delta);
    
    m_DebugOverlay->clear();
    if(m_DragInfo.isActive())
    {
        const Vec2 halfVisibleSize = Director::getInstance()->getOpenGLView()->getVisibleSize() * 0.5f;
        m_DebugOverlay->drawLine(halfVisibleSize, m_DragInfo.mousePos, Color4F::RED);
    }
    
    // Update head
    
    SnakeNode& headNode = m_Snake.getAllNodes()[0];
    if(m_DragInfo.isActive())
    {
        const Vec2 viewCenter = Director::getInstance()->getOpenGLView()->getVisibleSize() * 0.5f;
        const Vec2 mouseHeading = m_DragInfo.mousePos - viewCenter;
        
        const float targetingDelta = common::GetShortestAngle(m_Snake.getHeading(), mouseHeading);
        const float targetingDeltaAbs = std::abs(targetingDelta);
        
        if(targetingDeltaAbs > FLOATING_POINT_TOLLERANCE)
        {
            float angularDisplacement = BaseRotationSpeed * _delta;
            if(angularDisplacement > targetingDeltaAbs)
            {
                angularDisplacement = targetingDeltaAbs;
            }
            
            if(targetingDelta < 0.0f)
            {
                angularDisplacement *= -1.0f;
            }
            
            Vec2 newHeading = common::RotateVector(m_Snake.getHeading(), angularDisplacement);
            newHeading.normalize();
            
            m_Snake.setHeading(newHeading);
        }
    }

    headNode.visual->setPosition(headNode.visual->getPosition() + m_Snake.getHeading() * BaseMovementSpeed * _delta);
    
    // Update nodes
    std::vector<SnakeNode>& nodes = m_Snake.getAllNodes();
    
    for(int i = 1; i < nodes.size(); ++i)
    {
        SnakeNode& node = nodes[i];
        SnakeNode& prevNode = nodes[i - 1];
        
        Vec2 displacementDirection = node.visual->getPosition() - prevNode.visual->getPosition();
        displacementDirection.normalize();
        
        Vec2 pos = prevNode.visual->getPosition() + displacementDirection * (prevNode.visual->getContentSize().height + BaseNodesOffset);
        node.visual->setPosition(pos);
    }
}
