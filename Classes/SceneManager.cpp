//
//  SceneManager.cpp
//  Doodler
//
//  Created by Liang Fan on 6/30/15.
//
//

#include "SceneManager.h"
#include "DrawingCanvas.h"
#include "Compressor.h"

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
    compressor = new Compressor();
}

SceneManager::~SceneManager()
{
    delete networkingWrapper;
    delete compressor;
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
    const char* cstr = reinterpret_cast<const char*>(data);
    std::string compressedJson = std::string(cstr, length);
    
    std::vector<LineData> dataList = this->compressor->unCompressData(compressedJson);
    if (drawingCanvas)
    {
        for (auto it = dataList.cbegin(); it != dataList.cend(); ++it)
        {
            this->drawingCanvas->receivedData(*it);
        }
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

void SceneManager::sendData()
{
    std::string data = this->compressor->compressData(this->bufferPool);
    this->bufferPool.clear();
    this->networkingWrapper->sendData(data.c_str(), data.length());
}

void SceneManager::sendDataToBufferPool(LineData lineData)
{
    this->bufferPool.push_back(lineData);
}


