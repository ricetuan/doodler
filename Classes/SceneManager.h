//
//  SceneManager.h
//  Doodler
//
//  Created by Liang Fan on 6/30/15.
//
//

#ifndef __Doodler__SceneManager__
#define __Doodler__SceneManager__

#include "NetworkingWrapper.h"
#include "Compressor.h"
#include "Constants.h"

class DrawingCanvas;
class SceneManager : public NetworkingDelegate
{
public:
    static SceneManager* getInstance();
    void enterSoloGame();
    void returnToLobby();
    void connectAndEnterNetworkedGame();
    void sendDataToBufferPool(LineData lineData);
    void sendData(CanvasAction canvasAction = CanvasAction::None);

    
private:
    void receivedData(const void* data, unsigned long length) override;
    void stateChanged(ConnectionState state) override;
    void loadDrawingScene(bool networked);
    std::string createActionHeader(CanvasAction canvasAction);
    
    //TODO: add some limit of buffer pool
    std::vector<LineData> bufferPool;
    
    DrawingCanvas* drawingCanvas;
    NetworkingWrapper* networkingWrapper;
    Compressor* compressor;
    
    SceneManager();
    ~SceneManager();
};

#endif /* defined(__Doodler__SceneManager__) */
