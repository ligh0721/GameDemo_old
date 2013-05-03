#include "CommInc.h"
#include "Box2DEx.h"
#include "GameDisplay.h"

#define DEBUG_DRAW true


bool CCB2dScene::init()
{
	this->m_debugDraw = new GLESDebugDraw(PTM_RATIO);
	CCB2dWorld::sharedB2dWorld()->setDebugDraw(this->m_debugDraw);

    return true;
}

void CCB2dScene::onEnter()
{
    CCScene::onEnter();
    getScheduler()->scheduleSelector(schedule_selector(CCB2dWorld::worldStep), CCB2dWorld::sharedB2dWorld(), 0.01f, false);
}

void CCB2dScene::draw()
{
    /*
	if(DEBUG_DRAW)
	{
		if(CCGameManager::sharedGameManager()->m_pBattleGroundLayer)
		{
			CCPoint point = CCGameManager::sharedGameManager()->m_pBattleGroundLayer->getPosition();
			b2Vec2 trans(point.x, point.y);
			this->m_debugDraw->setTrans(trans);
		}
		ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
		kmGLPushMatrix();
		CCB2dWorld::sharedB2dWorld()->drawDebugData();
		kmGLPopMatrix();	
	}
    */
}

CCB2dWorld* CCB2dWorld::m_pInst = NULL;

CCB2dWorld::CCB2dWorld()
{
}

CCB2dWorld::~CCB2dWorld()
{
	if (m_pWorld)
	{
		delete m_pWorld;
	}
}

bool CCB2dWorld::init()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);

	this->m_pWorld = new b2World(gravity);
	this->m_pWorld->SetAllowSleeping(true);
	this->m_pWorld->SetContinuousPhysics(true);

    return true;
}

void CCB2dWorld::worldStep(float fDt)
{
	int velocityIterations = 10;
	int positionIterations = 10;

	this->m_pWorld->Step(fDt, velocityIterations, positionIterations);
}

int CCB2dWorld::getBodyCount()
{
	int num = this->m_pWorld->GetBodyCount();

	return num;
}

void CCB2dWorld::setGravity( const b2Vec2& oGravity )
{
    this->m_pWorld->SetGravity(oGravity);
}

b2Body* CCB2dWorld::createBody(b2BodyDef* bd)
{
	return this->m_pWorld->CreateBody(bd);
}

void CCB2dWorld::destroyBody(b2Body* pBody)
{
	this->m_pWorld->DestroyBody(pBody);
}

CCB2dWorld* CCB2dWorld::sharedB2dWorld()
{
    if (m_pInst)
    {
        return m_pInst;
    }

    m_pInst = CCB2dWorld::create();
    CC_SAFE_RETAIN(m_pInst);
    return m_pInst;
}

void CCB2dWorld::setDebugDraw(b2Draw* debugDraw)
{
	this->m_pWorld->SetDebugDraw(debugDraw);
	uint32 flags = 1;
	debugDraw->SetFlags(flags);
}

void CCB2dWorld::drawDebugData()
{
	this->m_pWorld->DrawDebugData();
}

CCB2dAction* CCB2dAction::create()
{
	CCB2dAction* pB2dAction = new CCB2dAction();
	pB2dAction->autorelease();

	return pB2dAction;
}

CCB2dAction::CCB2dAction()
{
	this->m_pBody = NULL;
}

CCB2dAction::~CCB2dAction()
{
	if(this->m_pBody)
	{
		CCB2dWorld* pWorld = CCB2dWorld::sharedB2dWorld();
		pWorld->destroyBody(this->m_pBody);
	}
}

bool CCB2dAction::isDone(void)
{
	return false;
}

CCObject* CCB2dAction::copyWithZone(CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	CCB2dAction* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		//in case of being called at sub class
		pCopy = (CCB2dAction*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCB2dAction();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCAction::copyWithZone(pZone);

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

void CCB2dAction::startWithTarget( CCNode *pTarget )
{
    CCAction::startWithTarget(pTarget);
    
	CCB2dWorld* pWorld = CCB2dWorld::sharedB2dWorld();

	//create body
	b2BodyDef bd;
	bd.type = b2_dynamicBody;

	b2PolygonShape shape;
	CCRect bound = this->m_pTarget->boundingBox();
	b2Vec2 size(bound.size.width/2/PTM_RATIO, bound.size.height/2/PTM_RATIO);
	shape.SetAsBox(size.x, size.y);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 0.01f;

	float x = pTarget->getPositionX();
	float y = pTarget->getPositionY();
	b2Vec2 postion(x/PTM_RATIO, y/PTM_RATIO);
	bd.position = postion;
	bd.linearDamping = 4.0f;
	bd.angularDamping = 20.0f;
	this->m_pBody = pWorld->createBody(&bd);
	this->m_pBody->CreateFixture(&fixture);
}

void CCB2dAction::step(float dt)
{
	b2Vec2 pos  = this->m_pBody->GetPosition();
	float radian = this->m_pBody->GetAngle();

	float x = pos.x * PTM_RATIO;
	float y = pos.y * PTM_RATIO;

	CCPoint point = this->m_pTarget->getAnchorPointInPoints();

	if(this->m_pTarget)
	{
		this->m_pTarget->setPosition(x, y);
        this->m_pTarget->setRotation(-CC_RADIANS_TO_DEGREES(radian));
	}
}
