#pragma once

#include "management/Manager.h"
#include "management/SceneManager.h"

NS_CC_BEGIN

class ManagerRegistry
{
public:
    ManagerRegistry();

    template<typename T, typename... ARGS>
    T& addManager(ARGS&&... _args)
    {
        if(m_Managers.count(T::MANAGER_ID))
        {
            CCLOG("Manager %s already registered", T::MANAGER_ID.c_str());
            return;
        }
        
        std::unique_ptr<T> manager{new T{}};
        manager->init(std::forward<ARGS>(_args)...);
        
        m_Managers[T::MANAGER_ID] = std::move(manager);
        return static_cast<T&>(*m_Managers[T::MANAGER_ID]);
    }
    
    template<typename T>
    T& getManager()
    {
        auto it = m_Managers.find(T::MANAGER_ID);
        assert((it != m_Managers.end()) && "Unnable to find manager!");
        
        return static_cast<T&>(*(it->second));
    }
    
    void update(float _delta);
    void shutdown();
    
private:
    std::map<std::string, std::unique_ptr<Manager>> m_Managers;
    
public:
    static ManagerRegistry& Instance();
};

NS_CC_END

#define SCENE_MANAGER ManagerRegistry::Instance().getManager<SceneManager>()

