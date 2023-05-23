#pragma once

#include "Common.h"

NS_CC_BEGIN

class GameScene : public Scene
{
    using Parent_t = Scene;
    
public:
    static const std::string SCENE_NAME;
    
public:
    bool init() override;
    
public:
    VARIADIC_CREATE_FUNC(GameScene);
};

NS_CC_END
