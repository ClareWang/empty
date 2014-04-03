#include "LoginScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

static void setEnableRecursiveCascading(CCNode* node, bool enable)
{
	CCRGBAProtocol* rgba = dynamic_cast<CCRGBAProtocol*>(node);
	if (rgba)
	{
		rgba->setCascadeColorEnabled(enable);
		rgba->setCascadeOpacityEnabled(enable);
	}

	CCObject* obj;
	CCArray* children = node->getChildren();
	CCARRAY_FOREACH(children, obj)
	{
		CCNode* child = (CCNode*)obj;
		setEnableRecursiveCascading(child, enable);
	}
}

LoginScene::LoginScene()
{
}

LoginScene::~LoginScene()
{

}

bool LoginScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCScene::init());
		_loginLayer = LoginLayer::create();
		this->addChild(_loginLayer, 0);

		bRet = true;
	} while (0);

	return bRet;
}