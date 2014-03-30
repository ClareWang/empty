#include "LoginLayer.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;


LoginLayer::LoginLayer()
{
}

LoginLayer::~LoginLayer()
{

}

bool LoginLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		CCSize s = CCDirector::sharedDirector()->getWinSize();

		CCLayerRGBA* layerRGBA = CCLayerRGBA::create();
		CCSprite *bg_Sprite = CCSprite::create("Gui/Login_1/Login_BG.png");
		layerRGBA->addChild(bg_Sprite);
		bg_Sprite->setPosition( ccp( s.width/2, s.height/2));
		layerRGBA->runAction(
			CCRepeatForever::create(
			CCSequence::create(
			CCFadeTo::create(4, 0),
			CCFadeTo::create(4, 255),
			CCDelayTime::create(0.5),
			NULL)));

		bg_Sprite->runAction(
			CCRepeatForever::create(
			CCSequence::create(
			CCFadeTo::create(2, 0),
			CCFadeTo::create(2, 255),
			CCFadeTo::create(2, 0),
			CCFadeTo::create(2, 255),
			CCDelayTime::create(1),
			NULL)));
		this->addChild(layerRGBA, 0, 1);
		this->setTouchEnabled(true);



		bRet = true;
	} while (0);

	return bRet;
}


void LoginLayer::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	m_pUILayer = TouchGroup::create();
	m_pUILayer->scheduleUpdate();
	this->addChild(m_pUILayer, 1);

	// register root from json
	m_pLayout = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("Gui/Login_1/Login_1.json"));

	m_pUILayer->addWidget(m_pLayout);
	// background panel
	//Layout* bg_layout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_BG"));

	//login_pLayout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_Login"));
	//login_pLayout->setVisible(false);

	popupInit();

	this->setTouchEnabled(true);
}

void LoginLayer::menuCloseCallback(CCObject* pSender, TouchEventType type)
{
	Layout* login_pLayout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_Login"));
	login_pLayout->setVisible(false);
	//	if (type == TOUCH_EVENT_ENDED)
	//	{
	//		CCDirector::sharedDirector()->end();
	//
	//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//		exit(0);
	//#endif
	//	}
}

void LoginLayer::textFieldEvent(CCObject *pSender, TextFiledEventType type)
{
	switch (type)
	{
	case TEXTFIELD_EVENT_ATTACH_WITH_IME:
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			TextField* textField = dynamic_cast<TextField*>(pSender);

			if ((strcmp(textField->getName(), "Input_Password") == 0) || (strcmp(textField->getName(), "Input_UserAccount") == 0))
			{
				CCMoveBy* moveBy = CCMoveBy::create(0.1f, ccp(0, textField->getContentSize().height * 2.5));
				m_pLayout->runAction(moveBy);
			}
#endif
		}
		break;

	case TEXTFIELD_EVENT_DETACH_WITH_IME:
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			TextField* textField = dynamic_cast<TextField*>(pSender);

			if ((strcmp(textField->getName(), "Input_Password") == 0) || (strcmp(textField->getName(), "Input_UserAccount") == 0))
			{
				CCMoveBy* moveBy = CCMoveBy::create(0.1f, ccp(0, -textField->getContentSize().height * 2.5));
				m_pLayout->runAction(moveBy);
			}
#endif
		}
		break;

	default:
		break;
	}
}

void LoginLayer::popupInit()
{
	Layout* login_pLayout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_Login"));

	TextField* passwd_textfield = static_cast<TextField*>(m_pUILayer->getWidgetByName("Input_Password"));
	passwd_textfield->addEventListenerTextField(this, textfieldeventselector(LoginLayer::textFieldEvent));

	TextField* account_textfield = static_cast<TextField*>(m_pUILayer->getWidgetByName("Input_UserAccount"));
	account_textfield->addEventListenerTextField(this, textfieldeventselector(LoginLayer::textFieldEvent));

	// close button
	Button* close_button = static_cast<Button*>(m_pUILayer->getWidgetByName("Button_Login"));
	close_button->addTouchEventListener(this, toucheventselector(LoginLayer::menuCloseCallback));
}