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

//void LoginScene::onEnter()
//{
//	CCScene::onEnter();
//	
//	_loginLayer = LoginLayer::create();
//	
//
//	this->addChild(_loginLayer, 1);
//
//	m_pUILayer = TouchGroup::create();
//	m_pUILayer->scheduleUpdate();
//	addChild(m_pUILayer, 0);
//
//	// register root from json
//	m_pLayout = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile("Gui/Login_1/Login_1.json"));
//
//	m_pUILayer->addWidget(m_pLayout);
//	// background panel
//	//Layout* bg_layout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_BG"));
//
//	//login_pLayout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_Login"));
//	//login_pLayout->setVisible(false);
//
//	popupInit();
//	// Enable cascading in scene
//	//setEnableRecursiveCascading(this, true);
//}

//void LoginScene::menuCloseCallback(CCObject* pSender, TouchEventType type)
//{
//	if (type == TOUCH_EVENT_ENDED)
//	{
//		CCDirector::sharedDirector()->end();
//
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//		exit(0);
//#endif
//	}
//}
//
//void LoginScene::textFieldEvent(CCObject *pSender, TextFiledEventType type)
//{
//	/*Layout* bg_pLayout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_BG"));
//	bg_pLayout->setOpacity(127);*/
//	Layout* login_pLayout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_Login"));
//	login_pLayout->setVisible(true);
//	switch (type)
//	{
//	case TEXTFIELD_EVENT_ATTACH_WITH_IME:
//		{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//			TextField* textField = dynamic_cast<TextField*>(pSender);
//
//			if ((strcmp(textField->getName(), "Input_Password") == 0) || (strcmp(textField->getName(), "Input_UserAccount") == 0))
//			{
//				CCMoveBy* moveBy = CCMoveBy::create(0.1f, ccp(0, textField->getContentSize().height * 2.5));
//				m_pLayout->runAction(moveBy);
//			}
//#endif
//		}
//		break;
//
//	case TEXTFIELD_EVENT_DETACH_WITH_IME:
//		{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//			TextField* textField = dynamic_cast<TextField*>(pSender);
//
//			if ((strcmp(textField->getName(), "Input_Password") == 0) || (strcmp(textField->getName(), "Input_UserAccount") == 0))
//			{
//				CCMoveBy* moveBy = CCMoveBy::create(0.1f, ccp(0, -textField->getContentSize().height * 2.5));
//				m_pLayout->runAction(moveBy);
//			}
//#endif
//		}
//		break;
//
//	default:
//		break;
//	}
//}
//
//void LoginScene::popupInit()
//{
//	Layout* login_pLayout = static_cast<Layout*>(m_pUILayer->getWidgetByName("Panel_Login"));
//	login_pLayout->setVisible(false);
//
//	TextField* passwd_textfield = static_cast<TextField*>(m_pUILayer->getWidgetByName("Input_Password"));
//	passwd_textfield->addEventListenerTextField(this, textfieldeventselector(LoginScene::textFieldEvent));
//
//	TextField* account_textfield = static_cast<TextField*>(m_pUILayer->getWidgetByName("Input_UserAccount"));
//	account_textfield->addEventListenerTextField(this, textfieldeventselector(LoginScene::textFieldEvent));
//
//	// close button
//	Button* close_button = static_cast<Button*>(m_pUILayer->getWidgetByName("Button_Login"));
//	close_button->addTouchEventListener(this, toucheventselector(LoginScene::menuCloseCallback));
//}