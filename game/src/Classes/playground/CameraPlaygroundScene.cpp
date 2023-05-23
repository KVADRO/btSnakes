#include "playground/CameraPlaygroundScene.h"

using namespace cocos2d;

const std::string CameraPlaygroundScene::SCENE_NAME = "CameraPlaygroundScene";

bool CameraPlaygroundScene::init()
{
    if(!Parent_t::init())
    {
        return false;
    }

    for(int i = 0; i < m_Boundaries.size(); ++i)
    {
        m_Boundaries[i] = Sprite::create("square_64x64.png");
        addChild(m_Boundaries[i]);
    }
    
    const Vec2 viewSize = Director::getInstance()->getOpenGLView()->getVisibleSize();
    m_Boundaries[0]->setPosition(Vec2{viewSize.x * 0.5f, 0.0f});
    m_Boundaries[1]->setPosition(Vec2{viewSize.x * 0.5f, viewSize.y});
    m_Boundaries[2]->setPosition(Vec2{0.0, viewSize.y * 0.5f});
    m_Boundaries[3]->setPosition(Vec2{viewSize.x, viewSize.y * 0.5f});
  
#ifdef USE_IMGUI
    CCIMGUI::getInstance()->addCallback([this]() {
        if(ImGui::Begin(SCENE_NAME.c_str()))
        {
            const Vec2 viewSize = Director::getInstance()->getOpenGLView()->getVisibleSize();
            
            const float posXMin = -viewSize.x;
            const float posXMax = viewSize.x * 2.0f;
            
            const float posYMin = -viewSize.y;
            const float posYMax = viewSize.y * 2.0f;
            
#define MODIFIER_IMPL(INDEX) \
            { \
                ImGui::SeparatorText(#INDEX); \
 \
                static float x = m_Boundaries[INDEX]->getPositionX(); \
                static float y = m_Boundaries[INDEX]->getPositionY(); \
 \
                const std::string xName = std::to_string(INDEX) + " (X)"; \
                ImGui::SliderScalar(xName.c_str(), ImGuiDataType_Float, &x, &posXMin, &posXMax); \
                m_Boundaries[INDEX]->setPositionX(x); \
 \
                const std::string yName = std::to_string(INDEX) + " (Y)"; \
                ImGui::SliderScalar(yName.c_str(), ImGuiDataType_Float, &y, &posYMin, &posYMax); \
                m_Boundaries[INDEX]->setPositionY(y); \
            }
            
            MODIFIER_IMPL(0)
            MODIFIER_IMPL(1)
            MODIFIER_IMPL(2)
            MODIFIER_IMPL(3)
            
            ImGui::End();
        }
    }, SCENE_NAME);
#endif
    
    scheduleUpdate();
    return true;
}

void CameraPlaygroundScene::cleanup()
{
    Parent_t::cleanup();
    
#ifdef USE_IMGUI
    CCIMGUI::getInstance()->removeCallback(SCENE_NAME);
#endif
}

void CameraPlaygroundScene::update(float _delta)
{
    Parent_t::update(_delta);
}

float CameraPlaygroundScene::getAvg(const std::vector<float>& _values) const
{
    float summ = 0.0f;
    for(float value : _values)
    {
        summ += value;
    }
    
    return summ / static_cast<float>(_values.size());
}
