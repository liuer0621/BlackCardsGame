//
//  Card.h
//  cards
//
//  Created by Lana Sun on 12/30/14.
//
//

#ifndef __cards__Card__
#define __cards__Card__
#define LOCATION_ACCURACY 100


#include "cocos2d.h"
#include <iostream>
USING_NS_CC;

class Card;

class CardDelegate
{
public:
    virtual void cardDidSubmit(Card *card) {}
    virtual bool cardIsMovable(const Card *card) {}
};

class Card : public Sprite
{
private:
    EventListenerTouchOneByOne *listener;
    
    bool mMoving;
    Vec2 mMovingStartPt;
    
    CardDelegate *mDelegate;

public:
    Card(void);
    virtual ~Card(void);

    static Card* create(const std::string &filename);
    static Card* create(Texture2D *texture2D);

    virtual bool init(void);
    virtual bool init(const std::string &filename);
    virtual bool init(Texture2D *texture2D);

    bool submitCard();

    bool onTouchBegan(Touch*, Event*);
    void onTouchMoved(Touch*, Event*);
    void onTouchEnded(Touch*, Event*);
    
    void setDelegate(CardDelegate *delegate) { mDelegate = delegate; }
    
    void fadeOut(void);
    void pullBack(void);
};


#endif /* defined(__cards__Card__) */
