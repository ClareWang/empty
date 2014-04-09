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
	: m_randSpeed(1.0)
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

	//initMap();
	initBallLauncher();
	initSnags();

	setTouchEnabled( true );
	schedule( schedule_selector(SnagForestLayer::tick) );
	schedule( schedule_selector(SnagForestLayer::updateRandSpeed), 0.8 );

	this->scheduleUpdate();
	m_gameEntry = g_gameEntries + entryId;
	m_box2dWorld = m_gameEntry->createFcn(m_winSize.width, m_winSize.height);
	return true;
}

void SnagForestLayer::initMap()
{
	CCSprite *bg_Sprite = CCSprite::create("SnagForest_BG.png");
	bg_Sprite->setPosition( ccp( m_winSize.width/2, m_winSize.height/2));
	bg_Sprite->setOpacity(100);
	this->addChild(bg_Sprite, 0, 1);
}

void SnagForestLayer::initBallLauncher()
{
	m_ballLauncher = Ball::create();
	m_ballLauncher->bindSprite(CCSprite::create("CloseNormal.png"));
	m_ballLauncher->setPosition( ccp( m_winSize.width/2, m_winSize.height-20.0));
	m_ballLauncher->setBallSize(m_ballLauncher->getSprite()->getContentSize());
	m_ballLauncher->setMoving(true);
	m_ballLauncher->setMovingDirection(true);
	this->addChild(m_ballLauncher,1);
}

void SnagForestLayer::initSnags()
{
	m_snagArr = CCArray::create();
	m_snagArr->retain();
	CCSpriteBatchNode* snags = CCSpriteBatchNode::create("stock_draw_circle.png");
	this->addChild(snags,2);

	float radius = 5.0f;
	float winX = m_winSize.width -radius*2;
	for (int32 i = 0; i < 7; ++i)
	{
		for(int32 j = 0; j < 13; ++j)
		{
			CCSprite* snag = CCSprite::create("stock_draw_circle.png");
			if (j%2 == 1)
			{
				snag->setPosition(ccp(winX/6 * i + radius, (420-(winX/6/2)*j)));
			}
			else
			{
				snag->setPosition(ccp(winX/6/2 + winX/6 * i + radius, (420-(winX/6/2)*j)));
			}
			snags->addChild(snag);
			m_snagArr->addObject(snag);
		}
		//CCLOG("%f, %f", m_winSize.width/6, m_winSize.width/6/2-5);
	}
}

void SnagForestLayer::initSlots()
{


}

void SnagForestLayer::updateRandSpeed(float dt)
{

	if (m_randSpeed)
	{
		m_randSpeed = 8*CCRANDOM_0_1();
	}
	else
	{
		m_randSpeed = 1.0;
	}
}

void SnagForestLayer::update(float dt)
{
	if(m_ballLauncher == NULL)
	{
		CCLOG("SnagForestLayer::update(float dt) can not get ball");
		return;
	}
	if (!m_ballLauncher->isMoving())
	{
		return;
	}
	else
	{
		//CCLOG("%d", m_ballLauncher->getMovingDirection());
		if(m_ballLauncher->getMovingDirection())
		{
			m_ballLauncher->setMovingSpeed(m_ballLauncher->getPositionX()-m_randSpeed);
		}
		else
		{
			m_ballLauncher->setMovingSpeed(m_ballLauncher->getPositionX()+m_randSpeed);
		}

		m_ballLauncher->setPositionX(m_ballLauncher->getMovingSpeed());


		if(m_ballLauncher->getPositionX() <= m_ballLauncher->getBallSize().width/2)
		{
			//CCLOG("m_ball->m_movingDirection = false;");
			m_ballLauncher->setMovingDirection(false);
		}
		else if (m_ballLauncher->getPositionX() >= m_winSize.width - m_ballLauncher->getBallSize().width/2)
		{
			//CCLOG("m_ball->m_movingDirection = true;");
			m_ballLauncher->setMovingDirection(true);
		}
		//m_ballLauncher->setZOrder(m_ballLauncher->getPositionY());
	}

	// base on position of body(box2d) to update position of CCSprite
	b2Body* removeb = NULL;
	for(b2Body* b = m_box2dWorld->m_world->GetBodyList(); b; b = b->GetNext())
	{
		if ((b->GetType() == b2_dynamicBody) && (b->GetUserData() != NULL))
		{
			Ball* fallBall = (Ball*)b->GetUserData();
			if (fallBall)
			{
				fallBall->setPosition(ccp(b->GetPosition().x*PT_RATIO, b->GetPosition().y*PT_RATIO));
				fallBall->setRotation(-(float)CC_RADIANS_TO_DEGREES(b->GetAngle()));

				routeDetection(fallBall);

				if (fallBall->getPositionY() <= fallBall->getBallSize().height/2)
				{
					this->removeChild(fallBall);
					removeb = b;
				}
				else
				{
					//CCLOG("sprite->getPositionY() %f", sprite->getPositionY());
				}
			}
		}
	}

	if(removeb != NULL)
	{
		m_box2dWorld->m_world->DestroyBody(removeb);
		this->setTouchEnabled(true);
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

	//m_box2dWorld->m_world->DrawDebugData();
	drawTriangle();
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
	//this->setTouchEnabled( false );
	CCPoint boxBall = m_ballLauncher->getPosition();

	//CCSprite* fallBall = CCSprite::create("CloseNormal.png");
	Ball* fallBall = Ball::create();
	fallBall->bindSprite(CCSprite::create("Gui/Login_1/Object.png"));
	fallBall->setBallSize(fallBall->getSprite()->getContentSize());

	b2CircleShape shape1;
	shape1.m_radius = 11.5/PT_RATIO;
	b2FixtureDef fd1;
	fd1.shape = &shape1;
	fd1.density = 8.0f;
	fd1.friction = 10.0f*CCRANDOM_0_1();
	float32 restitution = 0.1f;
	b2BodyDef bd1;
	bd1.type = b2_dynamicBody;
	bd1.position.Set(boxBall.x/PT_RATIO, boxBall.y/PT_RATIO);
	bd1.userData = fallBall;

	b2Body* body = m_box2dWorld->m_world->CreateBody(&bd1);

	fd1.restitution = restitution;
	body->CreateFixture(&fd1);
	this->addChild(fallBall);

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

void SnagForestLayer::routeDetection(Ball* fallBall)
{
	CCObject* obj = NULL;
	CCSprite* snag = NULL;
	//CCSprite* tmpsnag = NULL;
	CCARRAY_FOREACH(m_snagArr, obj)
	{
		snag = (CCSprite *)obj;
		if (isCollidedWithBall(fallBall, snag))
		{
			
			//CCLOG("void SnagForestLayer::routeDetection(Ball* fallBall)");
		}
	}
}


void SnagForestLayer::drawTriangle()
{
	CHECK_GL_ERROR_DEBUG();
	ccDrawColor4B(0,0,255,128);
	CCPoint m_vertices[c_triSnags] = { ccp(0,5), ccp(45,50), ccp(10,155), ccp(15,58) };
	ccDrawSolidPoly(m_vertices, c_triSnags, ccc4f(0.5f, 0.5f, 1, 1 ) );
}


bool SnagForestLayer::isCollidedWithBall(Ball* fallBall, CCSprite *snag)
{
	if (fallBall->getSprite() != NULL)
	{
		CCRect entityRrct = fallBall->getBoundingBox();
		CCPoint snagPos = snag->getPosition();

		return entityRrct.containsPoint(snagPos);
	}

	return false;
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