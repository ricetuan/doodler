//
//  Lobby.h
//  Doodler
//
//  Created by Liang Fan on 6/27/15.
//
//

#ifndef __Doodler__Lobby__
#define __Doodler__Lobby__

#include "CocosGUI.h"

class Lobby : public cocos2d::Node
{
public:
    CREATE_FUNC(Lobby);
    
protected:
    bool init() override;
    void onEnter() override;
    void setupUI();
    cocos2d::LayerColor* background;
    
    void soloPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);
    void duoPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType);

    
};


#endif /* defined(__Doodler__Lobby__) */
