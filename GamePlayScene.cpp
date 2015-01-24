#include "GamePlayScene.h"
#include "Card.h"

USING_NS_CC;

float CARDSPACE = 100.0f;
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
    
    // TODO: comment
    const float cardSpacing = CARDSPACE,
                halfViewWidth = visibleSize.width / 2;
    const int numCards = CARDAMOUNT;
    for (int i = 0; i < numCards; i++)
    {
        //add piece
        Card * card = Card::create("whiteBack.png");
        card->setPosition(Vec2(halfViewWidth+cardSpacing*i, visibleSize.height/4));
        //card->setAnchorPoint(Vec2(0.5,1));
        card->setTargetPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.75));
        card->setRotation(5*(i-CARDAMOUNT*0.5));
        this->WhiteCards.pushBack(card);
        scrollContainer->addChild(card);
    }
    
    scrollContainer->setPosition(Point::ZERO);
    Size csize = Size(halfViewWidth*2 + (numCards-1)*cardSpacing, visibleSize.height);
    scrollContainer->setContentSize(csize);
    
    //SETUP SCROLL VIEW
    scrollView = ScrollView::create(visibleSize, scrollContainer);
    // Move to the center card
    const int centerCard = numCards / 2;
    scrollView->setContentOffset(Point(-centerCard * cardSpacing, 0.f));
    scrollView->setPosition(Point::ZERO);
    scrollView->setDirection(ScrollView::Direction::HORIZONTAL);
    scrollView->setDelegate(this);
    
    addChild(scrollView);
    
    //bottom layer
    auto BlackCardLayer = Layer::create();
    BlackCardLayer->setPosition(Point::ZERO);
    BlackCardLayer->setContentSize(visibleSize);
    
    //create label for black card
    auto BlackCardLabel = Label::createWithTTF("Black Card Text", "fonts/Marker Felt.ttf", 32);
    BlackCardLabel->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.75));
    BlackCardLayer->addChild(BlackCardLabel);
    addChild(BlackCardLayer);
    
    arrangeCards();
    
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


void GamePlay::arrangeCards(void)
{
    const Size visibleSize = Director::getInstance()->getVisibleSize();
    const float scrollOffset = scrollView->getContentOffset().x;
    const float screenCenter = visibleSize.width / 2 - scrollOffset;
    
    for (int i = 0; i < WhiteCards.size(); ++i) {
        Card *card = WhiteCards.at(i);
        float cardX = card->getPositionX();
        card->setPositionY(visibleSize.height/4 + std::max(0.f, 200-abs(cardX - screenCenter)*0.5f));
    }
}


void GamePlay::scrollViewDidScroll(ScrollView * view)
{
    arrangeCards();
}
