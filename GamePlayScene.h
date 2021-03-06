#ifndef __GamePlay_SCENE_H__
#define __GamePlay_SCENE_H__

#include "cocos2d.h"
#include "Card.h"
#include "cocos-ext.h"

#ifdef USE_LOCAL_CCSCROLLVIEW
#include "CCScrollView.h"
#endif

using namespace cocos2d;
using namespace extension;

class GamePlay : public cocos2d::Layer, public ScrollViewDelegate, public CardDelegate
{
private:
    // Constants
    float mCardHeight, mCardScaling;
    float mCardYBase, mCardMaxYOffset;
    float mCardXSpacing;
    float mHalfViewWidth;
    Rect mSubmitRegion;
    
    // States
    bool mCardChosen;
    int mCurrentCardIndex;
    
    // Nodes
    Vector<Card *> WhiteCards;
    ScrollView *scrollView;
    Layer *scrollContainer;
    Layer *BlackCardLayer;
    
    void moveToCard(int index, bool animated);
    void arrangeCards(void);
    
    bool cardsAreDraggable(void);
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GamePlay);
    
    virtual void scrollViewDidScroll(ScrollView * view);
    virtual void scrollViewDidZoom(ScrollView * view){};
    
    void cardDidSubmit(Card *card);
    bool cardIsMovable(const Card *card);
    void cardIsSelected(Card *card);
    
    bool onTouchBegan(Touch *touch, Event *event);
    
    void menuCallBack(Ref *sender);
};

#endif // __GamePlay_SCENE_H__
