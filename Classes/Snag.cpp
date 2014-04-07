#include "Snag.h"

#define RADIUS 10

Snag::Snag()
	//: m_isCollided(false)
	: m_preBeCollided(NULL)
{
}

Snag::~Snag()
{
}

bool Snag::init()
{
	return true;
}

bool Snag::isCollidedWithBall(Ball* fallball)
{
	if (getSprite() != NULL)
	{
		CCRect entityRrct = fallball->getBoundingBox();
		CCPoint snagPos = getPosition();

		/*if (entityRrct.containsPoint(snagPos))
		{
		return true;
		}*/
		return entityRrct.containsPoint(snagPos);
	}

	return false;
}


//void Snag::setCollided(bool isCollided)
//{
//	this->m_isCollided = isCollided;
//}

void Snag::removePlatBetweenSnags(Ball* fallball, Snag* preSnag)
{
	m_preBeCollided = preSnag;
	if (isCollidedWithBall(fallball))
	{
		if (m_preBeCollided->getSprite() == NULL)
		{
			preSnag = this;
		}
		else
		{
			this->draw();
		}
	}
}

void Snag::draw()
{
	glLineWidth(5);
	CCPoint srcPos = CCPoint(this->getPositionX() - RADIUS, this->getPositionY() + RADIUS);
	CCPoint destPos = CCPoint(this->getPositionX() + RADIUS, this->getPositionY() - RADIUS);

	ccDrawRect(srcPos,destPos);
	glLineWidth(1);
}