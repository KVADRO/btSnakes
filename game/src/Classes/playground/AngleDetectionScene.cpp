#include "AngleDetectionScene.h"

#ifdef USE_IMGUI
#include <CCIMGUI.h>
#endif

using namespace cocos2d;

AngleDetectionScene::~AngleDetectionScene()
{
    if(m_MouseEventListener)
    {
        getEventDispatcher()->removeEventListener(m_MouseEventListener);
    }
}

bool AngleDetectionScene::init()
{
    if(!Parent_t::init())
        return  false;
    
    m_TargetingVec = BaseHeading;
    
    m_DebugOverlay = DrawNode::create();
    addChild(m_DebugOverlay);

    m_MouseEventListener = EventListenerMouse::create();
    m_MouseEventListener->onMouseMove = [this](EventMouse* _event)
    {
        m_MousePos = _event->getLocationInView();
    };
    
   
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_MouseEventListener, this);
    
#ifdef USE_IMGUI
    CCIMGUI::getInstance()->addCallback([&]()
    {
        const Vec2 viewCenter = Director::getInstance()->getOpenGLView()->getVisibleSize() * 0.5f;
        const Vec2 mouseDisplacement = m_MousePos - viewCenter;
        
        if(ImGui::CollapsingHeader("Mouse"))
        {
            ImGui::Text("Mouse displacement vec: %f / %f", mouseDisplacement.x, mouseDisplacement.y);
            
            const float mouseDisplacementAngle = common::Get360Angle(BaseHeading, mouseDisplacement);
            ImGui::Text("Mouse displacement angle: %f", mouseDisplacementAngle);
            
            const Vec2 mouseDisplacementFromAngle = common::RotateVector(BaseHeading * mouseDisplacement.length(), mouseDisplacementAngle);
            ImGui::Text("Mouse displacement vec from angle: %f / %f", mouseDisplacementFromAngle.x, mouseDisplacementFromAngle.y);
        }
        
        if(ImGui::CollapsingHeader("Targeting"))
        {
            const float targetingDelta = common::GetShortestAngle(m_TargetingVec, mouseDisplacement);
            ImGui::Text("Targeting delta: %f", targetingDelta);
        }
        
    }, "AngleDetectionScene");
#endif
    
    scheduleUpdate();
    return true;
}

void AngleDetectionScene::cleanup()
{
#ifdef USE_IMGUI
    CCIMGUI::getInstance()->removeCallback("AngleDetectionScene");
#endif
}

void AngleDetectionScene::update(float _delta)
{
    Parent_t::update(_delta);
    
    const Vec2 viewCenter = Director::getInstance()->getOpenGLView()->getVisibleSize() * 0.5f;
    const Vec2 mouseHeading = m_MousePos - viewCenter;
    
    const float targetingDelta = common::GetShortestAngle(m_TargetingVec, mouseHeading);
    const float targetingDeltaAbs = std::abs(targetingDelta);
    
    if(targetingDeltaAbs > FLOATING_POINT_TOLLERANCE)
    {
        float angularDisplacement = TargetingSpeed * _delta;
        if(angularDisplacement > targetingDeltaAbs)
        {
            angularDisplacement = targetingDeltaAbs;
        }
        
        if(targetingDelta < 0.0f)
        {
            angularDisplacement *= -1.0f;
        }
        
        m_TargetingVec = common::RotateVector(m_TargetingVec, angularDisplacement);
        m_TargetingVec.normalize();
    }
    
    m_DebugOverlay->clear();
    m_DebugOverlay->drawSolidCircle(viewCenter, 2.0f, 0.0f, 64, 1.0f, 1.0f, Color4F::YELLOW);
    m_DebugOverlay->drawLine(viewCenter, viewCenter + BaseHeading * 40.0f, Color4F::YELLOW); // Base heading
    m_DebugOverlay->drawLine(viewCenter, m_MousePos, Color4F::RED); // Mouse heading
    m_DebugOverlay->drawLine(viewCenter, viewCenter + m_TargetingVec * 100.0f, Color4F::GREEN); // Target heading
}
