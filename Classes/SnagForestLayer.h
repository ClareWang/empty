#ifndef __SNAGFOREST_LAYER_H__
#define __SNAGFOREST_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Ball.h"


class SnagForestLayer: public cocos2d::CCLayer 
{
public:	
	SnagForestLayer();	
	virtual ~SnagForestLayer();	
	virtual bool init();
	CREATE_FUNC(SnagForestLayer);

	void update(float dt);
	//virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	//virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
private:
	Ball* m_ball;
	CCSize  m_winSize;
};
#endif // __SNAGFOREST_LAYER_H__