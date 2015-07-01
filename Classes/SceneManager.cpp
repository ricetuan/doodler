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
    std::string compressedData = std::string(cstr, length);
    //if the message contains some action header,
    if (compressedData.substr(0,13) == "ActionHeader:") {
        switch (stoi(compressedData.substr(13,1))) {
            case CanvasAction::Clear:
                this->drawingCanvas->clearCanvas();
                return;
            default:
                break;
        }
    }

    std::vector<LineData> dataList = this->compressor->unCompressData(compressedData);
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
std::string SceneManager::createActionHeader(CanvasAction canvasAction)
{
    return "ActionHeader:"+std::to_string(canvasAction);
}
void SceneManager::sendData(CanvasAction canvasAction)
{
    if (canvasAction != CanvasAction::None) {
        std::string header = createActionHeader(canvasAction);
        this->networkingWrapper->sendData(header.c_str(),header.size());
        return;
    }
    std::string data;
    this->compressor->compressData(this->bufferPool, data);
    this->bufferPool.clear();
    this->networkingWrapper->sendData(data.c_str(), data.length());
}

void SceneManager::sendDataToBufferPool(LineData lineData)
{
    this->bufferPool.push_back(lineData);
}


