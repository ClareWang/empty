#include "DevilLayer.h"



DevilLayer::DevilLayer()
	: m_devil(NULL)
	, m_winSize(0,0)
{
}

DevilLayer::~DevilLayer()
{
}

bool DevilLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		m_winSize = CCDirector::sharedDirector()->getWinSize();
		initDevil();


		schedule( schedule_selector(DevilLayer::updateDevil), 3.0 );
		this->setTouchEnabled(true);

		CCNotificationCenter::sharedNotificationCenter()->addObserver(
			this,
			callfuncO_selector(DevilLayer::devilFighting),
			MsgTypeForObserver::c_DevilFightingStart,
			NULL);

		bRet = true;
	} while (0);

	return bRet;
}

void DevilLayer::updateDevil(float dt)
{
	float rand_x = m_winSize.width*CCRANDOM_0_1();
	float rand_y = 0.0;
	do
	{
		rand_y = m_winSize.height*CCRANDOM_0_1();
	} while (rand_y > 420 || rand_y <420 - m_winSize.width );
	m_devil->setPosition(rand_x, rand_y);
	//m_devil->setPosition(ccp(m_winSize.width/2, m_winSize.height/2));

	CCNotificationCenter::sharedNotificationCenter()->postNotification(MsgTypeForObserver::c_DevilPosUpdate, m_devil);
}


/*private function*/
void DevilLayer::initDevil()
{
	m_devil = Devil::create();
	m_devil->bindSprite(CCSprite::create("Gui/Login_1/Object.png"));
	m_devil->setPosition(ccp(m_winSize.width/2, m_winSize.height/2));
	this->addChild(m_devil);
}


void DevilLayer::devilFighting()
{

}