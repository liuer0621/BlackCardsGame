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

bool Card::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(this->getBoundingBox().containsPoint(touch->getLocation()))
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
}