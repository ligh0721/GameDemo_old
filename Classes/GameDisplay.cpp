#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "SkillUI.h"
#include "GameCtrl.h"


CCEventTriggerInterface::~CCEventTriggerInterface()
{
}

CCLogicInfoInterface::CCLogicInfoInterface()
    : m_pLogicBody(NULL)
{
}

CCLogicInfoInterface::~CCLogicInfoInterface()
{
    delete m_pLogicBody;
}

CCProjectileWithAttackData::CCProjectileWithAttackData()
    : m_pAttackData(NULL)
    , m_iGeneration(1)
{
}

CCProjectileWithAttackData::~CCProjectileWithAttackData()
{
    CC_SAFE_RELEASE(m_pAttackData);
}

CAttackData* CCProjectileWithAttackData::getAttackData() const
{
    return m_pAttackData;
}

void CCProjectileWithAttackData::setAttackData( CAttackData* pAttackData )
{
    if (pAttackData != m_pAttackData)
    {
        CC_SAFE_RELEASE(m_pAttackData);
    }

    m_pAttackData = pAttackData;
    CC_SAFE_RETAIN(m_pAttackData);
}

CGameLogicObject* CCLogicInfoInterface::getLogicBody() const
{
    return m_pLogicBody;
}

void CCLogicInfoInterface::setLogicBody( CGameLogicObject* pLogicBody )
{
    m_pLogicBody = pLogicBody;
}

CCGameSprite::CCGameSprite()
    : m_fLineVelocity(0.0)
    , m_fAngularVelocity(0.0)
    , m_dwDoingFlags(0)
    , m_pShadowNode(NULL)
{
}

CCGameSprite::~CCGameSprite()
{
    delShadowNode();
}

void CCGameSprite::setPosition( const CCPoint& roPos )
{
    if (m_pShadowNode)
    {
        m_pShadowNode->setPosition(roPos);
    }
    CCSprite::setPosition(roPos);
}

float CCGameSprite::reviseRotation(float fDegree)
{
    if (fDegree < -180)
    {
        fDegree += static_cast<int>(floor(180.0 - fDegree)) / 360 * 360;
    }
    else if (fDegree > 180)
    {
        fDegree -= static_cast<int>(ceil(180.0 + fDegree)) / 360 * 360;
    }
    return fDegree;
}

void CCGameSprite::startDoing( uint32_t dwMask )
{
    m_dwDoingFlags |= dwMask;
}

void CCGameSprite::endDoing( uint32_t dwMask )
{
    m_dwDoingFlags &= (~dwMask);
}

bool CCGameSprite::isDoingOr( uint32_t dwMask ) const
{
    return (m_dwDoingFlags & dwMask) != 0;
}

bool CCGameSprite::isDoingAnd( uint32_t dwMask ) const
{
    return (m_dwDoingFlags & dwMask) == dwMask;
}

bool CCGameSprite::isDoingNothing() const
{
    return m_dwDoingFlags == 0;
}

CCShadowNode* CCGameSprite::getShadowNode()
{
    if (!m_pShadowNode)
    {
        setShadowNode(CCShadowNode::create(CCGameManager::sharedGameManager()->m_pBattleGroundLayer));
    }
    return m_pShadowNode;
}

void CCGameSprite::setShadowNode( CCShadowNode* pShadowNode )
{
    CCAssert(!m_pShadowNode && pShadowNode, "invalid call");
    m_pShadowNode = pShadowNode;
    m_pShadowNode->setOwner(this);
}

void CCGameSprite::delShadowNode()
{
    if (!m_pShadowNode)
    {
        return;
    }
    m_pShadowNode->setOwner(NULL);
    m_pShadowNode->removeFromParentAndCleanup(true);
    m_pShadowNode = NULL;
}

CCShadowNode::~CCShadowNode()
{
    if (m_pOwner)
    {
        m_pOwner->delShadowNode();
    }
}

bool CCShadowNode::init( CCNode* pParent )
{
    pParent->addChild(this);
    return true;
}

CCGameSprite* CCShadowNode::getOwner()
{
    return m_pOwner;
}

void CCShadowNode::setOwner( CCGameSprite* pOwner )
{
    m_pOwner = pOwner;
    if (m_pOwner)
    {
        setAnchorPoint(pOwner->getAnchorPoint());
        setContentSize(pOwner->getContentSize());
        setPosition(pOwner->getPosition());
    }
}

CCRadarStreak::CCRadarStreak()
    : m_pSprite(NULL)
    , m_fRadius(0.0)
    , m_fAngle(0.0)
{
}

CCRadarStreak* CCRadarStreak::create(float fade, float minSeg, float stroke, ccColor3B color, const char* path, CCSprite* pSprite)
{
    CCRadarStreak *pRet = new CCRadarStreak();
    if (pRet && pRet->initWithFade(fade, minSeg, stroke, color, path))
    {
        pRet->m_pSprite = pSprite;
        pRet->m_fRadius = 96;
        pRet->m_fAngle = 0.0f;
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return NULL;
}

void CCRadarStreak::updateStreak( float dt )
{
    CCPoint oPos = m_pSprite->getPosition();
    m_fAngle += 10.0f;
    setPosition(ccp(oPos.x + cosf(m_fAngle / 180 * M_PI)*m_fRadius, oPos.y + sinf(m_fAngle/ 180 * M_PI)*m_fRadius));
}

CCGameManager* CCGameManager::m_pInst = NULL;

CCGameManager::CCGameManager()
    : m_pLogicBody(NULL)
    , m_pBattleGroundLayer(NULL)
    , m_pCtrlLayer(NULL)
    , m_pBackGroundLayer(NULL)
    , m_pTankBatch(NULL)
    , m_pBulletBatch(NULL)
    , m_pParticleBatch(NULL)
    , m_pAudio(NULL)
    , m_iPlayerKey(0)
    , m_bTurnOnVoice(false)
    , m_fMaxBonusDist(400)
	, m_iTotalChapter(0)
	, m_iCompletedChapter(0)
{
}

CCGameManager::~CCGameManager()
{
    delete m_pLogicBody;
}

bool CCGameManager::init()
{
    srand(time(NULL));

    m_pLogicBody = new CGameControl();
    m_pLogicBody->setGameManager(this);

    m_pAudio = SimpleAudioEngine::sharedEngine();

	m_iTotalChapter = 18;
	m_iCompletedChapter = 4;
    return true;
}

CCGameManager* CCGameManager::sharedGameManager()
{
    if (m_pInst)
    {
        return m_pInst;
    }

    m_pInst = CCGameManager::create();
    CC_SAFE_RETAIN(m_pInst);
    return m_pInst;
}

void CCGameManager::addBattleGroundLayer( CCScene* pTargetSence )
{
    m_pBattleGroundLayer = CCBattleGroundLayer::create();
    pTargetSence->addChild(m_pBattleGroundLayer);

    m_pTankBatch = CCSpriteBatchNode::create("tank.png");
    m_pBulletBatch = CCSpriteBatchNode::createWithTexture(m_pTankBatch->getTexture());

    m_pParticleBatch = CCParticleBatchNode::createWithTexture(CCTextureCache::sharedTextureCache()->addImage("particle/fire.png"));

    m_pBattleGroundLayer->addChild(m_pTankBatch);
    m_pBattleGroundLayer->addChild(m_pBulletBatch);
    m_pBattleGroundLayer->addChild(m_pParticleBatch);

    CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();

    const int iImageCount = 18;
    CCArray* pArr = CCArray::createWithCapacity(iImageCount);
    for (int i = 0; i < iImageCount; i++)
    {
        char szName[32];
        sprintf(szName, "explode%d.png", i);
        CCSpriteFrame* pFrame = pCache->spriteFrameByName(szName);
        //pFrame->setOriginalSize(CCSize(10, 10));
        pArr->addObject(pFrame);
    }

    CCAnimation* pAni = CCAnimation::createWithSpriteFrames(pArr, 0.05f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(pAni, "explode");
}

void CCGameManager::addCtrlLayer( CCScene* pTargetSence )
{
    m_pCtrlLayer = CCCtrlLayer::create();
    pTargetSence->addChild(m_pCtrlLayer, 10);
}

void CCGameManager::addBackGroundLayer( CCScene* pTargetSence )
{
    m_pBackGroundLayer = CCBackGroundLayer::createWithColor(ccc4(204, 232, 207, 64));
    pTargetSence->addChild(m_pBackGroundLayer, -10);
}

void CCGameManager::addTank( CCTankSprite* pTank, const CCPoint& oPos, float fRotate )
{
    //CCNormalTankSprite* pTank = CCNormalTankSprite::createWithSpriteFrameNameAndLogicGm(pBodyFrameName, this);
    //CCSprite* pGun = CCSprite::createWithSpriteFrameName(pGunFrameName);
    //pTank->addChild(pGun, 0, 0);
    //CCRadarStreak* pRadar = CCRadarStreak::create(0.25f, 10, 128, ccc3(255, 255, 0), "radar.png", pTank);
    //m_pLayer->addChild(pRadar, -1);
    //pRadar->schedule(schedule_selector(CCRadarStreak::updateStreak), 0);
    //pTank->setScale(1.3);
    m_pTankBatch->addChild(pTank, 0, pTank->getLogicBody()->getLogicKey());
    pTank->setPosition(oPos);
    pTank->setRotation(fRotate);
    pTank->scheduleUpdate();

    pTank->triggerOnInit();

    int iCount = getTankCount();
    for (int i = 0; i < iCount; i++)
    {
        CCTankSprite* pOtherTank = getTankByIndex(i);
        if (pOtherTank != pTank)
        {
            pOtherTank->triggerOnJionGame(pTank);
        }
    }
}

void CCGameManager::addBullet( CCBulletSprite* pBullet, CCTankSprite* pTank )
{
    m_pBulletBatch->addChild(pBullet);
    CCSprite* pGun = pTank->getGun();
    CCPoint oGunPos = pGun->getPosition();
    CCSize oGunSize = pGun->getContentSize();
    CCPoint oPos = m_pBattleGroundLayer->convertToNodeSpace(pGun->convertToWorldSpace(ccp(oGunSize.width, oGunSize.height / 2)));
    float fDegree = pTank->getRotation() + pTank->getGun()->getRotation();
    pBullet->setPosition(oPos);
    pBullet->setRotation(fDegree);
    dynamic_cast<CBullet*>(pBullet->getLogicBody())->setOwnerKey(pTank->getKey());
    pBullet->scheduleUpdate();
}

CGameControl* CCGameManager::getLogicBody() const
{
    return m_pLogicBody;
}

int CCGameManager::getBulletCount() const
{
    return m_pBulletBatch->getChildrenCount();
}

int CCGameManager::getTankCount() const
{
    return m_pTankBatch->getChildrenCount();
}

CCBulletSprite* CCGameManager::getBulletByIndex( int iIndex )
{
    return dynamic_cast<CCBulletSprite*>(m_pBulletBatch->getChildren()->objectAtIndex(iIndex));
}

CCTankSprite* CCGameManager::getTankByIndex( int iIndex )
{
    return dynamic_cast<CCTankSprite*>(m_pTankBatch->getChildren()->objectAtIndex(iIndex));
}

CCTankSprite* CCGameManager::getTankByKey( int iKey )
{
    return dynamic_cast<CCTankSprite*>(m_pTankBatch->getChildByTag(iKey));
}

CSkill* CCGameManager::getSkillByKey( int iOwner, int iKey )
{
    CCTankSprite* pTank = getTankByKey(iOwner);
    if (!pTank)
    {
        return NULL;
    }
    CUnit* pOwner = dynamic_cast<CUnit*>(pTank->getLogicBody());
    return pOwner->getSkill(iKey);
}

CBuffSkill* CCGameManager::getBuffByKey( int iOwner, int iKey )
{
    CCTankSprite* pTank = getTankByKey(iOwner);
    if (!pTank)
    {
        return NULL;
    }
    CUnit* pOwner = dynamic_cast<CUnit*>(pTank->getLogicBody());
    return pOwner->getBuff(iKey);
}

void CCGameManager::setVoice( bool bTurnOn )
{
    m_bTurnOnVoice = bTurnOn;
}

bool CCGameManager::isVoiceEnabled() const
{
    return m_bTurnOnVoice;
}

void CCGameManager::preloadEffectSound( const char* pEffect )
{
    m_pAudio->preloadEffect(pEffect);
}

void CCGameManager::preloadBackgroundSound( const char* pBackground )
{
    m_pAudio->preloadBackgroundMusic(pBackground);
}

void CCGameManager::playEffectSound( const char* pEffect, bool bLoop )
{
    if (isVoiceEnabled())
    {
        m_pAudio->playEffect(pEffect, bLoop);
    }
}

void CCGameManager::playBackgroundSound( const char* pBackground, bool bLoop )
{
    if (isVoiceEnabled())
    {
        m_pAudio->playBackgroundMusic(pBackground, bLoop);
    }
}

void CCGameManager::setPackage( CUnitPackage* pPackage )
{
	m_pPackage = pPackage;
}

void CCGameManager::pushScene( CCScene* pScene )
{
	if(m_oArrScene.count() < 1)
	{

		m_oArrScene.addObject(CCDirector::sharedDirector()->getRunningScene());
	}
	m_oArrScene.addObject(pScene);
	CCDirector::sharedDirector()->replaceScene(pScene);

}

void CCGameManager::popSceneWithTrans(float duration)
{
	CCAssert(m_oArrScene.count() > 1, "scene count must greate 1");
	m_oArrScene.removeObjectAtIndex(m_oArrScene.count() - 1);
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(duration, dynamic_cast<CCScene*>(m_oArrScene.objectAtIndex(m_oArrScene.count() - 1))));
}

CCSkillManager::CCSkillManager()
{
}

CCSkillManager::~CCSkillManager()
{
}

bool CCSkillManager::init()
{
    m_oArrSkill.init();
    return true;
}

CCSkillManager* CCSkillManager::m_pInst = NULL;

CCSkillManager* CCSkillManager::sharedSkillManager()
{
    if (m_pInst)
    {
        return m_pInst;
    }

    m_pInst = CCSkillManager::create();
    CC_SAFE_RETAIN(m_pInst);
    return m_pInst;
}

void CCSkillManager::cleanUp()
{
    m_oArrSkill.removeAllObjects();
}

int CCSkillManager::addSkill( CSkill* pSkill )
{
    m_oArrSkill.addObject(pSkill);
    return pSkill->getKey();
}

CSkill* CCSkillManager::getSkill( int iKey )
{
    CSkill* pSkill = NULL;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrSkill, pObj)
    {
        pSkill = dynamic_cast<CSkill*>(pObj);
        if (pSkill->getKey() == iKey)
        {
            return pSkill;
        }
    }
    return NULL;
}

CSkill* CCSkillManager::copySkill( int iKey )
{
    CSkill* pSkill = getSkill(iKey);
    if (!pSkill)
    {
        return NULL;
    }
    return dynamic_cast<CSkill*>(pSkill->copy());
}

CSkill* CCSkillManager::copySkill( CSkill* pSkill )
{
    return dynamic_cast<CSkill*>(pSkill->copy());
}

CCSkillInfoManager* CCSkillInfoManager::m_pInst = NULL;

CCSkillInfoManager::CCSkillInfoManager()
{
}

CCSkillInfoManager::~CCSkillInfoManager()
{
}

bool CCSkillInfoManager::init()
{
    m_oArrSkillInfo.init();
    return true;
}

CCSkillInfoManager* CCSkillInfoManager::sharedSkillInfoManager()
{
    if (m_pInst)
    {
        return m_pInst;
    }

    m_pInst = CCSkillInfoManager::create();
    CC_SAFE_RETAIN(m_pInst);
    return m_pInst;
}

void CCSkillInfoManager::cleanUp()
{
    m_oArrSkillInfo.removeAllObjects();
}

void CCSkillInfoManager::addSkillInfo( CSkillInfo* pSkillInfo )
{
    m_oArrSkillInfo.addObject(pSkillInfo);
}

CSkillInfo* CCSkillInfoManager::getSkillInfo( int iKey )
{
    CSkillInfo* pSkillInfo = NULL;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrSkillInfo, pObj)
    {
        pSkillInfo = dynamic_cast<CSkillInfo*>(pObj);
        if (pSkillInfo->getKey() == iKey)
        {
            return pSkillInfo;
        }
    }
    return NULL;
}

CCSkillButtonBase::CCSkillButtonBase()
    : m_pTarget(NULL)
    , m_iClickRetCode(0)
{
}

CCSkillButtonBase::~CCSkillButtonBase()
{

}

void CCSkillButtonBase::onCDBlickDone(CCNode* pNode)
{
    if (m_pBlink)
    {
        m_pBlink->setVisible(false);
    }
}

bool CCSkillButtonBase::init(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished)
{
	CCSprite* pNormalSprite = NULL;
	CCSprite* pSelectedSprite = NULL;
	CCSprite* pDisabledSprite = NULL;
	m_pDisableImageFrameName = pDisabledImage;
	m_pNormalImageFrameName = pNormalImage;
	m_pSelectImageFrameName = pSelectedImage;
	
	pNormalImage && (pNormalSprite = CCSprite::createWithSpriteFrameName(pNormalImage));
	pSelectedImage && (pSelectedSprite = CCSprite::createWithSpriteFrameName(pSelectedImage));
	pDisabledImage && (pDisabledSprite = CCSprite::createWithSpriteFrameName(pDisabledImage));
	if (!initWithNormalSprite(pNormalSprite, pSelectedSprite, pDisabledSprite, this, menu_selector(CCSkillButtonBase::onClick)))
	{
		return false;
	}
    m_pTarget = pTarget;
    m_pOnClick = pOnClick;
    m_pOnFinished = pOnFinished;
    setCoolDown(fCoolDown);
    CCSize sz = getContentSize();
    CCPoint oAp = getAnchorPointInPoints();

    // Create ProgressTimer
    m_pPt = CCProgressTimer::create((CCSprite*)getNormalImage());
    addChild(m_pPt);
    m_pPt->setPosition(oAp);
    m_pPt->setVisible(false);
    m_pPt->setType(kCCProgressTimerTypeRadial);

    // Create Blink Image
    if (pBlinkImage)
    {
        m_pBlink = CCSprite::createWithSpriteFrameName(pBlinkImage);
        addChild(m_pBlink, 10);
        m_pBlink->setPosition(oAp);
        m_pBlink->setVisible(false);
    }
    else
    {
        m_pBlink = NULL;
    }

    if (pMaskImage)
    {
        m_pMask = CCSprite::createWithSpriteFrameName(pMaskImage);
        addChild(m_pMask);
        m_pMask->setPosition(oAp);
        m_pMask->setVisible(false);
    }
    else
    {
        m_pMask = NULL;
    }

    // Create Label
    //m_pLabel = CCLabelTTF::create("5", "", 24, CCSizeMake(sz.width - 8, 32), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom); //CCLabelBMFont::create("88", "fonts/futura-48.fnt", sz.width - 6, kCCTextAlignmentRight);//, CCSizeMake(getContentSize().width - 6, 16.0), kCCTextAlignmentRight, "", 16.0
    //addChild(m_pLabel);
    //CCSize szLbl = m_pLabel->getContentSize();
    //m_pLabel->setPosition(ccp(sz.width - szLbl.width / 2 - 4, szLbl.height / 2));
    //m_pLabel->setColor(ccRED);

    return true;
}

void CCSkillButtonBase::onClick(CCObject* pObject)
{
    setClickRetCode(0);
    if (m_pTarget && m_pOnClick)
    {
        (m_pTarget->*m_pOnClick)(this);
    }
    if (getClickRetCode() < 0)
    {
        return;
    }

}

void CCSkillButtonBase::onCoolDownDone(CCNode* pNode)
{
    if (getCoolDown())
    {
        m_pMask->setVisible(false);
        m_pPt->setVisible(false);
        this->setEnabled(true);
        if (m_pBlink)
        {
            m_pBlink->setVisible(true);
            m_pBlink->runAction(CCSequence::create(CCFadeIn::create(0.25f), CCFadeOut::create(0.25f), CCCallFuncN::create(this, callfuncN_selector(CCSkillButtonBase::onCDBlickDone)), NULL));
        }
    }

    if (m_pTarget && m_pOnFinished)
    {
        (m_pTarget->*m_pOnFinished)(this);
    }
}

void CCSkillButtonBase::coolDown()
{
    if (getCoolDown())
    {
        m_pMask->setVisible(true);
        m_pPt->setVisible(true);
        this->setEnabled(false);
        dynamic_cast<CCSprite*>(m_pDisabledImage)->setOpacity(0x50);
        m_pDisabledImage->runAction(CCFadeTo::create(getCoolDown(), 0xFF));
        //this->getDisabledImage()->runAction()

        m_pPt->runAction(CCSequence::create(CCProgressFromTo::create(getCoolDown(), 0.0f, 100.0f), CCCallFuncN::create(this, callfuncN_selector(CCSkillButtonBase::onCoolDownDone)), NULL));
    }
    else
    {
        onCoolDownDone(this);
    }
}

bool CCSkillButtonNormalWithInfo::init( const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iInfoKey )
{
    setInfoKey(iInfoKey);
    CCSkillInfoManager* pSim = CCSkillInfoManager::sharedSkillInfoManager();
    const char* pBtnImg = pSim->getSkillInfo(m_iInfoKey)->getButtonImage();
    char szNormal[256];
    char szSelected[256];
    char szDisabled[256];
    sprintf(szNormal, "%s/BTN%s.png", pBtnImg, pBtnImg);
    sprintf(szSelected, "%s/BTN%sDOWN.png", pBtnImg, pBtnImg);
    sprintf(szDisabled, "%s/DISBTN%s.png", pBtnImg, pBtnImg);
    
    pSim->getSkillInfo(m_iInfoKey);
    CCSkillButtonNormal::init(szNormal, szSelected, szDisabled, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    
    return true;
}

CCSkillButtonAdvance::CCSkillButtonAdvance()
    : m_iSkillKey(0)
{
}

CCSkillButtonAdvance::~CCSkillButtonAdvance()
{
}

bool CCSkillButtonAdvance::init(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, int iUnitKey, int iSkillKey, CCUnitLayer* pUnitLayer )
{
    m_bPressed = false;
    setUnitLayer(pUnitLayer);
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(iUnitKey);
    if (!pUnit)
    {
        return false;
    }
    CSkill* pSkill = pUnit->getSkill(iSkillKey);
    if (!pSkill)
    {
        return false;
    }
    m_iUnitKey = iUnitKey;
    CCSkillButtonBase::init(pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, pSkill->getCoolDown(), this, callfuncN_selector(CCSkillButtonAdvance::onSkillClick), callfuncN_selector(CCSkillButtonAdvance::onSkillFinished));

    m_iSkillKey = iSkillKey;
    pSkill->setDisplayBody(this);
    return true;
}

void CCSkillButtonAdvance::setCoolDown( float fDuration )
{
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(m_iUnitKey);
    if (!pUnit)
    {
        return;
    }
    CSkill* pSkill = pUnit->getSkill(m_iSkillKey);
    if (!pSkill)
    {
        return;
    }
    pSkill->setCoolDown(fDuration);
}

float CCSkillButtonAdvance::getCoolDown() const
{
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(m_iUnitKey);
    if (!pUnit)
    {
        return -1;
    }
    CSkill* pSkill = pUnit->getSkill(m_iSkillKey);
    if (!pSkill)
    {
        return -1;
    }
    return pSkill->getCoolDown();
}

void CCSkillButtonAdvance::onSkillClick( CCNode* pNode )
{
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(m_iUnitKey);
    if (!pUnit || pUnit->isDead() || pUnit->isDoingOr(CGameUnit::kSuspended))
    {
        M_SKILL_BUTTON_CANCEL(pNode);
        return;
    }
    CSkill* pSkill = pUnit->getSkill(m_iSkillKey);
    if (!pSkill)
    {
        M_SKILL_BUTTON_CANCEL(pNode);
        return;
    }
    CActiveSkill* pAct = dynamic_cast<CActiveSkill*>(pSkill);
    if (!pAct)
    {
        M_SKILL_BUTTON_CANCEL(pNode);
        return;
    }
    M_DEF_FC(pFc);
    switch (pAct->getCastTargetType())
    {
    case CActiveSkill::kPointTarget:
    case CActiveSkill::kUnitTarget:
        if (isPressed())
        {
            // 此次为取消操作
            pUnit->getUnitLayer()->endOrderUnitToCast();
        }
        else
        {
            pUnit->getUnitLayer()->preOrderUnitToCast(m_iUnitKey, m_iSkillKey);
            setPressed();
        }
        
        return;
    }

    pUnit->getUnitLayer()->preOrderUnitToCast(m_iUnitKey, m_iSkillKey);
    if (!pUnit->cast())
    {
        M_SKILL_BUTTON_CANCEL(pNode);
        getUnitLayer()->endOrderUnitToCast();
        return;
    }
    getUnitLayer()->endOrderUnitToCast();
}

void CCSkillButtonAdvance::onSkillFinished( CCNode* pNode )
{
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(m_iUnitKey);
    if (!pUnit)
    {
        return;
    }
    CSkill* pSkill = pUnit->getSkill(m_iSkillKey);
    if (!pSkill)
    {
        return;
    }
    if (pSkill->getOwner())
    {
        pSkill->setCoolDownLeft(0);
        pSkill->onSkillReady();
    }
}

void CCSkillButtonAdvance::setPressed( CCSpriteFrame* pFrame )
{
    if (pFrame)
    {
        setNormalSpriteFrame(pFrame);
    }
    else
    {
        M_DEF_FC(pFc);
        setNormalSpriteFrame(pFc->spriteFrameByName(m_pNormalImageFrameName));
    }
    m_bPressed = (pFrame != NULL);
}

bool CCSkillButtonAdvance::isPressed() const
{
    return m_bPressed;
}

bool CCButtonPanel::init( int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName )
{
    m_iRow = iRow;
    m_iLine = iLine;
    m_fButtonWidth = fButtonWidth;
    m_fBorderWidth = fBorderWidth;
    m_fInnerBorderWidth = fInnerBorderWidth;
    if (pBackgroundFrameName)
    {
        m_pBackground = CCSprite::createWithSpriteFrameName(pBackgroundFrameName);
        addChild(m_pBackground);
        m_pBackground->setPosition(getAnchorPointInPoints());
    }
    else
    {
        m_pBackground = NULL;
// 		m_pBackground=CCSprite::create("HelloWorld.png",CCRectMake(0,0,84,84));
// 		addChild(m_pBackground);
// 		m_pBackground->setPosition(getAnchorPointInPoints());
    }
    
    CCSize oSz = CCSizeMake(m_fBorderWidth * 2 + m_fInnerBorderWidth * (m_iLine - 1) + m_fButtonWidth * m_iLine, m_fBorderWidth * 2 + m_fInnerBorderWidth * (m_iRow - 1) + m_fButtonWidth * m_iRow);
    setContentSize(oSz);
    

    m_pSkillMenu = CCMenu::create();
    addChild(m_pSkillMenu);
    m_pSkillMenu->setContentSize(getContentSize());
    m_pSkillMenu->setPosition(CCPointZero);

    return true;
}

void CCButtonPanel::addButton( CCSkillButtonBase* pButton, int iX, int iY )
{
    m_pSkillMenu->addChild(pButton);
    float fTmp = m_fButtonWidth + m_fInnerBorderWidth;
    float fTmp2 = m_fBorderWidth + m_fButtonWidth / 2;
    CCSize oSz = getContentSize();
    pButton->setPosition(ccp(iX * fTmp - oSz.width / 2 + fTmp2, iY * fTmp - oSz.height / 2 + fTmp2));
}

void CCButtonPanel::delButton( CCSkillButtonBase* pButton )
{
    m_pSkillMenu->removeChild(pButton, true);
}


CCProgressBar::CCProgressBar()
{
}

CCProgressBar::~CCProgressBar()
{
}

bool CCProgressBar::init( const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop )
{
    setAnchorPoint(ccp(0.5, 0.5));
    setContentSize(roSize);

    CCSize oFillSz(roSize.width - fVertBorderWidth * 2, roSize.height - fHorizBorderWidth * 2);

    m_oPt.initWithSprite(pFill);
    addChild(&m_oPt, bFillOnTop);
    CCSize oSz = m_oPt.getContentSize();
    m_oPt.setPosition(getAnchorPointInPoints());
    m_oPt.setScaleX(oFillSz.width / oSz.width);
    m_oPt.setScaleY(oFillSz.height / oSz.height);
    m_oPt.setType(kCCProgressTimerTypeBar);
    m_oPt.setPercentage(0);
    m_oPt.setBarChangeRate(ccp(1, 0));
    m_oPt.setMidpoint(ccp(0, 0));

    if (pBorder)
    {
        addChild(pBorder, !bFillOnTop);
        oSz = pBorder->getContentSize();
        pBorder->setPosition(getAnchorPointInPoints());
        pBorder->setScaleX(roSize.width / oSz.width);
        pBorder->setScaleY(roSize.height / oSz.height);
    }
    
    return true;
}

void CCProgressBar::setPercentage( float fPercent )
{
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    //m_oHd.setPosition()
    m_oPt.setPercentage(fPercent);
}

void CCProgressBar::setPercentage( float fPercent, float fDuration, CCFiniteTimeAction* pEndAction /*= NULL*/ )
{
    m_oPt.stopAllActions();
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    float fWidth = m_oPt.getScaleX() * m_oPt.getContentSize().width;
    m_oPt.runAction(CCSequence::create(CCProgressTo::create(fDuration, fPercent), pEndAction, NULL));
}

void CCProgressBar::setFillColor( const ccColor3B& roColor )
{
    m_oPt.setColor(roColor);
}

const float CCProgressBar::CONST_MAX_PROCESS_BAR_PERCENT = 99.99999;

bool CCPackagePanel::init( int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName )
{
	m_iRow = iRow;
	m_iLine = iLine;
	m_fButtonWidth = fButtonWidth;
	m_fBorderWidth = fBorderWidth;
	m_fInnerBorderWidth = fInnerBorderWidth;
	m_pBackground = CCSprite::createWithSpriteFrameName(pBackgroundFrameName);
	addChild(m_pBackground);
	CCSize oSz = CCSizeMake(m_fBorderWidth * 2 + m_fInnerBorderWidth * (iRow - 1) + m_fButtonWidth * iRow, m_fBorderWidth * 2 + m_fInnerBorderWidth * (iLine - 1) + m_fButtonWidth * iLine);
	setContentSize(oSz);
	m_pBackground->setPosition(getAnchorPointInPoints());;

	m_pPropMenu = CCMenu::create();
	addChild(m_pPropMenu);
	m_pPropMenu->setContentSize(getContentSize());
	m_pPropMenu->setPosition(CCPointZero);

	return true;
}

void CCPackagePanel::addProp(CCMenuItem* pItem, int iX, int iY)
{
	m_pPropMenu->addChild(pItem);
	float fTmp = m_fButtonWidth + m_fInnerBorderWidth;
	float fTmp2 = m_fBorderWidth + m_fButtonWidth / 2;
	CCSize oSz = getContentSize();
	pItem->setPosition(ccp(iX * fTmp - oSz.width / 2 + fTmp2, iY * fTmp - oSz.height / 2 + fTmp2));
}

void CCPackagePanel::delProp(CCMenuItem* pItem)
{
	m_pPropMenu->removeChild(pItem, true);
}

CCPropItemImg::CCPropItemImg()
	:m_iPropKey(0)
{

}

CCPropItemImg::~CCPropItemImg()
{

}

bool CCPropItemImg::init( const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, int iPropKey )
{
	CCSkillButtonBase::init(pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, 
		pMaskImage, 0, this, callfuncN_selector(CCPropItemImg::onPropItemClick), callfuncN_selector(CCPropItemImg::onPropItemFinished));
	m_iPropKey = iPropKey;
	return true;
	
}

void CCPropItemImg::onPropItemClick( CCNode* pNode )
{
	
}

void CCPropItemImg::onPropItemFinished( CCNode* pNode )
{

}

void CCPropItemImg::setCoolDown( float fDuration )
{

}

float CCPropItemImg::getCoolDown() const
{
	return 0.0;
}

CCSkillButtonNormal* CCSkillButtonNormal::copyImg(CCObject* target, SEL_CallFuncN callOnClick, SEL_CallFuncN callOnFinished )
{
	CCSkillButtonNormal* pButtonRet = CCSkillButtonNormal::create(this->m_pNormalImageFrameName,this->m_pSelectImageFrameName,this->m_pDisableImageFrameName,NULL,NULL,this->getCoolDown(),target,callOnClick,callOnFinished);
// 	pButtonRet->setNormalImage(this->getNormalImage());
// 	pButtonRet->setSelectedImage(this->getSelectedImage());
// 	pButtonRet->setDisabledImage(this->getDisabledImage());
	return pButtonRet;
}

bool CCGameChapterButton::init( const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iLevel )
{
	CCSkillButtonNormal::init(pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
	m_iLevel = iLevel;
	return true;
}


bool CCCommmButton::init( const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iKey )
{
	CCSkillButtonNormal::init(pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
	m_iKey = iKey;
	return true;
}
