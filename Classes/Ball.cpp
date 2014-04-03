#include "Ball.h"


Ball::Ball()
	: m_isMoving(false)
	, m_movingDirection(true)
	, m_moveSpeed(1.0)
	//, m_ballSize()
{
	//m_ballSize = NULL;
}

Ball::~Ball()
{
}

bool Ball::init()
{

	return true;
}


//void Ball::update(float dt)
//{
//	moveHorizontal();
//}


void Ball::moveHorizontal()
{
	//if(getSprite() == NULL)
	//{
	//	CCLOG("Ball::move() can not get ball");
	//	return;
	//}
	//if (!m_isMoving)
	//{
	//	m_ballSize = getSprite()->getContentSize();
	//	m_isMoving = true;
	//}
	//if(m_movingDirection)
	//{
	//	m_moveSpeed = ccp(this->getPositionX()-1.0, this->getPositionY());
	//}
	//else
	//{
	//	m_moveSpeed = ccp(this->getPositionX()+1.0, this->getPositionY());
	//}

	//CCPoint expect = this->getPosition() + m_moveSpeed;
	////CCPoint actual = expect;

	//this->setPosition(expect);


	//if(this->getPositionX() == m_ballSize.width/2)
	//{
	//	m_movingDirection = false;
	//}
	//else
	//{
	//	m_movingDirection = true;
	//}
	//this->setZOrder(this->getPositionY()+1);
}