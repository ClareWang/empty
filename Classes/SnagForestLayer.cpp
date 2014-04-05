#include "SnagForestLayer.h"
#include "GLES-Render.h"
#include "Box2dWorld.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace ui;

#define kAccelerometerFrequency 30
#define FRAMES_BETWEEN_PRESSES_FOR_DOUBLE_CLICK 10

extern int g_totalEntries;

Settings settings;

SnagForestLayer::SnagForestLayer()
{
}

SnagForestLayer::~SnagForestLayer()
{
	delete m_box2dWorld;
}

//bool SnagForestLayer::init()
//{
//	bool bRet = false;
//	do 
//	{
//		CC_BREAK_IF(!CCLayer::init());
//		m_winSize = CCDirector::sharedDirector()->getWinSize();
//		m_ball = Ball::create();
//		m_ball->bindSprite(CCSprite::create("CloseNormal.png"));
//		m_ball->setPosition( ccp( m_winSize.width/2, m_winSize.height-20.0));
//		m_ball->m_ballSize = m_ball->getSprite()->getContentSize();
//		m_ball->m_isMoving = true;
//		this->addChild(m_ball);
//
//
//		schedule( schedule_selector(SnagForestLayer::tick) );
//
//		this->scheduleUpdate();
//		m_entryID = 0;
//		m_gameEntry = g_gameEntries + m_entryID;
//		m_box2dWorld = m_gameEntry->createFcn();
//
//		this->setTouchEnabled(true);
//		this->autorelease();
//		bRet = true;
//	} while (0);
//
//	return bRet;
//}

bool SnagForestLayer::initWithEntryID(int entryId)
{    


	m_winSize = CCDirector::sharedDirector()->getWinSize();
	m_ball = Ball::create();
	m_ball->bindSprite(CCSprite::create("CloseNormal.png"));
	m_ball->setPosition( ccp( m_winSize.width/2, m_winSize.height-20.0));
	m_ball->m_ballSize = m_ball->getSprite()->getContentSize();
	m_ball->m_isMoving = true;
	this->addChild(m_ball);



	setTouchEnabled( true );
	schedule( schedule_selector(SnagForestLayer::tick) );

	this->scheduleUpdate();
	m_gameEntry = g_gameEntries + entryId;
	m_box2dWorld = m_gameEntry->createFcn(m_winSize.width, m_winSize.height);
	m_isExsist = false;
	return true;
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
		return;
	}
	else
	{
		if(m_ball->m_movingDirection)
		{
			m_ball->m_moveSpeed = m_ball->getPositionX()-1.0;
		}
		else
		{
			m_ball->m_moveSpeed = m_ball->getPositionX()+1.0;
		}

		m_ball->setPositionX(m_ball->m_moveSpeed);


		if(m_ball->getPositionX() <= m_ball->m_ballSize.width/2)
		{
			CCLOG("m_ball->m_movingDirection = false;");
			m_ball->m_movingDirection = false;
		}
		else if (m_ball->getPositionX() >= m_winSize.width - m_ball->m_ballSize.width/2)
		{
			CCLOG("m_ball->m_movingDirection = true;");
			m_ball->m_movingDirection = true;
		}
		//m_ball->setZOrder(m_ball->getPositionY());
	}
}

void SnagForestLayer::tick(float dt)
{
	m_box2dWorld->Step(&settings);
}

void SnagForestLayer::draw()
{
	CCLayer::draw();

	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

	kmGLPushMatrix();

	m_box2dWorld->m_world->DrawDebugData();

	kmGLPopMatrix();

	CHECK_GL_ERROR_DEBUG();
}


void SnagForestLayer::registerWithTouchDispatcher()
{
	// higher priority than dragging
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -10, true);
}

bool SnagForestLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint boxBall = m_ball->getPosition();
	//m_ball->m_isMoving = false;
	//this->setTouchEnabled( false );

	//CCPoint touchLocation = touch->getLocation();    

	//CCPoint nodePosition = convertToNodeSpace( boxBall );
	//    NSLog(@"pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);

	//return m_box2dWorld->MouseDown(b2Vec2(nodePosition.x/PT_RATIO,nodePosition.y/PT_RATIO));
	/*m_gameEntry = g_gameEntries + m_entryID;
	m_box2dWorld = m_gameEntry->createFcn((float32)(boxBall.x/PT_RATIO),(float32)(boxBall.y/PT_RATIO));*/

	if (!m_isExsist)
	{
		CCLOG("SnagForestLayer::ccTouchBegan ");
		b2CircleShape shape1;
		shape1.m_radius = 13.0/PT_RATIO;
		b2FixtureDef fd1;
		fd1.shape = &shape1;
		fd1.density = 1.0f;
		fd1.friction = 0.0f;
		float32 restitution = 0.1f;
		b2BodyDef bd1;
		bd1.type = b2_dynamicBody;
		bd1.position.Set(boxBall.x/PT_RATIO, boxBall.y/PT_RATIO);

		b2Body* body = m_box2dWorld->m_world->CreateBody(&bd1);

		fd1.restitution = restitution;
		body->CreateFixture(&fd1);
		m_isExsist = false;
	}


	return true;

}
void SnagForestLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	//CCPoint location = touch->getLocation();

	//m_ball->setPosition(location);
	//m_ball->m_isMoving = false;

	CCPoint touchLocation = touch->getLocation();
	CCPoint nodePosition = convertToNodeSpace( touchLocation );

	m_box2dWorld->MouseMove(b2Vec2(nodePosition.x/PT_RATIO,nodePosition.y/PT_RATIO));
}
void SnagForestLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	/*CCPoint location = touch->getLocation();

	m_ball->setPosition(location);

	if (m_ball->m_isMoving)
	{

	m_ball->m_isMoving = false;
	}
	else
	{
	m_ball->m_isMoving = true;
	}*/

	CCPoint touchLocation = touch->getLocation();    
	CCPoint nodePosition = convertToNodeSpace( touchLocation );

	m_box2dWorld->MouseUp(b2Vec2(nodePosition.x/PT_RATIO,nodePosition.y/PT_RATIO));
}




//void SnagForestLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
//{
//	//CCTouch* touch = (CCTouch*)pTouches->anyObject();
//	//CCPoint location = touch->getLocation();
//	//
//	//m_ball->setPosition(location);
//	
//	if (m_ball->m_isMoving)
//	{
//		
//		m_ball->m_isMoving = false;
//	}
//	else
//	{
//		m_ball->m_isMoving = true;
//	}
//	
//}
//
//void SnagForestLayer::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
//{
//
//	CCTouch* touch = (CCTouch*)pTouches->anyObject();
//	CCPoint location = touch->getLocation();
//	
//	m_ball->setPosition(location);
//	m_ball->m_isMoving = false;
//}