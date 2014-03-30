#ifndef __LOGIN_LAYER_H__
#define __LOGIN_LAYER_H__

#include "cocos2d.h"
#include "cocos-ext.h"

class LoginLayer: public cocos2d::CCLayer 
{
public:	
	LoginLayer();	
	virtual ~LoginLayer();	
	virtual bool init();
	CREATE_FUNC(LoginLayer)	;
	
	void popupInit();

	// a selector callback
	void menuCloseCallback(cocos2d::CCObject* pSender, cocos2d::ui::TouchEventType type);

	void textFieldEvent(cocos2d::CCObject* pSender, cocos2d::ui::TextFiledEventType type);

	
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	//virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	
protected:
	cocos2d::ui::TouchGroup* m_pUILayer;
	cocos2d::ui::Layout* m_pLayout;
};
#endif // __LOGIN_LAYER_H__