#include "management/ManagerRegistry.h"

using namespace cocos2d;

ManagerRegistry::ManagerRegistry()
{
    addManager<SceneManager>();
    
#ifdef USE_IMGUI
    CCIMGUI::getInstance()->addCallback([this](){
        if(ImGui::Begin("ManagerRegistry"))
        {
            for(auto& manager : m_Managers)
            {
                manager.second->updateImGui();
            }
            
            ImGui::End();
        }
    }, "ManagerRegistry");
#endif
}

ManagerRegistry& ManagerRegistry::Instance()
{
    static ManagerRegistry instance;
    return instance;
}

void ManagerRegistry::update(float _delta)
{
    for(auto& manager : m_Managers)
    {
        manager.second->update(_delta);
    }
}

void ManagerRegistry::shutdown()
{
    for(auto& manager : m_Managers)
    {
        manager.second->shutdown();
    }
    
#ifdef USE_IMGUI
    CCIMGUI::getInstance()->removeCallback("ManagerRegistry");
#endif
}
