#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;

class SettingScene: public cocos2d::CCLayer 
{
public:	
	SettingScene();	
	virtual ~SettingScene();

	static CCScene* scene();
	virtual bool init();
	CREATE_FUNC(SettingScene);

	// a selector callback
	//void menuCloseCallback(cocos2d::CCObject* pSender, cocos2d::ui::TouchEventType type);
	//virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	//virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	
protected:
	cocos2d::ui::TouchGroup* m_pUILayer;
	cocos2d::ui::Layout* m_pLayout;
};
#endif // __SETTING_SCENE_H__