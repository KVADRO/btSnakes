#pragma once

#include "Common.h"

NS_CC_BEGIN

class Manager
{
public:
    virtual ~Manager() = default;
    virtual void update(float _delta) {}
    virtual void shutdown() {}
    
#ifdef USE_IMGUI
    virtual void updateImGui() {}
#endif
};

NS_CC_END
