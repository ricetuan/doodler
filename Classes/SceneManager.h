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

class DrawingCanvas;
class SceneManager : public NetworkingDelegate
{
public:
    static SceneManager* getInstance();
    void enterSoloGame();
    void returnToLobby();
    void connectAndEnterNetworkedGame();
    void sendData(const void* data, unsigned long length);
    
private:
    void receivedData(const void* data, unsigned long length) override;
    void stateChanged(ConnectionState state) override;
    void loadDrawingScene(bool networked); 
   

    DrawingCanvas* drawingCanvas;
    NetworkingWrapper* networkingWrapper;
    
    SceneManager();
    ~SceneManager();
};

#endif /* defined(__Doodler__SceneManager__) */
