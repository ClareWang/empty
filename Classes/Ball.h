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

	//void update(float dt);

	void moveHorizontal();
	//void fall();
public:
	bool m_isMoving;
	bool m_movingDirection;    /*true means left, false means right*/
	float m_moveSpeed;
	CCSize m_ballSize;
};
#endif // __BALL_H__