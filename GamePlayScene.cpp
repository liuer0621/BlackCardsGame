#include "GamePlayScene.h"
#include "Card.h"

USING_NS_CC;

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
    
    // Load card texture
    // TODO: temporary, as all cards are currently the same
    Image cardImage;
    cardImage.initWithImageFile("whiteBack-withShadow.png");
    Texture2D *cardTexture = new Texture2D;
    cardTexture->autorelease();
    cardTexture->initWithImage(&cardImage);
    
    // Compute constants
    mHalfViewWidth = visibleSize.width * 0.5f;
    mCardHeight = visibleSize.height * 0.5f;
    mCardScaling = mCardHeight / cardTexture->getPixelsHigh();
    mCardMaxYOffset = mCardHeight * 0.7f;
    mCardYBase = mCardHeight * (0.66f - 0.5f);
    mCardXSpacing = visibleSize.width * 0.2f;
    
    // Submit region
    float submitRegionHeight = visibleSize.height - 0.5f * mCardHeight - mCardMaxYOffset - mCardYBase;
    mSubmitRegion.setRect(0, visibleSize.height - submitRegionHeight,
                          visibleSize.width, submitRegionHeight);
    
    // For scroll view
    scrollContainer = Layer::create();
    
    this->WhiteCards = cocos2d::Vector<Card *>{CARDAMOUNT};
    
    // TODO: avoid using MACRO
    const int numCards = CARDAMOUNT;
    for (int i = 0; i < numCards; i++)
    {
        //add piece
        Card * card = Card::create(cardTexture);
        card->setScale(mCardScaling, mCardScaling);
        card->setDelegate(this);
        this->WhiteCards.pushBack(card);
        scrollContainer->addChild(card);
    }
    
    scrollContainer->setPosition(Point::ZERO);
    Size csize = Size(mHalfViewWidth*2 + (numCards-1)*mCardXSpacing, visibleSize.height);
    scrollContainer->setContentSize(csize);
    
    //SETUP SCROLL VIEW
    scrollView = ScrollView::create(visibleSize, scrollContainer);
    moveToCard(numCards/2, false);
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
    BlackCardLabel->setPosition(Vec2(mSubmitRegion.getMidX(), mSubmitRegion.getMidY()));
    BlackCardLayer->addChild(BlackCardLabel);
    addChild(BlackCardLayer);
    
    arrangeCards();
    mSnapToPlace = false;
    mCardChosen = false;
    
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


float unitHeightGaussian(float x, float sigma)
{
    return expf(-x*x/(2*sigma*sigma));
}

void GamePlay::moveToCard(int index, bool animated)
{
    mCurrentCardIndex = index;
    scrollView->setContentOffset(Point(-index * mCardXSpacing, 0.f), animated);
}

void GamePlay::arrangeCards(void)
{
    const float scrollOffset = scrollView->getContentOffset().x;
    // Note scrollOffset is negative
    const float screenCenter = mHalfViewWidth - scrollOffset;
    const float sigma = 0.25f * mCardXSpacing;
    
    for (int i = 0; i < WhiteCards.size(); ++i) {
        Card *card = WhiteCards.at(i);
        float cardX = mHalfViewWidth + mCardXSpacing * i;
        
        // Stop all actions on the card
        card->stopAllActions();
        
        // factor: how close the card is to the center of the screen
        // 1 means right at the center. Closer to 0 when further away
        const float factor = unitHeightGaussian(cardX - screenCenter, sigma);
        
        // Compute Y offset
        const float yOffset = mCardMaxYOffset * factor;
        card->setPosition(Vec2(cardX, mCardYBase + yOffset));
        
        // Compute rotation
        const float rotation = (1-factor) * (5*(i-CARDAMOUNT*0.5f));
        card->setRotation(rotation);
    }
}


void GamePlay::scrollViewDidScroll(ScrollView * view)
{
    if (mSnapToPlace && !view->isDragging()) {
        // When user finishes dragging, and we want to snap the scrollview to predefined places
        Vec2 offset = view->getContentOffset();
        Vec2 maxOffset = view->maxContainerOffset(),
             minOffset = view->minContainerOffset();
        // Check if the current offset is within the min/max boundary; if not, ScrollView will handle that by bounce
        // and we shouldn't do anything
        if (offset.x > minOffset.x && offset.x < maxOffset.x) {
            // Move to the closest card. (Note: offset is negative)
            moveToCard(-floorf(offset.x / mCardXSpacing + 0.5f), true);
        }
        mSnapToPlace = false;
    }
    
    if(view->isDragging() && cardsAreDraggable()) {
        // Whenever the user is dragging, we turn on "snap to place" at the end of draggin
        mSnapToPlace = true;
    }
    
    arrangeCards();
}

void GamePlay::cardDidSubmit(Card *card)
{
    // Note: getPosition() returns position in parent's space, so we use parent's convertToWorldSpace()
    //       to convert it to world space, and then convert it to GamePlay's space
    Vec2 pt = convertToNodeSpace(scrollContainer->convertToWorldSpace(card->getPosition()));
    if (mSubmitRegion.containsPoint(pt)) {
        card->fadeOut();
    } else {
        card->pullBack();
    }
}

bool GamePlay::cardIsMovable(const Card *card)
{
    // TODO: we should make sure card is not movable when there is scrolling animation
    return (card == WhiteCards.at(mCurrentCardIndex));
}

bool GamePlay::cardsAreDraggable(void)
{
    return !mCardChosen;
}