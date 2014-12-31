//
//  Card.h
//  cards
//
//  Created by Lana Sun on 12/30/14.
//
//

#ifndef __cards__Card__
#define __cards__Card__

#include "cocos2d.h"
USING_NS_CC;

class Card : public Sprite
{
    private:
        EventListenerTouchOneByOne *listener;
        bool mMoving;
    
    public:
        Card(void);
        virtual ~Card(void);
    
        static Card* create(const std::string &filename);
        virtual bool init(const std::string &filename);

    
        bool onTouchBegan(Touch*, Event*);
        void onTouchMoved(Touch*, Event*);
        void onTouchEnded(Touch*, Event*);
    
};




#endif /* defined(__cards__Card__) */
