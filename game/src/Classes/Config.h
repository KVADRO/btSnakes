#pragma once

#include <cocos2d.h>

#define USE_IMGUI

#ifdef USE_IMGUI
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
