//
//  DrawingCanvas.cpp
//  Doodler
//
//  Created by Liang Fan on 6/27/15.
//
//

#include "DrawingCanvas.h"
#include "Lobby.h"
#include "Constants.h"
#include "SceneManager.h"


using namespace cocos2d;

bool DrawingCanvas::init()
{
    if (! Node::init()) {
        return false;
    }
    drawNode = DrawNode::create();
    background = LayerColor::create(Color4B(COLOR_WHITE));
    selectedColor = COLOR_GREEN;
    
    this->addChild(background);
    this->addChild(drawNode);

    return true;
}

void DrawingCanvas::onEnter()
{
    Node::onEnter();
    setupMenus();
    
    setupTouchHandling();
}
void DrawingCanvas::setupMenus()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->setContentSize(visibleSize);
    drawNode->setContentSize(visibleSize);
    
    ui::Button* clearButton = ui::Button::create();
    clearButton->setAnchorPoint(Vec2(1.0f,1.0f));
    clearButton->setPosition(Vec2(visibleSize.width,visibleSize.height));
    clearButton->loadTextures("clearButton.png", "clearButtonPressed.png");
    clearButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::clearPressed,this));
    this->addChild(clearButton);
    
    ui::Button* backButton = ui::Button::create();
    backButton->setAnchorPoint(Vec2(0.0f,1.0f));
    backButton->setPosition(Vec2(0.0f,visibleSize.height));
    backButton->loadTextures("backButton.png", "backButtonPressed.png");
    backButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::backPressed,this));
    this->addChild(backButton);

    this->check = Sprite::create("checkMark.png");
    this->check->setAnchorPoint(Vec2(0.5f,0.5f));
    this->check->setNormalizedPosition(Vec2(0.5f,0.5f));
    this->check->retain();
    
    Node* colorButtonLayout = Node::create();
    colorButtonLayout->setContentSize(Size(visibleSize.width, visibleSize.height*0.2f));
    colorButtonLayout->setAnchorPoint(Vec2(0.5f, 0.0f));
    colorButtonLayout->setPosition(Vec2(visibleSize.width / 2.0f, 0.0f));
    this->addChild(colorButtonLayout);
    
    std::vector<Color4F> colorList{COLOR_WHITE,COLOR_RED,COLOR_YELLOW,COLOR_GREEN,COLOR_BLUE,COLOR_PURPLE};
    for (int i = 1; i <= 5; ++i)
    {
        ui::Button* colorButton = ui::Button::create();
        colorButton->setAnchorPoint(Vec2(0.5f,0.0f));
        colorButton->setPosition(Vec2(visibleSize.width * i / 6.0f, 0.0f));
        colorButton->loadTextures("colorSwatch.png", "colorSwatch.png");
        colorButton->addTouchEventListener(CC_CALLBACK_2(DrawingCanvas::colorChangePressed,this));
        colorButton->setColor(Color3B(colorList[i]));
        colorButtonLayout->addChild(colorButton);
        
        if (colorList[i] == COLOR_GREEN) {
             colorButton->addChild(this->check);
        }
    }
    
}

void DrawingCanvas::clearPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        drawNode->clear();
        SceneManager::getInstance()->sendData(CanvasAction::Clear);
    }
}
void DrawingCanvas::clearCanvas()
{
    drawNode->clear();
}

void DrawingCanvas::backPressed(Ref *pSender, ui::Widget::TouchEventType eEventType)
{
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        SceneManager::getInstance()->returnToLobby();
    }
    
}
void DrawingCanvas::colorChangePressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eEventType)
{
    ui::Button* pressedButton = static_cast<ui::Button*>(pSender);
    if (eEventType == ui::Widget::TouchEventType::BEGAN)
    {
        pressedButton->setScale(0.85f);
    }
    if (eEventType == ui::Widget::TouchEventType::CANCELED)
    {
        pressedButton->setScale(1.0f);
    }
    
    if (eEventType == ui::Widget::TouchEventType::ENDED)
    {
        selectedColor = Color4F(pressedButton->getColor());
        this->check->removeFromParent();
        pressedButton->addChild(this->check);
        pressedButton->setScale(1.0f);
    }
    
}



void DrawingCanvas::setupTouchHandling()
{
    static Vec2 lastTouchPos;
    static float lastRadius = INITIAL_RADIUS;
    
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        lastTouchPos = drawNode->convertTouchToNodeSpace(touch);
        lastRadius = INITIAL_RADIUS;
        return true;
    };
    
    touchListener->onTouchMoved = [&](Touch* touch, Event* event)
    {

        Vec2 touchPos = drawNode->convertTouchToNodeSpace(touch);
        float distance = lastTouchPos.distance(touchPos);
        
        float dt = 1.0f / 60.0f;
        float rc = 1.0f;
        float alpha = dt / (rc + dt);
        float radius = (alpha * distance) + (1.0f - alpha) * lastRadius;

        CCLOG("Touch Moved! x:%f y:%f", touchPos.x, touchPos.y);
        drawNode->drawSegment(lastTouchPos, touchPos, radius, selectedColor);
        
        if (this->networkedSession)
        {
            this->sendToBufferPool(lastTouchPos, touchPos, radius, selectedColor);
        }
        
        lastRadius = radius;
        lastTouchPos = touchPos;
        //no need to return true
    };
    
    touchListener->onTouchEnded = [&](Touch* touch, Event* event)
    {
        this->sendStrokeOverNetwork();
    };
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void DrawingCanvas::setNetworkedSession(bool networkdSession)
{
    this->networkedSession = networkdSession;
}

bool DrawingCanvas::getNetworkedSession()
{
    return this->networkedSession;
}

void DrawingCanvas::receivedData(LineData lineData)
{
    drawNode->drawSegment(lineData.startPoint, lineData.endPoint, lineData.radius, lineData.color);
}

void DrawingCanvas::sendToBufferPool(cocos2d::Vec2 startPoint, cocos2d::Vec2 endPoint, float radius, cocos2d::Color4F color)
{
    LineData drawingData;
    drawingData.startPoint = startPoint;
    drawingData.endPoint = endPoint;
    drawingData.radius = radius;
    drawingData.color = color;
       
    SceneManager::getInstance()->sendDataToBufferPool(drawingData);
}

void DrawingCanvas::sendStrokeOverNetwork()
{
    SceneManager::getInstance()->sendData();
}


