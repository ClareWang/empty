#include "Ball.h"


Ball::Ball()
	: m_isMoving(false)
	, m_movingDirection(true)
	, m_movingSpeed(1.0)
	, m_preBeCollided(NULL)
{
}

Ball::~Ball()
{
}

bool Ball::init()
{
	return true;
}

bool Ball::isMoving()
{
	return this->m_isMoving;
}
void Ball::setMoving(bool isMoving)
{
	this->m_isMoving = isMoving;
}
bool Ball::getMovingDirection()
{
	return this->m_movingDirection;
}
void Ball::setMovingDirection(bool movingDirection)
{
	this->m_movingDirection = movingDirection;
}
float Ball::getMovingSpeed()
{
	return this->m_movingSpeed;
}
void Ball::setMovingSpeed(float movingSpeed)
{
	this->m_movingSpeed = movingSpeed;
}
CCSize Ball::getBallSize()
{
	return this->m_ballSize;
}
void Ball::setBallSize(CCSize ballSize)
{
	this->m_ballSize = ballSize;
}


CCRect Ball::getBoundingBox()
{
	if(getSprite() == NULL)
	{
		return CCRectMake(0,0,0,0);
	}

	CCPoint entityPos = getPosition();

	return CCRectMake(entityPos.x - m_ballSize.width/2, entityPos.y - m_ballSize.height/2, m_ballSize.width, m_ballSize.height);
}



bool Ball::isCollidedWithSprite(CCSprite* sprite)
{
	if (getSprite() != NULL)
	{
		CCRect entityRrct = this->getBoundingBox();
		CCPoint spritePos = sprite->getPosition();

		/*if (entityRrct.containsPoint(snagPos))
		{
		return true;
		}*/
		return entityRrct.containsPoint(spritePos);
	}

	return false;
}


//void Snag::setCollided(bool isCollided)
//{
//	this->m_isCollided = isCollided;
//}

void Ball::removePlatBetweenSnags(CCSprite* preSprite)
{
	m_preBeCollided = preSprite;
	if (isCollidedWithSprite(preSprite))
	{
		if (m_preBeCollided == NULL)
		{
			preSprite = this;
		}
		else
		{
			this->draw();
		}
	}
}

void Ball::draw()
{
	glLineWidth(5);
	CCPoint srcPos = CCPoint(this->getPositionX() - RADIUS, this->getPositionY() + RADIUS);
	CCPoint destPos = CCPoint(this->getPositionX() + RADIUS, this->getPositionY() - RADIUS);

	ccDrawRect(srcPos,destPos);
	glLineWidth(1);
}