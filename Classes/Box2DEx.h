#pragma once

class CCB2dScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCB2dScene);

    virtual void onEnter();
	virtual void draw();

private:
	GLESDebugDraw* m_debugDraw;
};

class CCB2dWorld : public CCObject
{
public:
	CCB2dWorld();
	virtual ~CCB2dWorld();

    virtual bool init();
    CREATE_FUNC(CCB2dWorld);

	virtual void worldStep(float fDt);
	virtual int getBodyCount();
    virtual void setGravity(const b2Vec2& oGravity);
	virtual b2Body* createBody(b2BodyDef* bd);
	virtual void destroyBody(b2Body* pBody);

	//debug draw
	virtual void setDebugDraw(b2Draw* debugDraw);
	virtual void drawDebugData();

	static CCB2dWorld* sharedB2dWorld();

protected:
    static CCB2dWorld* m_pInst;
	b2World* m_pWorld;
};

class CCB2dAction : public CCAction
{
public:
	CCB2dAction();
	virtual ~CCB2dAction();

	static CCB2dAction* create();

	//implements for CCAction
	virtual bool isDone(void);
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual void startWithTarget(CCNode *pTarget);
    virtual void step(float dt);

protected:
	b2Body* m_pBody;
};

