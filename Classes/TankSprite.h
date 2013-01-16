#pragma once

class CTank;

class CCTankEventTrigger : public CCEventTriggerInterface
{
public:
    CCTankEventTrigger();
    virtual ~CCTankEventTrigger() = 0;

    virtual void triggerOnTick(float fDt); // 每个时间单位会触发
    virtual void triggerOnFire(CCBulletSprite* pBullet); // 有坦克开火会触发
    virtual void triggerOnDamage(CCBulletSprite* pBullet); // 受到伤害会触发
    virtual void triggerOnHitWall(); // 撞墙会触发
    virtual void triggerOnHitTank(CCTankSprite* pTank); // 撞上坦克会触发
    virtual void triggerOnDeath(); // 坦克被干掉会触发
    virtual void triggerOnKill(CCTankSprite* pKiller, CCTankSprite* pDead); // 干掉一辆坦克会触发
    virtual void triggerOnInit(); // 坦克被创建会触发
    virtual void triggerOnDestroy(); // 坦克被销毁会触发
    virtual void triggerOnGameInit(); // 游戏初始化时会触发
    virtual void triggerOnGameOver(); // 游戏结束时会触发
    virtual void triggerOnJionGame(CCTankSprite* pTank); // 有新坦克加入会触发

};

class CCTankSprite : public CCGameSprite, public CCLogicInfoInterface, public CCTankEventTrigger
{
public:
    typedef enum
    {
        kExploding = 1,
        kTurning = 1 << 1,
        kMoving = 1 << 2,
        kTurningGun = 1 << 3,
        kSuspended = 1 << 4,

    } E_TANK_DOING_FLAG;

public:
    CCTankSprite();
    virtual ~CCTankSprite() = 0;

    virtual bool initWithSpriteFrameNameAndLogicGun(const char *pszSpriteFrameName, CTank* pTank, CCSprite* pGunSprite);
    virtual void update(float dt);
    virtual bool collisionDetection();

    virtual CEventInterface* getEventInterface() const;
    virtual bool isTriggerOn() const;

    virtual void setLineVelocity(float fVel);
    virtual CCSprite* getGun() = 0;
    virtual void setGun(CCSprite* pGunSprite) = 0;
    virtual int getKey() const;
    CC_SYNTHESIZE(float, m_fTurningVelocity, TurningVelocity);
    CC_SYNTHESIZE(float, m_fTurningGunVelocity, TurningGunVelocity);
    CC_SYNTHESIZE(float, m_fFirePower, FirePower);
    CC_SYNTHESIZE(float, m_fFireRange, FireRange);

    virtual void explode() = 0;
    virtual void stop() = 0;
    virtual void fire(float fPower) = 0;
    virtual void stopMove() = 0;
    virtual void suspend() = 0;
    virtual void resume() = 0;
    
    virtual bool isExploding() const;
    virtual bool isTurning() const;
    virtual bool isMoving() const;
    virtual bool isTurningGun() const;
    virtual bool isSuspended() const;

};

class CCNormalTankSprite : public CCTankSprite
{
public:
    CCNormalTankSprite();
    virtual ~CCNormalTankSprite();

    DECLARE_CREATE_WITH_SPRITE_FRAME_NAME_GUN(CCNormalTankSprite);

    virtual void update(float dt);

    virtual CCSprite* getGun();
    virtual void setGun(CCSprite* pGunSprite);

    virtual void explode();
    virtual void stop();
    virtual void fire(float fPower);

    virtual void turnBy(float fDegree, float fVel);

    virtual void turnGunBy(float fDegree, float fVel);

    virtual void moveTo(const CCPoint& oPos, float fVel);
    virtual void moveBy(float fDistance, float fVel);
    virtual void stopMove();

    virtual void suspend();
    virtual void resume();

    void actExplodeEnd(CCNode* pNode);
    void actTurnEnd(CCNode* pNode);
    void actMoveEnd(CCNode* pNode);
    void actTurnGunEnd(CCNode* pNode);

public:
};

class CCSkillTankSprite : public CCNormalTankSprite
{
public:
    DECLARE_CREATE_WITH_SPRITE_FRAME_NAME_GUN(CCSkillTankSprite);
    virtual void skill1(float fPower, int iKey);
    virtual void skill2(float fDuration);
};