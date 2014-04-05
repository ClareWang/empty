#ifndef __SNAGFOREST_LAYER_H__
#define __SNAGFOREST_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Ball.h"


struct GameEntry;
class Box2dWorld;
class SnagForestLayer: public cocos2d::CCLayer 
{
public:
	GameEntry*     m_gameEntry;
    Box2dWorld*    m_box2dWorld;
    int            m_entryID;

public:	
	SnagForestLayer();	
	virtual ~SnagForestLayer();	
	//virtual bool init();
	CREATE_FUNC(SnagForestLayer);

	void update(float dt);
    void tick(float dt);
    void draw();

	virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

	bool SnagForestLayer::initWithEntryID(int entryId);

	//virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	//virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
private:
	Ball* m_ball;
	CCSize  m_winSize;
	bool m_isExsist;
};
#endif // __SNAGFOREST_LAYER_H__