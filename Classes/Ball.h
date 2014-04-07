#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Ball: public Entity
{
public:	
	Ball();	
	~Ball();
	virtual bool init();
	CREATE_FUNC(Ball);
	void draw();

	CCRect getBoundingBox(); //obtain collide range

	bool isCollidedWithSprite(CCSprite* sprite);
	void removePlatBetweenSnags(CCSprite* preSprite);


	bool isMoving();
	void setMoving(bool isMoving);
	bool getMovingDirection();
	void setMovingDirection(bool movingDirection);
	float getMovingSpeed();
	void setMovingSpeed(float movingSpeed);
	CCSize getBallSize();
	void setBallSize(CCSize ballSize);

protected:
	bool m_isMoving;
	bool m_movingDirection;    /*true means left, false means right*/
	float m_movingSpeed;
	CCSize m_ballSize;
private:
	CCSprite* m_preBeCollided;
};
#endif // __BALL_H__