#pragma once



class CBuffSkill;


typedef CCPoint S_POINT;
typedef CCSize S_SIZE;
typedef CCRect S_RECT;

class CGameInfo;
class CBulletInfo;
class CTankInfo;
class CBullet;
class CTank;
class CCGameSprite;
class CCBulletSprite;
class CCNormalTankSprite;
class CCGameManager;
class CUnitPackage;

class CEventInterface
{
public:
    virtual ~CEventInterface() = 0;

    virtual void onTick(CGameInfo* pGameInfo, float fDt); // ÿ��ʱ�䵥λ�ᴥ��
    virtual void onFire(CGameInfo* pGameInfo, CBulletInfo* pBullet); // ��̹�˿���ᴥ��
    virtual void onDamage(CGameInfo* pGameInfo, CBulletInfo* pBullet); // �ܵ��˺��ᴥ��
    virtual void onHitWall(CGameInfo* pGameInfo); // ײǽ�ᴥ��
    virtual void onHitTank(CGameInfo* pGameInfo, CTankInfo* pTank); // ײ��̹�˻ᴥ��
    virtual void onDeath(CGameInfo* pGameInfo); // ̹�˱��ɵ��ᴥ��
    virtual void onKill(CGameInfo* pGameInfo, CTankInfo* pKiller, CTankInfo* pDead); // �ɵ�һ��̹�˻ᴥ��
    virtual void onInit(CGameInfo* pGameInfo); // ̹�˱������ᴥ��
    virtual void onDestroy(CGameInfo* pGameInfo); // ̹�˱����ٻᴥ��
    virtual void onGameInit(CGameInfo* pGameInfo); // ��Ϸ��ʼ��ʱ�ᴥ��
    virtual void onGameOver(CGameInfo* pGameInfo); // ��Ϸ����ʱ�ᴥ��
    virtual void onJionGame(CGameInfo* pGameInfo, CTankInfo* pTank); // ����̹�˼���ᴥ��
};

class CGameLogicObject
{
public:
    CGameLogicObject();
    virtual ~CGameLogicObject() = 0;
    
    virtual S_POINT getPosition() const = 0;
    virtual S_SIZE getSize() const;
    virtual S_RECT getRect() const = 0;
    virtual float getLineVelocity() const = 0;
    virtual float getAngularVelocity() const = 0;
    virtual float getHeading() const = 0;
    virtual float getDistance(const CGameLogicObject* pTarget);
    virtual int getLogicKey() const;

private:
    const int m_iLogicKey;
};

class CDisplayInfoInterface
{
public:
    CDisplayInfoInterface();
    virtual ~CDisplayInfoInterface() = 0;

    virtual CCGameSprite* getDisplayBody() const;
    virtual void setDisplayBody(CCGameSprite* pDisplayBody);

public:
    CCGameSprite* m_pDisplayBody;
};

class CBulletInfo : public CGameLogicObject
{
public:
    CBulletInfo();

    virtual int getOwnerKey() const = 0;
    virtual float getPower() const = 0;
};

class CTankInfo : public CGameLogicObject
{
public:
    CTankInfo();

    virtual float getGunHeading() const = 0;
    virtual bool isMoving() const = 0;
};

class CGameInfo
{
public:
    virtual ~CGameInfo() = 0;

    virtual int getTankCount() const = 0;
    virtual CTankInfo* getTankByIndex(int iIndex) const = 0;
    virtual CTankInfo* getTankByKey(int iKey) const = 0;
    virtual CBulletInfo* getBulletByKey(int iKey) const = 0;
    virtual S_SIZE getStageSize() const = 0;
};

class CGameControl : public CGameInfo
{
public:
    CGameControl();
    virtual ~CGameControl();

    virtual int getTankCount() const;
    virtual CTankInfo* getTankByIndex(int iIndex) const;
    virtual CTankInfo* getTankByKey(int iKey) const;
    virtual CBulletInfo* getBulletByKey(int iKey) const;
    virtual S_SIZE getStageSize() const;
    int keygen();

    M_SYNTHESIZE(CCGameManager*, m_pGameMgr, GameManager);

public:
    int m_iIdKeygen;
};

class CBullet : public CBulletInfo, public CDisplayInfoInterface
{
public:
    CBullet();
    virtual ~CBullet();

    virtual S_POINT getPosition() const;
    virtual S_RECT getRect() const;
    virtual float getLineVelocity() const;
    virtual float getAngularVelocity() const;
    virtual float getHeading() const;
    virtual int getOwnerKey() const;
    virtual void setOwnerKey(int iOwnerKey);
    M_SYNTHESIZE(float, m_fPower, Power);

public:
    int m_iOwnerKey;
};

#include "Unit.h"
class CTank : public CTankInfo, public CUnit, public CLevelExp, public CUnitForce, public CDisplayInfoInterface, public CEventInterface
{
public:
    CTank();
    virtual ~CTank();

    virtual S_POINT getPosition() const;
    virtual S_RECT getRect() const;
    virtual float getLineVelocity() const;
    virtual float getAngularVelocity() const;
    virtual float getHeading() const;

    virtual float getGunHeading() const;
    virtual void setVelocity(float fVel);
    virtual float getTurningVelocity() const;
    virtual void setTurningVelocity(float fVel);
    virtual float getTurningGunVelocity() const;
    virtual void setTurningGunVelocity(float fVel);
    virtual float getFireRange() const;
    virtual void setFireRange(float fRange);

    virtual void fire(float Power);
    
    virtual void say(const string& sStr);

    virtual void suspend();
    virtual void resume();

    virtual void startDoing(uint16_t wMask);
    virtual void endDoing(uint16_t wMask);
    virtual bool isDoingOr(uint16_t wMask) const;
    virtual bool isDoingAnd(uint16_t wMask) const;
    virtual bool isDoingNothing() const;
    virtual bool isExploding() const;
    virtual bool isTurning() const;
    virtual bool isMoving() const;
    virtual bool isTurningGun() const;
    virtual bool isSuspended() const;

    virtual void onTick(CGameInfo* pGameInfo, float fDt);
    virtual void onDie();

    M_SYNTHESIZE(uint32_t, m_dwBonusExp, BonusExp);

public:
    int m_iSuspendCount;
};

class CNormalTank : public CTank
{
public:
    CNormalTank();
    virtual ~CNormalTank();

    virtual void turnBy(float fDegree, float fVel);

    virtual void turnGunBy(float fDegree, float fVel);

    virtual void stopMove();
    virtual void moveTo(const S_POINT& oPos, float fVel);
    virtual void moveBy(float fDistance, float fVel);

public:

};

class CTankDemoA : public CNormalTank
{
public:
    virtual void onTick(CGameInfo* pGameInfo, float fDt);
    virtual void onInit(CGameInfo* pGameInfo);
    float m_fDt;
};

class CTankDemoByTS : public CNormalTank
{
public:
    virtual void skill1(int iKey);
    virtual void skill2(float fDuration);
    virtual void onTick(CGameInfo* pGameInfo, float fDt);
    virtual void onFire(CGameInfo* pGameInfo, CBulletInfo* pBullet);
    virtual void onInit(CGameInfo* pGameInfo);
    virtual void updateMaxExp(); // @override
    virtual void onLevelChange(int32_t iChanged); // @override

    float getDeflection(CTankInfo* pTank) const;
    float m_fDt;
    int m_iBulletKey;
    bool m_bUseAI;

	
};



