#include "DevilLayer.h"



DevilLayer::DevilLayer()
	: m_devil(NULL)
	, m_fightingVal(0.0f)
	, m_fightingMoved(0.0f)
	, m_winSize(0,0)
	, m_progress(NULL)
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


		this->schedule( schedule_selector(DevilLayer::updateDevil), 3.0 );

		CCNotificationCenter::sharedNotificationCenter()->addObserver(
			this,
			callfuncO_selector(DevilLayer::devilFighting),
			MsgTypeForObserver::c_DevilFightingStart,
			NULL);

		this->setTouchEnabled(true);
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

void DevilLayer::registerWithTouchDispatcher()
{
	// higher priority than dragging
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -9, true);
}

bool DevilLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	if (m_progress == NULL)
	{
		return false;
	}
	return true;
}

void DevilLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint start = touch->getStartLocation();

	CCPoint afterStart = touch->getLocation();
	if ( abs(afterStart.x - m_fightingMoved) > 10.0f)
	{
		m_fightingVal = 0.1f;
	}
	else
	{
		m_fightingVal = (m_progress !=NULL && (m_progress->getPercentage() <= 80.0f)) ? -0.08f : -0.18f;
		CCLOG("m_fightingVal  %f", m_fightingVal);
	}
	m_fightingMoved = afterStart.x;
}

void DevilLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	m_fightingVal = -0.1f;
}

/*private function*/
void DevilLayer::initDevil()
{
	m_devil = Devil::create();
	m_devil->bindSprite(CCSprite::create("Gui/Login_1/Object.png"));
	m_devil->setPosition(ccp(m_winSize.width/2, m_winSize.height/2));
	this->addChild(m_devil);
}


void DevilLayer::devilFighting(CCObject* pData)
{
	m_devil->setVisible(false);
	CCSprite* fightingProgressBg = CCSprite::create("slider_bar.png");
	fightingProgressBg->setPosition(ccp(150,300));
	this->addChild(fightingProgressBg);

	CCSprite* fightingProgress = CCSprite::create("silder_progressBar.png");
	m_progress = CCProgressTimer::create(fightingProgress);
	m_progress->setType(kCCProgressTimerTypeBar);
	m_progress->setPosition(ccp(150,300));
	m_progress->setMidpoint(ccp(0,0));
	m_progress->setBarChangeRate(ccp(1,0));
	m_progress->setPercentage(50);
	this->addChild(m_progress);
	this->schedule( schedule_selector(DevilLayer::updateFightingBar) );
}


void DevilLayer::updateFightingBar(float dt)
{
	float proPercet = m_progress->getPercentage();
	if (proPercet == 0.0f)
	{
		this->unschedule( schedule_selector(DevilLayer::updateDevil) );
		this->unschedule( schedule_selector(DevilLayer::updateFightingBar) );
		this->removeChild(m_progress);
		this->removeChild(m_devil);
		this->setTouchEnabled(false);
		CCNotificationCenter::sharedNotificationCenter()->postNotification(MsgTypeForObserver::c_DevilFightingStop, NULL);
	}
	else
	{
		m_progress->setPercentage(proPercet+m_fightingVal);
	}
}