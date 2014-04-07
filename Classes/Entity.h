#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;

class Entity: public cocos2d::CCNode
{
public:	
	Entity();	
	~Entity();
	CCSprite* getSprite();		/*get sprite object*/
	void setSpriteAsNULL();		/*set sprite object*/
	void bindSprite(CCSprite* sprite);		/*bind sprite object*/
private:
	CCSprite* m_sprite;
};
#endif // __ENTITY_H__