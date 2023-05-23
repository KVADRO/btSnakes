#pragma once

#include "playground/PlaygroundCommon.h"

NS_CC_BEGIN

class CameraPlaygroundScene : public Scene
{
    using Parent_t = Scene;
    
public:
    static const std::string SCENE_NAME;
    VARIADIC_CREATE_FUNC(CameraPlaygroundScene);
    
public:
    bool init() override;
    void cleanup() override;
    void update(float _delta) override;
    
private:
    float getAvg(const std::vector<float>& _values) const;
    
private:
    std::array<Sprite*, 4> m_Boundaries;
};

NS_CC_END
