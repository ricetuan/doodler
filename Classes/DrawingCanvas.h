//
//  DrawingCanvas.h
//  Doodler
//
//  Created by Liang Fan on 6/27/15.
//
//

#ifndef __Doodler__DrawingCanvas__
#define __Doodler__DrawingCanvas__

#include "CocosGUI.h"
#include "Constants.h"

class DrawingCanvas : public cocos2d::Node
{
public:
    CREATE_FUNC(DrawingCanvas);
    void setNetworkedSession(bool networkedSession);
    bool getNetworkedSession();
    void receivedData(LineData lineData);
protected:
    cocos2d::DrawNode* drawNode;
    cocos2d::LayerColor* background;
    cocos2d::Sprite* check;
    cocos2d::Color4F selectedColor;
    
    bool init() override;
    void onEnter() override;
    
    void sendToBufferPool(cocos2d::Vec2 startPoint, cocos2d::Vec2 endPoint, float radius, cocos2d::Color4F color);
    void sendStrokeOverNetwork();
    void setupTouchHandling();
    void setupMenus();
    
    void clearPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void backPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void colorChangePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
private:
    bool networkedSession;

};

#endif /* defined(__Doodler__DrawingCanvas__) */
