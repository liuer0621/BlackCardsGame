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

class Card : public Sprite
{
    private:
        EventListenerTouchOneByOne *listener;
        bool mMoving;
        Vec2 _targetPosition;
    
    public:
        Card(void);
        virtual ~Card(void);
    
        static Card* create(const std::string &filename);
        virtual bool init(const std::string &filename);

        void setTargetPosition(Vec2 targetPosition);
        bool submitCard();
    
        bool onTouchBegan(Touch*, Event*);
        void onTouchMoved(Touch*, Event*);
        void onTouchEnded(Touch*, Event*);
    
};




#endif /* defined(__cards__Card__) */
