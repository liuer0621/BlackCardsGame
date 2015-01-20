//
//  Card.cpp
//  cards
//
//  Created by Lana Sun on 12/30/14.
//
//

#include "Card.h"

Card::Card(void) : mMoving(false)
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

bool Card::init(const std::string &filename)
{
    if(!Sprite::initWithFile(filename))
    {
        return false;
    }
    
    listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = CC_CALLBACK_2(Card::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Card::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Card::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
    
}

void Card::setTargetPosition(Vec2 targetPosition)
{
    this->_targetPosition = targetPosition;
}

bool Card::submitCard()
{
    Vec2 currentPosition = this->getPosition();
    std::cout << "current Position is " << currentPosition.x << "and" << currentPosition.y << "\n";
    int deltaX, deltaY;
    deltaX = _targetPosition.x - currentPosition.x;
    deltaY = _targetPosition.y - currentPosition.y;
    
    if((abs(deltaX) < LOCATION_ACCURACY) && (abs(deltaY) < LOCATION_ACCURACY))
    {
        Action* moveToTargetPosition = MoveTo::create(0.2, _targetPosition);
        this->runAction(moveToTargetPosition);
        std::cout << "submitted";
        return true;
    }
    
    return false;
        
}

bool Card::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    Vec2 touchPtInNodeSpace = convertToNodeSpace(touch->getLocation());
    if (touchPtInNodeSpace.x > 0
        && touchPtInNodeSpace.y > 0
        && touchPtInNodeSpace.x < getContentSize().width
        && touchPtInNodeSpace.y < getContentSize().height)
    {
        this->listener->setSwallowTouches(true);
        mMoving = true;
    }
    else
    {
        this->listener->setSwallowTouches(false);
    }
    
    return true;
}

void Card::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (mMoving) {
        this->setPosition(Vec2(this->getPositionX()+touch->getDelta().x, this->getPositionY()+touch->getDelta().y));
    }
}

void Card::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    mMoving = false;
    this->submitCard();
}