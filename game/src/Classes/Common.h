#pragma once

#include <cocos2d.h>

#define USE_IMGUI

#ifdef USE_IMGUI
#include <CCImGuiLayer.h>
#include <CCIMGUI.h>
#endif

// Custom fp. tollerance with less precision
#define FLOATING_POINT_TOLLERANCE 0.001f

// cc2dx give us deaful macro CREATE_FUNC, but in case if we need arguments in init(...) we'll use this macro
#define VARIADIC_CREATE_FUNC(T) \
template<typename... ARGS> \
static T* create(ARGS&&... _args) \
{ \
    T* scene = new(std::nothrow) T{}; \
    if(scene && scene->init(std::forward<ARGS>(_args)...)) \
    { \
        scene->autorelease(); \
    } \
    else \
    { \
        CC_SAFE_DELETE(scene); \
    } \
 \
    return scene; \
}

NS_CC_BEGIN

namespace common
{
    //#TODO: Return shortest angle between two vectors. CW - positive, CCW - negative [0-180]
    extern float GetShortestAngle(const Vec2& _v1, const Vec2& _v2);

    //#TODO: Return full 360 angle (CW) between two vectors
    extern float Get360Angle(const Vec2& _v1, const Vec2& _v2);

    //#TODO: Rotate vector by specified angle
    extern Vec2 RotateVector(const Vec2& _v, float _angle360);
}

NS_CC_END
