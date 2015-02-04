//
//  Card.cpp
//  cards
//
//  Created by Lana Sun on 12/30/14.
//
//

#include "Card.h"

Card::Card(void) : mMoving(false), mDelegate(nullptr)
{
}

Card::~Card(void)
{
}

Card* Card::create(const std::string &filename)
{
    Card* pCard = new Card();
    
    if(pCard && pCard->init(filename))
    {
        pCard->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pCard);
        pCard = NULL;
    }
    
    return pCard;
}

Card* Card::create(Texture2D *texture2D)
{
    Card* pCard = new Card();
    
    if(pCard && pCard->init(texture2D))
    {
        pCard->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pCard);
        pCard = NULL;
    }
    
    return pCard;
}

bool Card::init(void)
{
    listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(Card::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Card::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Card::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // TODO: by default on
    //getTexture()->setAntiAliasTexParameters();
    
    return true;
    
}

bool Card::init(const std::string &filename)
{
    if(!Sprite::initWithFile(filename))
    {
        return false;
    }
    
    return init();
}

bool Card::init(Texture2D *texture2D)
{
    if(!Sprite::initWithTexture(texture2D))
    {
        return false;
    }
    
    return init();
}

// TODO: remove
/*bool Card::submitCard()
{
    Vec2 currentPosition = convertToWorldSpace(this->getPosition());
    // TODO
    //std::cout << "current Position is " << currentPosition.x << "," << currentPosition.y << "\n";
    int deltaY;
    deltaY = _targetPosition.y - currentPosition.y;
    
    if(abs(deltaY) < LOCATION_ACCURACY)
    {
        Action* moveToTargetPosition = MoveTo::create(0.2, _targetPosition);
        this->runAction(moveToTargetPosition);
        // TODO
        //std::cout << "submitted";
        return true;
    }
    
    return false;
 
}*/

bool Card::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Vec2 touchPtInNodeSpace = convertToNodeSpace(touch->getLocation());
    if (touchPtInNodeSpace.x > 0
        && touchPtInNodeSpace.y > 0
        && touchPtInNodeSpace.x < getContentSize().width
        && touchPtInNodeSpace.y < getContentSize().height
        && getNumberOfRunningActions() == 0
        && (!mDelegate || mDelegate->cardIsMovable(this)))
    {
        // TODO
        log("Card::onTouchBegan(): card touched");
        
        mMoving = true;
        mMovingStartPt = getPosition();
        
        if (mDelegate) {
            mDelegate->cardIsSelected(this);
        }
        
        return true;
    }
    
    return false;
}

void Card::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (mMoving) {
        this->setPosition(Vec2(this->getPositionX()+touch->getDelta().x, this->getPositionY()+touch->getDelta().y));
    }
}

void Card::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (mMoving) {
        if (mDelegate) {
            mDelegate->cardDidSubmit(this);
        }
        
        mMoving = false;
    }
}

void Card::fadeOut(void)
{
    runAction(FadeOut::create(1.f));
}

void Card::pullBack(void)
{
    // Note: mMovingStartPt is only meaningful if the card is being moved
    if (mMoving) {
        runAction(MoveTo::create(0.1f, mMovingStartPt));
    }
}