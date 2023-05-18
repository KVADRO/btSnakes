#pragma once

#include "Common.h"

NS_CC_BEGIN

/*
 Goals:
 - Find angle between two vectors
 - Find vector from rotation angle
 - Rotate vector to target angle with speed using shortest path
*/

class AngleDetectionScene : public Scene
{
    using Parent_t = Scene;
    
public:
    ~AngleDetectionScene();
    
private:
    bool init() override;
    void cleanup() override;
    void update(float _delta) override;
    
private:
    const Vec2 BaseHeading = Vec2::UNIT_Y;
    const float TargetingSpeed = 15.0f;
    
    DrawNode* m_DebugOverlay{nullptr};
    EventListenerMouse* m_MouseEventListener{nullptr};

    Vec2 m_MousePos;
    Vec2 m_TargetingVec;
    
public:
    CREATE_FUNC(AngleDetectionScene)
};

NS_CC_END
