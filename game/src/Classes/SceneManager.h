#pragma once

#include "Config.h"

NS_CC_BEGIN

class GlobalRegistry
{
public:
    using SceneCreate_t = std::function<Scene*()>;
    
public:
    bool registerSceneFactory(const std::string& _name, const SceneCreate_t& _factory);
    
    template<typename T>
    T* createScene(const std::string& _name)
    {
        auto it = m_SceneFactory.find(_name);
        if(it != m_SceneFactory.end())
        {
            return static_cast<T*>((*it).second());
        }
        
        return nullptr;
    }
    
    std::vector<std::string> getAllSceneFactories() const;
    
#ifdef USE_IMGUI
    void initImGui();
#endif
    
private:
    bool isInitialized() const { return m_Initialized; }
    void setInitialized() { m_Initialized = true; }
    
public:
    static GlobalRegistry& Instance();
    
private:
    bool m_Initialized{false};
    std::map<std::string, SceneCreate_t> m_SceneFactory;
};

NS_CC_END
