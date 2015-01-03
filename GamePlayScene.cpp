#include "GamePlayScene.h"
#include "Card.h"

USING_NS_CC;

float CARDSPACE = 150.0f;
int CARDAMOUNT = 10;

Scene* GamePlay::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GamePlay::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GamePlay::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //For scroll view
    scrollContainer = Layer::create();
    //scrollContainer->setAnchorPoint(Point::Vec2(-CARDSPACE*CARDAMOUNT/2,0.0f));
    
    this->WhiteCards = cocos2d::Vector<Card *>{CARDAMOUNT};
    
    for (int i = 0; i < CARDAMOUNT; i++)
    {
        //add piece
        Card * card = Card::create("whiteBack.png");
        card->setPosition(Vec2(origin.x+CARDSPACE*(i+0.5), visibleSize.height/4 + origin.y));
        card->setScale(0.1, 0.1);
        //card->setAnchorPoint(Vec2(0.5,1));
        card->setRotation(5*(i-CARDAMOUNT*0.5));
        this->WhiteCards.pushBack(card);
        scrollContainer->addChild(card);
    }
    
    scrollContainer->setPosition(Point::ZERO);
    Size csize = Size(CARDSPACE*CARDAMOUNT, WhiteCards.at(0)->getBoundingBox().size.height);
    scrollContainer->setContentSize(csize);
    
    //SETUP SCROLL VIEW
    scrollView = ScrollView::create(visibleSize,scrollContainer);
    scrollView->setPosition(Point::ZERO);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    
    scrollContainer->retain();
    addChild(scrollView);
    
    return true;
}


void GamePlay::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
