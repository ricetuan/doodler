//
//  SceneManager.cpp
//  Doodler
//
//  Created by Liang Fan on 6/30/15.
//
//

#include "SceneManager.h"
#include "DrawingCanvas.h"
#include "JSONPacker.h"

using namespace cocos2d;

static SceneManager* sharedSceneManager = nullptr;

SceneManager* SceneManager::getInstance()
{
    if (! sharedSceneManager)
    {
        sharedSceneManager = new SceneManager();
    }
    
    return sharedSceneManager;
}

SceneManager::SceneManager()
{
    drawingCanvas = nullptr;
    networkingWrapper = new NetworkingWrapper();
    networkingWrapper->setDelegate(this);
}

SceneManager::~SceneManager()
{
    delete networkingWrapper;
}

void SceneManager::enterSoloGame()
{
    this->loadDrawingScene(false);
}

void SceneManager::returnToLobby()
{
    if (drawingCanvas) {
        Director::getInstance()->popScene();
        drawingCanvas = nullptr;
    }
   
}

void SceneManager::connectAndEnterNetworkedGame()
{
    networkingWrapper->attemptToJoinGame();
}

void SceneManager::loadDrawingScene(bool networked)
{
    auto scene = Scene::create();
    
    drawingCanvas = DrawingCanvas::create();
    drawingCanvas->setNetworkedSession(networked);
    
    scene->addChild(drawingCanvas);
    
    Director::getInstance()->pushScene(scene);
}


void SceneManager::receivedData(const void* data, unsigned long length)
{
    if (drawingCanvas)
    {
        drawingCanvas->receivedData(data, length);
    }
}

void SceneManager::stateChanged(ConnectionState state)
{
    switch (state) {
        case ConnectionState::CONNECTED:
        {
            CCLOG("CONNECTED");
            if (!drawingCanvas)
            {
                this->loadDrawingScene(true);
            }
        }
            break;
        case ConnectionState::CONNECTING:
            CCLOG("CONNECTING");
            break;
        case ConnectionState::NOT_CONNECTED:
            CCLOG("NOT_CONNECTED");
            break;
    }
}

void SceneManager::sendData(const void* data, unsigned long length)
{
    this->networkingWrapper->sendData(data, length);
}


