#include "SceneManager.h"

#ifdef USE_IMGUI
#include <CCIMGUI.h>
#endif

using namespace cocos2d;

GlobalRegistry& GlobalRegistry::Instance()
{
    static GlobalRegistry instance;
    if(!instance.isInitialized())
    {
#ifdef USE_IMGUI
        instance.initImGui();
#endif
        
        instance.setInitialized();
    }
    
    return instance;
}

bool GlobalRegistry::registerSceneFactory(const std::string& _name, const SceneCreate_t& _factory)
{
    if(m_SceneFactory.find(_name) == m_SceneFactory.end())
    {
        m_SceneFactory[_name] = _factory;
        return true;
    }
    
    return false;
}

std::vector<std::string> GlobalRegistry::getAllSceneFactories() const
{
    std::vector<std::string> result;
    result.reserve(m_SceneFactory.size());
    
    for(const auto& factory : m_SceneFactory)
    {
        result.push_back(factory.first);
    }
    
    return result;
}

#ifdef USE_IMGUI
void GlobalRegistry::initImGui()
{
    CCIMGUI::getInstance()->addCallback([&](){
        /*if(ImGui::CollapsingHeader("GlobalRegistry"))
        {
            
        }*/
        
        //ImGui::ShowDemoWindow();
    }, "GlobalRegistry");
}
#endif
