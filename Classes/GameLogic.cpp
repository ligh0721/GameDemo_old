#include "CommInc.h"

#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Unit.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "GameLogic.h"
#include "Package.h"
#include "Prop.h"






CEventInterface::~CEventInterface()
{

}

void CEventInterface::onTick( CGameInfo* pGameInfo, float fDt )
{

}

void CEventInterface::onFire( CGameInfo* pGameInfo, CBulletInfo* pBullet )
{

}

void CEventInterface::onDamage( CGameInfo* pGameInfo, CBulletInfo* pBullet )
{

}

void CEventInterface::onHitWall( CGameInfo* pGameInfo )
{

}

void CEventInterface::onHitTank( CGameInfo* pGameInfo, CTankInfo* pTank )
{

}

void CEventInterface::onDeath( CGameInfo* pGameInfo )
{

}

void CEventInterface::onKill( CGameInfo* pGameInfo, CTankInfo* pKiller, CTankInfo* pDead )
{

}

void CEventInterface::onInit( CGameInfo* pGameInfo )
{

}

void CEventInterface::onDestroy( CGameInfo* pGameInfo )
{

}

void CEventInterface::onGameInit( CGameInfo* pGameInfo )
{

}

void CEventInterface::onGameOver( CGameInfo* pGameInfo )
{

}

void CEventInterface::onJionGame( CGameInfo* pGameInfo, CTankInfo* pTank )
{

}

CGameLogicObject::CGameLogicObject()
    : m_iLogicKey(CCGameManager::sharedGameManager()->getLogicBody()->keygen())
{
}

CGameLogicObject::~CGameLogicObject()
{
}

int CGameLogicObject::getLogicKey() const
{
    return m_iLogicKey;
}

float CGameLogicObject::getDistance( const CGameLogicObject* pTarget )
{
    return ccpDistance(getPosition(), pTarget->getPosition());
}

S_SIZE CGameLogicObject::getSize() const
{
    return getRect().size;
}

CDisplayInfoInterface::CDisplayInfoInterface()
    : m_pDisplayBody(NULL)
{
}

CDisplayInfoInterface::~CDisplayInfoInterface()
{
}

CCGameSprite* CDisplayInfoInterface::getDisplayBody() const
{
    return m_pDisplayBody;
}

void CDisplayInfoInterface::setDisplayBody( CCGameSprite* pDisplayBody )
{
    m_pDisplayBody = pDisplayBody;
}

CBulletInfo::CBulletInfo()
{
}

CTankInfo::CTankInfo()
{
}

CGameInfo::~CGameInfo()
{
}

CGameControl::CGameControl()
    : m_iIdKeygen(rand() % 10000)
{
}

CGameControl::~CGameControl()
{
}

int CGameControl::getTankCount() const
{
    return getGameManager()->getTankCount();
}

CTankInfo* CGameControl::getTankByIndex( int iIndex ) const
{
    return dynamic_cast<CTankInfo*>(getGameManager()->getTankByIndex(iIndex)->getLogicBody());
}

CTankInfo* CGameControl::getTankByKey( int iKey ) const
{
    CCTankSprite* pTank = dynamic_cast<CCTankSprite*>(getGameManager()->m_pTankBatch->getChildByTag(iKey));
    return pTank ? dynamic_cast<CTankInfo*>(pTank->getLogicBody()) : NULL;
}

CBulletInfo* CGameControl::getBulletByKey( int iKey ) const
{
    return dynamic_cast<CBulletInfo*>(dynamic_cast<CCTankSprite*>(getGameManager()->m_pBulletBatch->getChildByTag(iKey))->getLogicBody());
}

S_SIZE CGameControl::getStageSize() const
{
    return CCDirector::sharedDirector()->getVisibleSize();
    //return m_pGameMgr->m_pLayer->getContentSize();
}

int CGameControl::keygen()
{
    return m_iIdKeygen++;
}

CBullet::CBullet()
    : m_iOwnerKey(0)
    , m_fPower(0.0)
{
}

CBullet::~CBullet()
{
    //CCLOG("~CBullet");
}

S_POINT CBullet::getPosition() const
{
    return getDisplayBody()->getPosition();
}

S_RECT CBullet::getRect() const
{
    return getDisplayBody()->boundingBox();
}

float CBullet::getLineVelocity() const
{
    return getDisplayBody()->getLineVelocity();
}

float CBullet::getAngularVelocity() const
{
    return getDisplayBody()->getAngularVelocity();
}

float CBullet::getHeading() const
{
    return CCGameSprite::reviseRotation(getDisplayBody()->getRotation());
}

int CBullet::getOwnerKey() const
{
    
    return m_iOwnerKey;
}

void CBullet::setOwnerKey( int iOwnerKey )
{
    m_iOwnerKey = iOwnerKey;
}

CTank::CTank()
    : m_iSuspendCount(0)
{
    revive(getMaxHp());
}

CTank::~CTank()
{
    //CCLOG("~CTank(), iKey(%d)", getKey());
}

S_POINT CTank::getPosition() const
{
    return getDisplayBody()->getPosition();
}

S_RECT CTank::getRect() const
{
    return getDisplayBody()->boundingBox();
}

float CTank::getLineVelocity() const
{
    return isMoving() ? getDisplayBody()->getLineVelocity() : 0.0;
}

float CTank::getAngularVelocity() const
{
    return isTurning() ? getDisplayBody()->getAngularVelocity() : 0.0;
}

float CTank::getHeading() const
{
    return CCGameSprite::reviseRotation(getDisplayBody()->getRotation());
}

float CTank::getGunHeading() const
{
    return CCGameSprite::reviseRotation(getDisplayBody()->getRotation() + dynamic_cast<CCTankSprite*>(getDisplayBody())->getGun()->getRotation());
}

void CTank::setVelocity( float fVel )
{
    getDisplayBody()->setLineVelocity(fVel);
}

float CTank::getTurningVelocity() const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->getTurningVelocity();
}

void CTank::setTurningVelocity( float fVel )
{
    dynamic_cast<CCTankSprite*>(getDisplayBody())->setTurningVelocity(fVel);
}

float CTank::getTurningGunVelocity() const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->getTurningGunVelocity();
}

void CTank::setTurningGunVelocity( float fVel )
{
    dynamic_cast<CCTankSprite*>(getDisplayBody())->setTurningGunVelocity(fVel);
}

float CTank::getFireRange() const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->getFireRange();
}

void CTank::setFireRange( float fRange )
{
    dynamic_cast<CCTankSprite*>(getDisplayBody())->setFireRange(fRange);
}

void CTank::fire(float Power)
{
    dynamic_cast<CCTankSprite*>(getDisplayBody())->fire(Power);
}

void CTank::say(const string& sStr)
{
    
}

void CTank::suspend()
{
    ++m_iSuspendCount;
    dynamic_cast<CCTankSprite*>(getDisplayBody())->suspend();
}

void CTank::resume()
{
    if (m_iSuspendCount > 0)
    {
        --m_iSuspendCount;
        if (!m_iSuspendCount)
        {
            dynamic_cast<CCTankSprite*>(getDisplayBody())->resume();
        }
    }
}

void CTank::startDoing( uint16_t wMask )
{
    dynamic_cast<CCTankSprite*>(getDisplayBody())->startDoing(static_cast<uint32_t>(wMask) << 16);
}

void CTank::endDoing( uint16_t wMask )
{
    dynamic_cast<CCTankSprite*>(getDisplayBody())->endDoing(static_cast<uint32_t>(wMask) << 16);
}

bool CTank::isDoingOr( uint16_t wMask ) const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->isDoingOr(static_cast<uint32_t>(wMask) << 16);
}

bool CTank::isDoingAnd( uint16_t wMask ) const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->isDoingAnd(static_cast<uint32_t>(wMask) << 16);
}

bool CTank::isDoingNothing() const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->isDoingNothing();
}

bool CTank::isExploding() const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->isExploding();
}

bool CTank::isTurning() const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->isTurning();
}

bool CTank::isMoving() const
{
    return dynamic_cast<CCTankSprite*>(getDisplayBody())->isMoving();
}

bool CTank::isTurningGun() const
{
    return dynamic_cast<CCNormalTankSprite*>(getDisplayBody())->isTurningGun();
}

bool CTank::isSuspended() const
{
    return dynamic_cast<CCNormalTankSprite*>(getDisplayBody())->isSuspended();
}

void CTank::onTick( CGameInfo* pGameInfo, float fDt )
{
    CUnit::onTick(fDt);
}

void CTank::onDie()
{
    dynamic_cast<CCTankSprite*>(getDisplayBody())->explode();
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CTank* pPlayer = dynamic_cast<CTank*>(pGm->getLogicBody()->getTankByKey(pGm->m_iPlayerKey));
    if (pPlayer && isEnemyOf(pPlayer) && getDistance(pPlayer) <= pGm->m_fMaxBonusDist)
    {
        pPlayer->addExp(m_dwBonusExp);
    }
    
    CUnit::onDie();
}

CNormalTank::CNormalTank()
{
}

CNormalTank::~CNormalTank()
{
}

void CNormalTank::turnBy( float fDegree, float fVel )
{
    dynamic_cast<CCNormalTankSprite*>(getDisplayBody())->turnBy(fDegree, fVel);
}

void CNormalTank::turnGunBy( float fDegree, float fVel )
{
    dynamic_cast<CCNormalTankSprite*>(getDisplayBody())->turnGunBy(fDegree, fVel);
}

void CNormalTank::stopMove()
{
    dynamic_cast<CCNormalTankSprite*>(getDisplayBody())->stopMove();
}

void CNormalTank::moveTo( const S_POINT& oPos, float fVel )
{
    dynamic_cast<CCNormalTankSprite*>(getDisplayBody())->moveTo(oPos, fVel);
}

void CNormalTank::moveBy( float fDistance, float fVel )
{
    dynamic_cast<CCNormalTankSprite*>(getDisplayBody())->moveBy(fDistance, fVel);
}

void CTankDemoA::onTick( CGameInfo* pGameInfo, float fDt )
{
    CTank::onTick(pGameInfo, fDt);

    m_fDt += fDt;
    if (/*isDoingOr(0x1000) &&*/ m_fDt > 0.5)
    {
        fire(rand() % 10 + 5);
        m_fDt = 0;
    }
    S_SIZE oSz = pGameInfo->getStageSize();
    if (!isTurningGun())
    {
        turnGunBy(180 - rand() % 360, rand() % 300 + 50);
    }

    if (!isMoving())
    {
        setTurningVelocity(rand() % 50 + 450);
        moveTo(ccp(rand() % (int)oSz.width, rand() % (int)oSz.height), rand() % 350 + 50);
    }
}

void CTankDemoA::onInit( CGameInfo* pGameInfo )
{
    m_fDt = (rand() % 10) / 10.0;
    setVelocity(200);
    setTurningVelocity(100);
    setTurningGunVelocity(100);
    setFireRange(800);
    addSkill(CThumpPas::create(20, CExtraCoeff(5, 0), 0, 1));
    addSkill(CVampirePas::create(1.0));
    addSkill(CStatusShowPas::create());
}

void CTankDemoByTS::onTick( CGameInfo* pGameInfo, float fDt )
{
    CTank::onTick(pGameInfo, fDt);

    m_fDt += fDt;
    CTank* pTank = NULL;
    int iCount = pGameInfo->getTankCount();
    if (iCount < 2)
    {
        return;
    }
    CTank* pIt = NULL;
    for (int i = 0; i < iCount; i++)
    {

        pIt = dynamic_cast<CTank*>(pGameInfo->getTankByIndex(i));
        if (isEnemyOf(pIt))
        {
            if (!pTank)
            {
                pTank = pIt;
            }
            else if (pIt->getDistance(this) < pTank->getDistance(this))
            {
                pTank = pIt;
            }
        }
    }

    if (!pTank)
    {
        return;
    }

    setTurningGunVelocity(rand() % 200 + 200);
    S_POINT oPos = pTank->getPosition();
    S_POINT oTankLoc = getPosition();
    float fA = -ccpToAngle(ccp(oPos.x - oTankLoc.x, oPos.y - oTankLoc.y));
    float fR = getGunHeading();
    float fDelta = CCGameSprite::reviseRotation(CC_RADIANS_TO_DEGREES(fA) - fR);
    float fDis = ccpDistance(oTankLoc, oPos);
    float fAlpha = getDeflection(pTank);
    float fGunHd = pTank->getGunHeading();
    float fVel = pTank->getLineVelocity();
    
    if (!isTurningGun())
    {
        turnGunBy(fDelta, 400);
    }

    if (!m_bUseAI)
    {
        return;
    }

    if (abs(fDelta) < 2 && fDis < 400)
    {
        if (m_fDt > 2 && fDis < 400 && fDis > 50)
        {
            fire(10000 / (fDis * 10 + 1));
            m_fDt = 0;
        }
    }
    else
    {
        if (m_fDt > 5)
        {
            skill1(pTank->getLogicKey());
            m_fDt = 0;
        }
    }
    
    /*
    CBulletInfo* pBullet = pGameInfo->getBulletByKey(m_iBulletKey);
    if (pBullet && pBullet->getDistance(this) < 200 && !isDoingOr(0x0008))
    {
        startDoing(0x0008);
        moveBy()
    }

    if (isDoingOr(0x0008))
    {
        if (!isMoving())
        {
            endDoing(0x0008);
        }
        else
        {
            return;
        }
    }
    */
    

    if (isDoingOr(0x0010))
    {
        if (!isMoving())
        {
            endDoing(0x0010);
        }
        else
        {
            return;
        }
    }
    
    

    if (!isTurning() && isDoingOr(0x0002))
    {
        if (!isDoingOr(0x0004))
        {
            startDoing(0x0002 | 0x0004);
            if (isDoingOr(0x8000))
            {
                moveBy(200, -400);
            }
            else
            {
                moveBy(200, 400);
            }
            
        }
        else
        {
            if (!isMoving())
            {
                endDoing(0x0002 | 0x0004 | 0x8000);
            }
            
        }
    }

    if (fDis < 200 && !isDoingOr(0x0002))
    {
        if (!isDoingOr(0x0001))
        {
            startDoing(0x0001);
            moveBy(200 - fDis, -200);
        }
        else
        {
            if (!isMoving())
            {
                endDoing(0x0001);
            }
        }
        //moveTo(pTank->getPosition(), fVel
    }
    else
    {
        if (isDoingOr(0x0001))
        {
            endDoing(0x0001);
        }
    }

    if (fDis < 200 && abs(CCGameSprite::reviseRotation(fGunHd - (fAlpha - 180))) < 45 && !isDoingOr(0x0010))
    {
        startDoing(0x0010);
        //moveTo(ccp(cos(fAlpha - 180) * (200 - fDis), sin(180 - fAlpha) * (200 - fDis)), 300);
        moveBy(300 - fDis, -200);
    }

    if (!isMoving() && !isDoingOr(0x0001 | 0x0002))
    {
        setTurningVelocity(rand() % 200 + 200);
        moveTo(pTank->getPosition(), rand() % 300 + 100);
    }
}

void CTankDemoByTS::onFire( CGameInfo* pGameInfo, CBulletInfo* pBullet )
{
    if (!m_bUseAI)
    {
        return;
    }

    m_iBulletKey = pBullet->getLogicKey();
    CTank* pTank = dynamic_cast<CTank*>(pGameInfo->getTankByKey(pBullet->getOwnerKey()));
	if (pTank->isAllyOf(this))
	{
		return;
	}

    
    if (pBullet->getDistance(this) > 400)
    {
        return;
    }
    float f = getDeflection(pTank);
    float fDegree = CCGameSprite::reviseRotation(pBullet->getHeading() - (f - 180));
    //CCLOG("%f", fDegree);
    
    float fTurn;
    if (fDegree < 0 && fDegree > -45)
    {
        fTurn = CCGameSprite::reviseRotation((pBullet->getHeading() + 90) - getHeading());
        if (abs(fTurn) > 90)
        {
            fTurn = 180 - fTurn;
            startDoing(0x8000);
        }
        stopMove();
        startDoing(0x0002);
        turnBy(fTurn, 400);
    }
    else if (fDegree > 0 && fDegree < 45)
    {
        fTurn = CCGameSprite::reviseRotation((pBullet->getHeading() - 90) - getHeading());
        if (abs(fTurn) > 90)
        {
            fTurn = 180 - fTurn;
            startDoing(0x8000);
        }
        stopMove();
        startDoing(0x0002);
        turnBy(fTurn, 400);
    }
}

void CTankDemoByTS::onInit( CGameInfo* pGameInfo )
{
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CCSkillManager* pSm = CCSkillManager::sharedSkillManager();
    m_fDt = 0;
    m_bUseAI = true;
    setVelocity(200);
    setTurningVelocity(400);
    setTurningGunVelocity(400);
    m_iBulletKey = 0;
    addExp(0);

    addSkill(CThumpPas::create(75, CExtraCoeff(1, 2.3), 0, 1));
    addSkill(CStatusShowPas::create());

    if (getLogicKey() == pGm->m_iPlayerKey)
    {
        setFireRange(300);
        addSkill(CCommboPas::create(1, "%d HITS!!"));
        addSkill(CAttackTextPas::create(1, "%.0f"));
        addSkill(CShrapnelPas::create(5, 3, 360.0 / 3, 200, 500, 0.5, 0.3, 50, 3));
        addSkill(CDoubleAttackPas::create(10));
        //addSkill(CThumpPas::create(10, CExtraCoeff(20, 0), 2));
        addSkill(CHpChangePas::create(1, 1, true, -1));
        addSkill(CVampirePas::create(0.2));
        
        
        CCSkillButtonBase* pSkillBtn;
        CSkill* pSkill;
        CBuffSkill* pBuff;
        
        CScatterFireAct* pSfAct = CScatterFireAct::create(10, 10, 5, 15, 600, 500, 0.5, 0.3);
        addSkill(pSfAct);
        pSkillBtn = M_CREATE_SKILL("skill3", pGm->m_iPlayerKey, pSfAct->getKey(), NULL);
        pGm->m_pCtrlLayer->m_pSkillPanel->addButton(pSkillBtn, 0, 1);

        pBuff = CHarmShieldBuff::create(5, false, CExtraCoeff(0, 0), true);
        pSm->addSkill(pBuff);
        pSkill = CSelfBuffMakerAct::create(30, pBuff->getKey(), 1);
        addSkill(pSkill);
        pSkillBtn = M_CREATE_SKILL("skill1", pGm->m_iPlayerKey, pSkill->getKey(), NULL);
        pGm->m_pCtrlLayer->m_pSkillPanel->addButton(pSkillBtn, 0, 3);

        pBuff = CHpChangeBuff::create(10, true, 0, 0.1, -1.7, false, 1);
        pSm->addSkill(pBuff);
        pSkill = CHomingMissileAct::create(20, 20, 1, pBuff->getKey(), 1);
        addSkill(pSkill);
        pSkillBtn = M_CREATE_SKILL("skill4", pGm->m_iPlayerKey, pSkill->getKey(), NULL);
        pGm->m_pCtrlLayer->m_pSkillPanel->addButton(pSkillBtn, 0, 2);

        pBuff = CHarmShieldBuff::create(5, false, CExtraCoeff(0, 0), true);
        //pBuff = CStunBuff::create(1, false);
        pSm->addSkill(pBuff);
        pSkill = CAuraPas::create(200, CAuraPas::kAlly, 1, pBuff->getKey(), 1);
        addSkill(pSkill);

		CCMenuItemImage *pPropImg = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseNormal.png",
			NULL,
			NULL);
		pGm->m_pCtrlLayer->m_pPackagePanel->addProp(pPropImg, 0, 1);
		CUnitPackage* pPackage = CUnitPackage::create(4);
		pPackage->retain();
		addPackage(pPackage);
		pGm->setPackage(pPackage);
		CAttributeProp * pProp = CAttributeProp::create(0, 12, 1, 0, 0, 1);
		pPackage->addProp(pProp);
    }
    else
    {
        setFireRange(400);
        addSkill(CDoubleAttackPas::create(80));
        addSkill(CThumpPas::create(10, CExtraCoeff(10, 0), 0, 1));
        addSkill(CVampirePas::create(0.5));
    }
    
}

float CTankDemoByTS::getDeflection(CTankInfo* pTank) const
{
    S_POINT oPos = pTank->getPosition();
    S_POINT oTankLoc = getPosition();
    float fA = CC_RADIANS_TO_DEGREES(-ccpToAngle(ccp(oPos.x - oTankLoc.x, oPos.y - oTankLoc.y)));
    return fA;
}

void CTankDemoByTS::skill1( int iKey )
{
    dynamic_cast<CCSkillTankSprite*>(getDisplayBody())->skill1(15, iKey);
}

void CTankDemoByTS::skill2( float fDuration )
{
    dynamic_cast<CCSkillTankSprite*>(getDisplayBody())->skill2(fDuration);
}

void CTankDemoByTS::updateMaxExp()
{
    m_dwMaxExp = (m_dwLvl / 1.7) * 500;
}

void CTankDemoByTS::onLevelChange( int32_t iChanged )
{
    setMaxHp(m_dwLvl * 300 + 150);
}

