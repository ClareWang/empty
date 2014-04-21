#ifndef __DEVIL_LAYER_H__
#define __DEVIL_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

#include "Devil.h"
#include "MsgTypeForObserver.h"


class DevilLayer: public cocos2d::CCLayer 
{
public:	
	DevilLayer();	
	virtual ~DevilLayer();	
	virtual bool init();
	CREATE_FUNC(DevilLayer);

	void updateDevil(float dt);
	
	
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);

private:
	void initDevil();
	void devilFighting(CCObject* pData);

private:
	Devil* m_devil;

	
	CCSize  m_winSize;
};
#endif // __DEVIL_LAYER_H__