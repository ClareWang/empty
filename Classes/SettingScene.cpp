#include "SettingScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

SettingScene::SettingScene()
{
}

SettingScene::~SettingScene()
{

}

CCScene* SettingScene::scene()
{
	CCScene* scene = CCScene::create();
	SettingScene* layer = SettingScene::create();
	scene->addChild(layer);
	return scene;

}

bool SettingScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		CCSprite *bg_Sprite = CCSprite::create("Gui/Login_1/Object.png");
		bg_Sprite->setPosition( ccp( s.width/2, s.height/2));
		this->addChild(bg_Sprite);

		bRet = true;
	} while (0);

	return bRet;
}