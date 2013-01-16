#pragma once

class CBullet;

class CCBulletSprite : public CCGameSprite, public CCLogicInfoInterface, public CCProjectileWithAttackData
{
public:
    typedef enum
    {
        kExploding = 0x00000001,

    } E_tANK_DOING_FLAG;

public:
    virtual ~CCBulletSprite() = 0;

    virtual bool initWithSpriteFrameNameAndLogic(const char *pszSpriteFrameName, CBullet* pBullet);

    virtual CCTankSprite* getOwner() const;

    virtual bool collisionDetection();

    virtual void explode() = 0;
    virtual bool isExploding() const = 0;
    virtual void fire() = 0;
};



class CCNormalBulletSprite : public CCBulletSprite
{
public:
    CCNormalBulletSprite();
    virtual ~CCNormalBulletSprite();

    DECLARE_CREATE_WITH_SPRITE_FRAME_NAME_LOGIC(CCNormalBulletSprite);

    virtual void update(float dt);
    virtual void explode();
    virtual bool isExploding() const;

    CC_SYNTHESIZE(float, m_fRange, Range);

    virtual void fire();
    virtual void fire(float fPower, float fVel, float fRange, float fScaleX, float fScaleY);

    void actExplodeEnd(CCNode* pNode);
    void actMoveEnd(CCNode* pNode);

};

class CCHomingMissileSprite : public CCBulletSprite
{
public:
    CCHomingMissileSprite();
    virtual ~CCHomingMissileSprite();

    DECLARE_CREATE_WITH_SPRITE_FRAME_NAME_LOGIC(CCHomingMissileSprite);

    virtual float getLineVelocity() const;
    virtual float getAngularVelocity() const;

    CC_SYNTHESIZE(int, m_iTargetKey, TargetKey);
    CC_SYNTHESIZE(float, m_fForceDuration, ForceDuration);

    //virtual bool isDirty(void);
    //virtual CCAffineTransform nodeToParentTransform(void);

    void updateBodyByPhyBody();
    virtual void update(float dt);

    virtual void explode();

    virtual bool isExploding() const;

    virtual void fire();
    virtual void fire(float fPower, int iTargetKey, float fInitDuration);

    void actExplodeEnd(CCNode* pNode);

    b2Vec2 getHeadPoint();
    b2Vec2 getTailPoint();

public:
    b2Body* m_pPhyBody;
    CCParticleSystem* m_pEmitter;
    b2Vec2 m_oVecForce;
    float m_fSumDt;
};

