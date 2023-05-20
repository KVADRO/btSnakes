#pragma once

#include "management/Manager.h"

NS_CC_BEGIN

class SceneManager : public Manager
{
public:
    static const std::string MANAGER_ID;
    
public:
    bool init();
    
    template<typename T, typename... ARGS>
    T* openScene(ARGS&&... _args)
    {
        T* scene = T::create(std::forward<ARGS>(_args)...);
        if(!scene)
        {
            CCLOG("Unnable to create %s scene", T::SCENE_NAME.c_str());
            return nullptr;
        }
        
        scene->setName(T::SCENE_NAME);
        Director::getInstance()->replaceScene(scene);
        
        return scene;
    }
    
    Scene* openSceneFromFactory(const std::string& _id);
    
    std::string getRunningSceneID() const;
    std::vector<std::string> getAllSceneFactories() const;
    
private:
#ifdef USE_IMGUI
    void updateImGui() override;
#endif
    
private:
    std::map<std::string, std::function<Scene*()>> m_Factory;
};

NS_CC_END
