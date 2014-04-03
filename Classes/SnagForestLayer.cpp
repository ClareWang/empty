#include "SnagForestLayer.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;


SnagForestLayer::SnagForestLayer()
{
}

SnagForestLayer::~SnagForestLayer()
{
}

bool SnagForestLayer::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		m_winSize = CCDirector::sharedDirector()->getWinSize();
		m_ball = Ball::create();
		m_ball->bindSprite(CCSprite::create("CloseNormal.png"));
		m_ball->setPosition( ccp( m_winSize.width/2, m_winSize.height-20.0));
		this->addChild(m_ball);
		this->setTouchEnabled(true);

		this->scheduleUpdate();

		bRet = true;
	} while (0);

	return bRet;
}


void SnagForestLayer::update(float dt)
{
	if(m_ball == NULL)
	{
		CCLOG("SnagForestLayer::update(float dt) can not get ball");
		return;
	}
	if (!m_ball->m_isMoving)
	{
		m_ball->m_ballSize = m_ball->getSprite()->getContentSize();
		m_ball->m_isMoving = true;
	}
	if(m_ball->m_movingDirection)
	{
		m_ball->m_moveSpeed = m_ball->getPositionX()-1.0;
	}
	else
	{
		m_ball->m_moveSpeed = m_ball->getPositionX()+1.0;
	}

	m_ball->setPositionX(m_ball->m_moveSpeed);


	if(m_ball->getPositionX() == m_ball->m_ballSize.width/2)
	{
		CCLOG("m_ball->m_movingDirection = false;");
		m_ball->m_movingDirection = false;
	}
	else if (m_ball->getPositionX() == m_winSize.width - m_ball->m_ballSize.width/2)
	{
		CCLOG("m_ball->m_movingDirection = true;");
		m_ball->m_movingDirection = true;
	}
	m_ball->setZOrder(m_ball->getPositionY());
}
