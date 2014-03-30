#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "LoginLayer.h"

class LoginScene: public cocos2d::CCScene
{
public:
	LoginScene();
	virtual ~LoginScene(void);

	virtual bool init();
	CREATE_FUNC(LoginScene);

	CC_SYNTHESIZE(LoginLayer*, _loginLayer, LoginLayer);

};

#endif // __LOGIN_SCENE_H__