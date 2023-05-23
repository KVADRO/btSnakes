#include "game/GameScene.h"

using namespace cocos2d;

const std::string GameScene::SCENE_NAME = "GameScene";

bool GameScene::init()
{
    if(!Parent_t::init())
    {
        return false;
    }
    
    return true;
}
