#include "management/SceneManager.h"

#include "playground/SnakePrototypingScene.h"
#include "playground/AngleDetectionScene.h"

using namespace cocos2d;

const std::string SceneManager::MANAGER_ID = "SceneManager";

bool SceneManager::init()
{
#define CREATE_WRAPPER(SCENE) []() -> Scene* { return SCENE::create(); }
    
    m_Factory[AngleDetectionScene::SCENE_NAME] = CREATE_WRAPPER(AngleDetectionScene);
    m_Factory[SnakePrototypingScene::SCENE_NAME] = CREATE_WRAPPER(SnakePrototypingScene);
    
#undef CREATE_WRAPPER
}

std::string SceneManager::getRunningSceneID() const
{
    return Director::getInstance()->getRunningScene()->getName();
}

std::vector<std::string> SceneManager::getAllSceneFactories() const
{
    std::vector<std::string> result;
    result.reserve(m_Factory.size());
    
    for(const auto& factory : m_Factory)
    {
        result.push_back(factory.first);
    }
    
    return result;
}

Scene* SceneManager::openSceneFromFactory(const std::string& _id)
{
    auto it = m_Factory.find(_id);
    if(it == m_Factory.end())
    {
        CCLOG("Scene %s doesn't registered in scene factory", _id.c_str());
        return nullptr;
    }
    
    Scene* scene = (*it).second();
    if(!scene)
    {
        CCLOG("Unnable to create %s scene", _id.c_str());
        return nullptr;
    }
    
    scene->setName(_id);
    Director::getInstance()->replaceScene(scene);
    
    return scene;
}

#ifdef USE_IMGUI
void SceneManager::updateImGui()
{
    if(ImGui::CollapsingHeader("Scene Manager"))
    {
        const int ScenesCount = 20;
        
        static const char* names[ScenesCount] = {0};
        static int selected = 0;
        
        std::vector<std::string> factories = getAllSceneFactories();
        for(int i = 0; i < factories.size() && i < ScenesCount; ++i)
        {
            names[i] = factories[i].data();
        }
        
        ImGui::ListBox("Scenes", &selected, names, std::min(ScenesCount, static_cast<int>(factories.size())));
        if(ImGui::Button("Open"))
        {
            openSceneFromFactory(factories[selected]);
        }
    }
}
#endif
