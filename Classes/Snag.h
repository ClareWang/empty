#ifndef __SNAG_H__
#define __SNAG_H__

#include "cocos2d.h"
#include "Entity.h"
#include "Ball.h"

USING_NS_CC;

class Snag: public Entity
{
public:	
	Snag();	
	~Snag();
	virtual bool init();
	CREATE_FUNC(Snag);
	
    void draw();

	void removePlatBetweenSnags(Ball* fallball, Snag* preSnag);

	//void setCollided(bool isCollided);
private:
	bool isCollidedWithBall(Ball* fallball);

private:
	Snag* m_preBeCollided;
};
#endif // __SNAG_H__