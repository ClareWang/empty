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
	, m_upBallAngle(-1.0)
	, m_upBall(NULL)
	, m_isBallGoingUp(true)
	, m_removeb(NULL)
	, m_devil(NULL)
{
}

SnagForestLayer::~SnagForestLayer()
{
	delete m_box2dWorld;
	m_snagArr->release();
	m_cellDic->release();
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
	m_winX = m_winSize.width -c_radius*2;
	m_gameEntry = g_gameEntries + entryId;
	m_box2dWorld = m_gameEntry->createFcn(m_winSize.width, m_winSize.height);

	initMap();
	initBallLauncher();
	initSnags();
	initCell();

	interactionSubscribe();

	setTouchEnabled( true );
	schedule( schedule_selector(SnagForestLayer::tick) );

	this->scheduleUpdate();

	return true;
}

void SnagForestLayer::update(float dt)
{
	// base on position of body(box2d) to update position of CCSprite
	for(b2Body* b = m_box2dWorld->m_world->GetBodyList(); b; b = b->GetNext())
	{
		if ((b->GetType() == b2_dynamicBody) && (b->GetUserData() != NULL))
		{
			Ball* fallBall = (Ball*)b->GetUserData();
			float bX = (float)(b->GetPosition().x);
			float bY = (float)(b->GetPosition().y);
			if (fallBall->getSprite() != NULL)
			{
				if (fallBall->getPositionY() <= fallBall->getBallSize().height/2)
				{
					this->removeChild(m_upBall);
					m_upBall = NULL;
					b->SetUserData(NULL);// otherwise  0xC0000005  error
					m_removeb = b;   /* this can generate remove missing, if we have more than one ball at the same time in screen.
									 the latest one can cover others, use array can solve it.*/
				}
				else
				{
					fallBall->setPosition(ccp(bX*PT_RATIO, bY*PT_RATIO));
					fallBall->setRotation(-(float)CC_RADIANS_TO_DEGREES(b->GetAngle()));
					if (m_devil != NULL && isCollidedWithBall(fallBall, m_devil))
					{
						triggerDevil();
					}
					routeDetection(fallBall);
				}
			}

		}
	}

	// need use array to avoid remove missing problem.
	if(m_removeb != NULL)
	{
		m_box2dWorld->m_world->DestroyBody(m_removeb);
		m_removeb = NULL;
		m_isBallGoingUp = true;
		this->setTouchEnabled(true);
	}
}

void SnagForestLayer::tick(float dt)
{
	m_box2dWorld->Step(&settings);
}

void SnagForestLayer::ballLauncherMoving(float dt)
{
	if (m_upBall != NULL && !this->isTouchEnabled())
	{
		CCPoint expect;
		if ( m_isBallGoingUp )
		{
			expect = ccp(10*tan(CC_DEGREES_TO_RADIANS(m_upBallAngle)), 10);
			m_isBallGoingUp = m_upBall->getPositionY() < (m_winSize.height - m_upBall->getBallSize().height/2) ? true : false;
		}
		else
		{
			expect = ccp(10*tan(CC_DEGREES_TO_RADIANS(m_upBallAngle)), -10);
		}
		m_upBall->setPosition(m_upBall->getPosition() + expect);
		if (!m_isBallGoingUp && m_upBall->getPositionY() < m_winSize.height - 100)
		{
			createFallBall();
			this->unschedule( schedule_selector(SnagForestLayer::ballLauncherMoving) );
		}
	}
}

//void SnagForestLayer::draw()
//{
//	CCLayer::draw();
//
//	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
//	kmGLPushMatrix();
//	//m_box2dWorld->m_world->DrawDebugData();
//	kmGLPopMatrix();
//
//	CHECK_GL_ERROR_DEBUG();
//}

void SnagForestLayer::registerWithTouchDispatcher()
{
	// higher priority than dragging
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -10, true);
}

bool SnagForestLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	schedule( schedule_selector(SnagForestLayer::ballLauncherMoving) );
	if (m_upBall != NULL)
	{
		this->removeChild(m_upBall);
		m_upBall = NULL;
	}

	m_upBall = Ball::create();
	m_upBall->bindSprite(CCSprite::create("CloseNormal.png"));
	m_upBall->setBallSize(m_upBall->getSprite()->getContentSize());
	m_upBall->setPosition(ccp(m_winSize.width/2-1, 80));

	this->addChild(m_upBall,1);

	return true;

}
void SnagForestLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCPoint start = touch->getStartLocation();

	CCPoint afterStart = touch->getLocation();

	if (afterStart.y - start.y < m_winSize.height && afterStart.x - start.x != 0 )
	{
		//m_upBall->setAnchorPoint(ccp(0.5,0));
		m_upBallAngle = atanf((afterStart.x - start.x)/abs(afterStart.y - m_upBall->getPositionY()));
		float upBallRotation = (float)CC_RADIANS_TO_DEGREES(m_upBallAngle);
		if (upBallRotation < 0)
		{
			upBallRotation = upBallRotation < -BALL_LAUNCH_ROTATION ? -BALL_LAUNCH_ROTATION : upBallRotation;
		}
		else if (upBallRotation > 0)
		{
			upBallRotation = upBallRotation > BALL_LAUNCH_ROTATION ? BALL_LAUNCH_ROTATION : upBallRotation;
		}
		m_upBallAngle = upBallRotation;
		m_upBall->setRotation(m_upBallAngle);

	}

}
void SnagForestLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	m_upBallAngle = m_upBall->getRotation();
	this->setTouchEnabled( false );
}


/*private function*/

/* === Initial Scene ===*/
void SnagForestLayer::initMap()
{
	CCSprite *bg_Sprite = CCSprite::create("SnagForest_BG.png");
	bg_Sprite->setPosition( ccp( m_winSize.width/2, m_winSize.height/2));
	bg_Sprite->setOpacity(100);
	this->addChild(bg_Sprite, 0, 1);
}

void SnagForestLayer::initBallLauncher()
{

}

void SnagForestLayer::initSnags()
{
	m_snagArr = CCArray::create();
	m_snagArr->retain();
	CCSpriteBatchNode* snags = CCSpriteBatchNode::create("stock_draw_circle.png");
	this->addChild(snags,2);

	for (int32 i = 0; i < 7; ++i)
	{
		for(int32 j = 0; j < 13; ++j)
		{
			CCSprite* snag = CCSprite::create("stock_draw_circle.png");
			if (j%2 == 1)
			{
				snag->setPosition(ccp(m_winX/6 * i + c_radius, (c_heightStart-(m_winX/6/2)*j)));
			}
			else
			{
				snag->setPosition(ccp(m_winX/6/2 + m_winX/6 * i + c_radius, (c_heightStart-(m_winX/6/2)*j)));
			}
			snags->addChild(snag);
			m_snagArr->addObject(snag);
		}
	}
}

void SnagForestLayer::initCell()
{
	m_cellDic = CCDictionary::create();
	m_cellDic->retain();
	for(int32 j = 0; j < 12; ++j)
	{
		CCArray* cellArr = CCArray::create();
		for (int32 i = 0; i < 7; ++i)
		{
			CCSprite* cell = CCSprite::create("CloseSelected.png");
			cell->setRotation(90);
			if (j%2 == 1)
			{
				cell->setPosition(ccp(m_winX/6 * i + c_radius, (c_heightStart-(m_winX/6/2)*(j+1))));
			}
			else
			{
				cell->setPosition(ccp(m_winX/6/2 + m_winX/6 * i + c_radius, (c_heightStart-(m_winX/6/2)*(j+1))));
			}
			cell->setVisible(false);
			this->addChild(cell, 3);
			cellArr->addObject(cell);
		}
		m_cellDic->setObject(cellArr,j);
	}
}

void SnagForestLayer::initSlots()
{


}


/* === Ball Action ===*/
void SnagForestLayer::createFallBall()
{
	b2CircleShape shape1;
	shape1.m_radius = 11.5/PT_RATIO;
	b2FixtureDef fd1;
	fd1.shape = &shape1;
	fd1.density = 8.0f;
	fd1.friction = 10.0f*CCRANDOM_0_1();
	float32 restitution = 0.15f;
	b2BodyDef bd1;
	bd1.bullet = true;
	bd1.type = b2_dynamicBody;
	bd1.position.Set(m_upBall->getPositionX()/PT_RATIO, m_upBall->getPositionY()/PT_RATIO);
	bd1.userData = m_upBall;

	b2Body* body = m_box2dWorld->m_world->CreateBody(&bd1);

	fd1.restitution = restitution;
	body->CreateFixture(&fd1);
}

void SnagForestLayer::routeDetection(Ball* fallBall)
{
	if (fallBall->getPositionY() > c_heightStart-(m_winX/6) && fallBall->getPositionY() <= c_heightStart)
	{
		showCells(fallBall, 0);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*1.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6))
	{
		showCells(fallBall, 1);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*2) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*1.5))
	{
		showCells(fallBall, 2);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*2.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*2))
	{
		showCells(fallBall, 3);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*3) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*2.5))
	{
		showCells(fallBall, 4);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*3.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*3))
	{
		showCells(fallBall, 5);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*4) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*3.5))
	{
		showCells(fallBall, 6);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*4.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*4))
	{
		showCells(fallBall, 7);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*4.5))
	{
		showCells(fallBall, 8);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*5.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*5))
	{
		showCells(fallBall, 9);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*6) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*5.5))
	{
		showCells(fallBall, 10);
	}
	else if (fallBall->getPositionY() > c_heightStart-(m_winX/6*6.5) && fallBall->getPositionY() <= c_heightStart-(m_winX/6*6))
	{
		showCells(fallBall, 11);
	}

}

void SnagForestLayer::showCells(Ball* fallBall, unsigned int indexOfCellArr)
{
	CCObject* obj = NULL;
	CCSprite* cell = NULL;
	CCArray* cellArr = (CCArray*)m_cellDic->objectForKey(indexOfCellArr);
	CCARRAY_FOREACH(cellArr, obj)
	{
		cell = (CCSprite *)obj;
		if (isCollidedWithBall(fallBall, cell))
		{
			if (!cell->isVisible())
			{
				cell->setVisible(true);
			}
		}
	}
}

bool SnagForestLayer::isCollidedWithBall(Ball* fallBall, CCNode *node)
{
	if (fallBall->getSprite() != NULL)
	{
		CCRect entityRrct = fallBall->getBoundingBox();
		CCPoint nodePos = node->getPosition();
		return entityRrct.containsPoint(nodePos);
	}

	return false;
}


/* === Devil Action ===*/
void SnagForestLayer::handleDevil(CCObject* pData)
{
	m_devil = (CCNode*)pData;
}

void SnagForestLayer::interactionSubscribe()
{
	CCNotificationCenter::sharedNotificationCenter()->addObserver(
		this,
		callfuncO_selector(SnagForestLayer::handleDevil),
		MsgTypeForObserver::c_DevilPosUpdate,
		NULL);
}

void SnagForestLayer::triggerDevil()
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification(MsgTypeForObserver::c_DevilFightingStart, NULL);
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