#include "playground/SnakePrototypingScene.h"

using namespace cocos2d;

const std::string SnakePrototypingScene::SCENE_NAME = "SnakePrototypingScene";

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
    
    for(int i = 0; i <= 5; ++i)
    {
        m_Snake.nodes.emplace_back();
        
        SnakeNode& node = m_Snake.nodes.back();
        node.visual = Sprite::create("circle_64x64.png");
        node.setHeading(SNAKE_BASE_HEADING);
        
        const Vec2 nodeSize = node.visual->getContentSize();
        
        DrawNode* debugView = DrawNode::create();
        debugView->drawLine(nodeSize * 0.5f, nodeSize * 0.5f + SNAKE_BASE_HEADING * 40.0f, Color4F::GREEN);
        node.visual->addChild(debugView);
        
        addChild(node.visual);
    }
    
    SnakeNode& head = m_Snake.nodes[0];
    
    // Align
    
    const Vec2 screenCenter = Director::getInstance()->getOpenGLView()->getVisibleSize() * 0.5f;
    const Vec2 buildDirection = head.getHeadingDirection() * -1.0f;
    
    Vec2 position = screenCenter;
    for(int i = 0; i < m_Snake.nodes.size(); ++i)
    {
        SnakeNode& node = m_Snake.nodes[i];
        
        node.visual->setPosition(position);
        position += buildDirection * (node.visual->getContentSize().height + SnakeNodesOffset * i);
    }
    
    m_TouchListener = EventListenerTouchOneByOne::create();
    m_TouchListener->onTouchBegan = [this](Touch* _touch, Event* _event)
    {
        if(m_DragInfo.isActive())
            return false;
        
        m_DragInfo.activate(_touch->getID());
        m_DragInfo.touchPosition = _touch->getLocation();
        
        return true;
    };
    
    m_TouchListener->onTouchMoved = [this](Touch* _touch, Event* _event)
    {
        if(!m_DragInfo.isActive() || (_touch->getID() != m_DragInfo.eventID))
            return;
        
        m_DragInfo.touchPosition = _touch->getLocation();
    };
    
    m_TouchListener->onTouchEnded = m_TouchListener->onTouchCancelled = [this](Touch* _touch, Event* _event)
    {
        if(!m_DragInfo.isActive() || (_touch->getID() != m_DragInfo.eventID))
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
    
    const Vec2 viewSize = Director::getInstance()->getOpenGLView()->getVisibleSize();
    const Vec2 viewSizeHalf = viewSize * 0.5f;
    
    m_DebugOverlay->clear();
    if(m_DragInfo.isActive())
    {
        m_DebugOverlay->drawLine(viewSizeHalf, m_DragInfo.touchPosition, Color4F::RED);
    }

    // Update head heading
    
    SnakeNode& headNode = m_Snake.nodes[0];
    if(m_DragInfo.isActive())
    {
        const Vec2 touchHeading = m_DragInfo.touchPosition - viewSizeHalf;
        applyAngularDisplacement(touchHeading, headNode, _delta);
    }

    // Update nodes heading
    for(int i = 1; i < m_Snake.nodes.size(); ++i)
    {
        SnakeNode& node = m_Snake.nodes[i];
        SnakeNode& prevNode = m_Snake.nodes[i - 1];
        
        const Vec2 targetHeading = prevNode.visual->getPosition() - node.visual->getPosition();
        applyAngularDisplacement(targetHeading, node, _delta);
        
        m_DebugOverlay->drawLine(node.visual->getPosition(), prevNode.visual->getPosition(), Color4F::RED);
    }
    
    /* #TODO: nodes with angular velocity
    
     // Move all nodes
    for(SnakeNode& node : m_Snake.nodes)
    {
        node.visual->setPosition(node.visual->getPosition() + node.getHeadingDirection() * SnakeMovementVelocity * _delta);
    }*/
    
    headNode.visual->setPosition(headNode.visual->getPosition() + headNode.getHeadingDirection() * SnakeMovementVelocity * _delta);
    for(int i = 1; i < m_Snake.nodes.size(); ++i)
    {
        SnakeNode& node = m_Snake.nodes[i];
        SnakeNode& prevNode = m_Snake.nodes[i - 1];
        
        Vec2 targetHeading = node.visual->getPosition() - prevNode.visual->getPosition();
        targetHeading.normalize();
        
        node.visual->setPosition(prevNode.visual->getPosition() + targetHeading * node.visual->getContentSize().height);
    }
}

void SnakePrototypingScene::applyAngularDisplacement(const Vec2& _targetHeading, SnakeNode& _node, float _delta)
{
    const Vec2 viewSize = Director::getInstance()->getOpenGLView()->getVisibleSize();
    const Vec2 viewSizeHalf = viewSize * 0.5f;
    
    const float targetingDelta = playground_common::GetShortestAngle(_node.getHeadingDirection(), _targetHeading);
    const float targetingDeltaAbs = std::abs(targetingDelta);
    
    if(targetingDeltaAbs > FLOATING_POINT_TOLLERANCE)
    {
        float angularDisplacement = SnakeAngularVelocity * _delta;
        if(angularDisplacement > targetingDeltaAbs)
        {
            angularDisplacement = targetingDeltaAbs;
        }
        
        if(targetingDelta < 0.0f)
        {
            angularDisplacement *= -1.0f;
        }
        
        _node.setHeading(_node.getHeadingAngle() + angularDisplacement);
    }
}
