#include "SnagForestScene.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

SnagForestScene::SnagForestScene()
{
}

SnagForestScene::~SnagForestScene()
{

}


bool SnagForestScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCScene::init());
		_snagForestLayer = SnagForestLayer::create();
		this->addChild(_snagForestLayer);

		bRet = true;
	} while (0);

	return bRet;
}