#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "Action.h"
#include "SkillInfo.h"
#include "GameCtrl.h"
#include "MainScene.h"
#include "WHomeScene.h"
#include "MissionInfo.h"
#include "ChildOfGameUnit.h"
#include "UnitInfo.h"
#include "SomeScene.h"
#include "TechTreeScene.h"
#include "SkillInfo.h"
#include "Tower.h"


bool CCWHomeScene::init()
{
    CCScene::init();
    m_pHomeSceneLayer = CCWHomeSceneLayer::create();
    addChild(m_pHomeSceneLayer,1);
    //     CGameCtrlLayer* testCtrlLayer=CGameCtrlLayer::create();
    //     addChild(testCtrlLayer,2);
    addChild(&m_pHomeSceneLayer->m_oGameCtrlLayer,2);

    return true;
}

CCWHomeSceneLayer::CCWHomeSceneLayer()
{
}

bool CCWHomeSceneLayer::init()
{
    CCWinUnitLayer::initWithColor(ccc4(204, 232, 207, 64));

    //setUnitTickInterval(1.0 / 60);
    M_DEF_GM(pGm);
    M_DEF_OS(pOs);

    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_FC(pFc);
    pFc->addSpriteFramesWithFile("background.plist");
    pFc->addSpriteFramesWithFile("UI.plist");
    pFc->addSpriteFramesWithFile("tank.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("skill.plist");
    //setBackGroundSprite(CCSprite::create("levels/level01/LevelHD.png"));
    setBackGroundSprite(CCSprite::create("LevelDemo2HD.png"));
    setBufferEffectParam(0.9, 10, 0.1);
    m_oMenu.init();
    addChild(&m_oMenu, 0, 5131115);
    m_oMenu.setPosition(CCPointZero);

    m_iTouchActionFlag = 0;
    m_iHero = 0;

    m_oFr.initWithChangeCallback(this, callfuncO_selector(CCWHomeSceneLayer::onGoldChange)); // 势力资源

    m_oCfg.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png", NULL, this, menu_selector(CCWHomeSceneLayer::onBtnCfgClick));
    m_oMenu.addChild(&m_oCfg);
    m_oCfg.setPosition(ccp(oSz.width * 0.9, oSz.height * 0.6));

    m_oStart.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png", NULL, this, menu_selector(CCWHomeSceneLayer::onBtnStartClick));
    m_oMenu.addChild(&m_oStart);
    m_oStart.setPosition(ccp(oSz.width * 0.8, oSz.height * 0.6));

    M_DEF_UPM(pUpm);
    //pUpm->addPatches("heroes.uip");
    m_oUipm.initWithFile("heroes.uip");
    // demo code
    M_DEF_UM(pUm);
    CGameUnit* midTower= pUm->unitByInfo(COrgUnitInfo::kArcane);
    addUnit(midTower);
    midTower->setPosition(ccp(1804,793));
    midTower->setForceByIndex(2);
    midTower->setAlly(1<<2);
    midTower->addSkill(CStatusShowPas::create());
    midTower->addSkill(pOs->skill(COrgSkillInfo::kHpChange2));
    midTower->addSkill(pOs->skill(COrgSkillInfo::kSlowDown1));
    midTower->addSkill(pOs->skill(COrgSkillInfo::kSpeedAura1));
    midTower->setForceResource(&m_oFr);

    midTower= pUm->unitByInfo(COrgUnitInfo::kTesla);
    addUnit(midTower);
    midTower->setPosition(ccp(1704, 793));
    midTower->setForceByIndex(2);
    midTower->setAlly(1<<2);
    midTower->addSkill(CStatusShowPas::create());
    midTower->addSkill(pOs->skill(COrgSkillInfo::kHpChange2));
    midTower->addSkill(pOs->skill(COrgSkillInfo::kImmo1));
    midTower->setForceResource(&m_oFr);


    CGameUnit* heroUnit=getHeroUnit();

    //addUnit(heroUnit);
    heroUnit->setPosition(ccp(804,793));
    heroUnit->setForceByIndex(2);
    heroUnit->setAlly(1<<2);
    heroUnit->addSkill(CStatusShowPas::create());
    heroUnit->addSkill(pOs->skill(COrgSkillInfo::kHero1));
    
    heroUnit->setLevelUpdate(&g_oDemoUpdate);
    heroUnit->setMaxLevel(100);
    heroUnit->setForceResource(&m_oFr);

    m_oGameCtrlLayer.initWithColor(ccc4(0,0,0,0));
    m_oMenuCtrl.init();
    m_oGameCtrlLayer.addChild(&m_oMenuCtrl);
    m_oMenuCtrl.setPosition(CCPointZero);
    m_oGoToTechTree.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png"
        , NULL, this, menu_selector(CCWHomeSceneLayer::onBtnGoClick));
    m_oGoToTechTree.setPosition(ccp(oSz.width - m_oGoToTechTree.getContentSize().width * 0.5 - 10, m_oGoToTechTree.getContentSize().height * 0.5 + 10));
    m_oMenuCtrl.addChild(&m_oGoToTechTree);

    m_oHeroHead.initWithNormalSprite(CCSprite::createWithSpriteFrameName(M_SKILL_PATH("jt")), CCSprite::createWithSpriteFrameName(M_SKILL_DOWN_PATH("jt"))
        , CCSprite::createWithSpriteFrameName(M_SKILL_DIS_PATH("jt")), this, menu_selector(CCWHomeSceneLayer::onBtnHeroClick));
    m_oHeroHead.setPosition(ccp(m_oHeroHead.getContentSize().width * 0.5 + 10, m_oGameCtrlLayer.getContentSize().height - m_oHeroHead.getContentSize().height * 0.5 - 10));
    m_oMenuCtrl.addChild(&m_oHeroHead);

    m_oHpBar.init(CCSizeMake(m_oHeroHead.getContentSize().width, 6), CCSprite::createWithSpriteFrameName("bar_white.png")
        , CCSprite::createWithSpriteFrameName("healthbar_border.png"), 1, 1, true);
    m_oHpBar.setPosition(ccp(m_oHeroHead.getPositionX(), m_oHeroHead.getPositionY() - m_oHeroHead.getContentSize().height * 0.5 - m_oHpBar.getContentSize().height * 0.5));
    m_oGameCtrlLayer.addChild(&m_oHpBar);

    m_oExpBar.init(CCSizeMake(m_oHpBar.getContentSize().width, 6), CCSprite::createWithSpriteFrameName("bar_white.png")
        , CCSprite::createWithSpriteFrameName("healthbar_border.png"), 1, 1, true);
    m_oExpBar.setPosition(ccp(m_oHpBar.getPositionX(), m_oHpBar.getPositionY() - m_oHpBar.getContentSize().height * 0.5 - m_oExpBar.getContentSize().height * 0.5));
    m_oGameCtrlLayer.addChild(&m_oExpBar);

    m_oSkillPanel.init(5, 1, 64, 7, 5, "Skill5_78x352_7_5_black.png");
    m_oGameCtrlLayer.addChild(&m_oSkillPanel);
    m_oSkillPanel.setPosition(ccp(m_oHeroHead.getPositionX(), m_oHeroHead.getPositionY() - m_oSkillPanel.getContentSize().height * 0.5 - 50));

    CActiveSkill* pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kSpeedUp1));
    pSkill->setCastAniInfo(CGameUnit::kAnimationAct3, 0.4);
    heroUnit->addSkill(pSkill);
    CCSkillButtonAdvance* pBtn;
    pBtn = M_CREATE_SKILL("skill2", heroUnit->getKey(), pSkill->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);

    pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kThunderClap1));
    pSkill->setCastAniInfo(CGameUnit::kAnimationAct5, 0.7);
    heroUnit->addSkill(pSkill);
    pBtn = M_CREATE_SKILL("skill1", heroUnit->getKey(), pSkill->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);
    
    pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kFastStrikeBack1));
    pSkill->setCastAniInfo(CGameUnit::kAnimationAct3, 0.4);
    heroUnit->addSkill(pSkill);
    pBtn = M_CREATE_SKILL("skill3", heroUnit->getKey(), pSkill->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);

    pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kShockWave1));
    pSkill->setCastAniInfo(CGameUnit::kAnimationAct2, 0.2);
    dynamic_cast<CProjectileWaveAct*>(pSkill)->setProjectileBirthOffset(ccp(0, 25));
    
    pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kJumpChop1));
    //pSkill->setCastAniInfo(CGameUnit::kAnimationAct2, 0.2);
    heroUnit->addSkill(pSkill);
    pBtn = M_CREATE_SKILL("skill4", heroUnit->getKey(), pSkill->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);

    m_oBuildBtn.init(M_SKILL_PATH("build"), M_SKILL_DOWN_PATH("build"), M_SKILL_DIS_PATH("build"), M_SKILL_PATH("white"), "mask/mask.png", 15, this, callfuncN_selector(CCWHomeSceneLayer::onBtnBuildClick), NULL);
    m_bCanBuild = false;
    //m_oSkillPanel.pushAddButtonExAction(&m_oBuildBtn, CCButtonPanel::kTopToBottom);
    //m_oSkillPanel.pushAddButtonAction(&m_oBuildBtn, 1);

    // for test
    //m_oSkillPanel.pushDelButtonAction(0, CCButtonPanel::kTopToBottom, CCButtonPanel::kLeftToRight, false);
    //m_oSkillPanel.pushDelButtonAction(1, CCButtonPanel::kTopToBottom, CCButtonPanel::kLeftToRight, false);
    //m_oSkillPanel.pushDelButtonAction(2, CCButtonPanel::kTopToBottom, CCButtonPanel::kLeftToRight, false);
    //m_oSkillPanel.pushDelButtonAction(3, CCButtonPanel::kTopToBottom, CCButtonPanel::kLeftToRight, false);
    //m_oSkillPanel.pushClearUpSlotAction();

    //curMission=this->DemoRound();
    m_pCurMission = g_oDemoMission.mission01();

    m_oGold.initWithString("      ", "fonts/Comic Book.ttf", 24, CCSizeMake(100, 48), kCCTextAlignmentLeft);
    //m_oGold.initWithString("55", "fonts/Abberancy.ttf", 24);
    m_oGameCtrlLayer.addChild(&m_oGold);
    m_oGold.setPosition(ccp(oSz.width - 50, oSz.height - 30));
    m_oGold.setColor(ccYELLOW);
    m_oFr.onGoldChange(0);
    //m_oGold.setString(UTEXT("我们"));

    onBtnCfgClick(&m_oCfg);

    pGm->setVoice(false);
    m_iBuffGetting = 0;

    m_oLeftToRevive.initWithString("Revive time: 0s", "", 24);
    m_oGameCtrlLayer.addChild(&m_oLeftToRevive);
    m_oLeftToRevive.setPosition(ccp(oSz.width * 0.8, oSz.height * 0.9));
    m_oLeftToRevive.setVisible(false);

    return true;
}

void CCWHomeSceneLayer::onBtnStartClick(CCObject* pObject)
{
    M_DEF_DR(pDr);
    unschedule(schedule_selector(CCWHomeSceneLayer::onTick));
    pDr->replaceScene(CCTransitionFade::create(0.5, CCWHomeScene::create()));
}

void CCWHomeSceneLayer::onBtnCfgClick(CCObject* pObject)
{
    schedule(schedule_selector(CCWHomeSceneLayer::onTick), 0.8);
}

void CCWHomeSceneLayer::onTick( float fDt )
{
    enum
    {
        kMalik,
        kPaladin,
        kMagnus,
        kMatchstick
    };
    M_DEF_GM(pGm);
    pGm->cmdRecv(0);
    M_DEF_UM(pUm);

    //DemoMission
    int r = m_pCurMission->curRound();
    int n = m_pCurMission->rushCount();
    // static int rushFinishedNumber=0;
    bool bAllRushEnd = true;
    for (int i = 0; i < n; ++i)
    {
        int iRes = m_pCurMission->rush(i, fDt);
        if (iRes >= 0)
        {
            // TODO: createUnitHere;
            bAllRushEnd = false;
            CUnitPath* p = m_pCurMission->pathOfRush(i);
            const CCPoint* pPos = p->getCurTargetPoint(0);
            if (pPos)
            {
                CGameUnit* u = m_pCurMission->m_oUipm.unitByIndex(iRes);
                addUnit(u);
                u->setForceByIndex(3);
                u->setAlly(1<<3);
                u->setPosition(*pPos);
                u->moveAlongPath(p, false);
                u->addSkill(CStatusShowPas::create());
                if (iRes == kPaladin)
                {
                    u->setArmorType(CArmorValue::kHoly);
                    u->setBaseAttackValue(CAttackValue(1, CAttackValue::kHoly, MAX(u->getBaseAttackValue(CAttackValue::kPhysical), u->getBaseAttackValue(CAttackValue::kMagical))));
                }
                //u->setMaxHp(u->getMaxHp() + r * 10);
                //u->setExAttackValue(CAttackValue::kPhysical, CExtraCoeff(1 + r / 10.0, 0));
                //u->setExAttackValue(CAttackValue::kMagical, CExtraCoeff(1 + r / 10.0, 0));
                //u->setExAttackValue(CAttackValue::kHoly, CExtraCoeff(1 + r / 10.0, 0));
            }
        }
        else if (iRes == CGameMission::kNoRound)
        {
            // TODO: roundEnd
            break;
        }
        else if (iRes == CGameMission::kNoUnit)
        {
            // TODO: curRushEnd
            continue;
        }
        else if (iRes == CGameMission::kWaiting)
        {
            // TODO: waitNextUnit
            bAllRushEnd=false;
            continue;
        }
        else
        {
            CCLOG("res(%d) err", iRes);
        }

    }

    if (bAllRushEnd)
    {
        // TODO: curRoundEnd
        m_pCurMission->nextRound();
    }
}

bool CCWHomeSceneLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCWinUnitLayer::ccTouchBegan(pTouch,pEvent);
    M_DEF_GM(pGm);
    CGameUnit* heroUnit = getHeroUnit();
    CCPoint touchPoint = ccpSub(pTouch->getLocation(), getPosition());
#if 0
    if (heroUnit!=NULL&&!heroUnit->isDead())//incomplete
    {
        CCPoint heroPoint=heroUnit->getPosition();
        CCPoint pointDistance=ccpSub(heroPoint,touchPoint);
        float fDistance=sqrtf(pointDistance.x*pointDistance.x+pointDistance.y*pointDistance.y);
        if (fDistance<50)
        {
            m_iTouchActionFlag=1;
        }
    }
#endif

    return true;
}

void CCWHomeSceneLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    CCWinUnitLayer::ccTouchEnded(pTouch,pEvent);
    M_DEF_GM(pGm);

    CGameUnit* heroUnit = getHeroUnit();
    if (!heroUnit || heroUnit->isDead())
    {
        return;
    }

    if (!isClickAction())
    {
        return;
    }

    CCPoint oPos = ccpSub(pTouch->getLocation(), getPosition());

    if (touchActionIndex() == kUnitCastTarget)
    {
        CGameUnit* t = NULL;
        CActiveSkill* pSkill = heroUnit->getToCastSkill();
        switch (pSkill->getCastTargetType())
        {
        case CActiveSkill::kPointTarget:
            pSkill->setTargetPoint(oPos);
            heroUnit->cast();
            endOrderUnitToCast();
            return;

        case CActiveSkill::kUnitTarget:
            t = getUnits()->getNearestUnitInRange(oPos, 50, CONDITION(&CUnitGroup::isLivingEnemyOf), dynamic_cast<CUnitForce*>(heroUnit));
            if (t)
            {
                pSkill->setTargetUnit(t->getKey());
                heroUnit->cast();
                endOrderUnitToCast();
            }
            return;

        default:
            CCAssert(false, "err skill cast target type");
            return;
        }

        return;
    }

    if (m_bCanBuild)
    {
        if (addTower(oPos))
        {
            m_oBuildBtn.coolDown();
        }
    }
    else
    {
        CGameUnit* pUnit = getUnits()->getNearestUnitInRange(oPos, 50, CONDITION(&CUnitGroup::isLivingEnemyOf), dynamic_cast<CUnitForce*>(heroUnit));
        if (pUnit)
        {
            heroUnit->attack(pUnit->getKey());
            return;
        }
        heroUnit->moveTo(oPos);
    }

}

void CCWHomeSceneLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    if (m_iTouchActionFlag==0)
    {
        CCWinUnitLayer::ccTouchMoved(pTouch,pEvent);
    }
    else if (m_iTouchActionFlag==1)
    {
        CCLOG("fast Cast Skill");
    }
}

void CCWHomeSceneLayer::onEnter()
{
    CCWinUnitLayer::onEnter();
}

void CCWHomeSceneLayer::onExit()
{
    CCWinUnitLayer::onExit();
}

void CCWHomeSceneLayer::onBtnGoClick( CCObject* pObject )
{
    CCTechTreeScene* pScene=CCTechTreeScene::create();
    CCDirector::sharedDirector()->pushScene(pScene);
}

void CCWHomeSceneLayer::onBtnHeroClick( CCObject* pObject )
{
    m_oSkillPanel.setVisible(!m_oSkillPanel.isVisible());
}

CGameUnit* CCWHomeSceneLayer::getHeroUnit()
{
    CGameUnit* pHero = getUnitByKey(m_iHero);
    if (pHero || m_iHero)
    {
        return pHero;
    }
    pHero = m_oUipm.unitByIndex(3);
    m_iHero = pHero->getKey();
    addUnit(pHero);
    return pHero;
}

void CCWHomeSceneLayer::onBtnBuildClick( CCNode* pObject )
{
    CCSkillButtonNormal* pItem = dynamic_cast<CCSkillButtonNormal*>(pObject);
    M_SKILL_BUTTON_CANCEL(pItem);
    m_bCanBuild = !m_bCanBuild;
    M_DEF_FC(pFc);
    //pItem->setNormalSpriteFrame(m_bCanBuild ? pFc->spriteFrameByName(M_SKILL_ON_PATH("build")): pFc->spriteFrameByName(M_SKILL_PATH("build")));
    pItem->setNormalSpriteFrame(m_bCanBuild ? pFc->spriteFrameByName(M_SKILL_PATH("cancel")): pFc->spriteFrameByName(M_SKILL_PATH("build")));
}

void CCWHomeSceneLayer::onTickEvent( float fDt )
{
    CCWinUnitLayer::onTickEvent(fDt);
    CGameUnit* heroUnit = getHeroUnit();
    if (!heroUnit)
    {
        return;
    }
    static CGameUnit* pMemHero = NULL;

    static uint32_t dwOldExp = 0;
    if (pMemHero != heroUnit || heroUnit->getExp() != dwOldExp)
    {
        if (heroUnit!=NULL&&heroUnit->getMaxExp()!=0)
        {
            dwOldExp = heroUnit->getExp();
            float fPer = heroUnit->getExp() * 100 / heroUnit->getMaxExp();
            m_oExpBar.setPercentage(fPer, 0);
            m_oExpBar.setFillColor(ccc3(169, 147, 0));
        }
        
    }

    static uint32_t dwOldHp = 0;
    if (pMemHero != heroUnit || heroUnit->getHp() != dwOldHp)
    {
        dwOldHp = heroUnit->getHp();
        float fPer = heroUnit->getHp() * 100 / heroUnit->getMaxHp();
        m_oHpBar.setPercentage(fPer, 0);
        m_oHpBar.setFillColor(ccc3(MIN(255, (100.0 - fPer) * 2.56 / 0.5), MIN(255, 2.56 / 0.5  * fPer), 0));
    }

    if (pMemHero != heroUnit)
    {
        pMemHero = heroUnit;
    }

}

bool CCWHomeSceneLayer::addTower( const CCPoint& roPos )
{
    M_DEF_TB(pTb);
    pTb->buildTower(rand() % 2, roPos, this, this, callfuncO_selector(CCWHomeSceneLayer::addTowerEnd));
    onBtnBuildClick(&m_oBuildBtn);
    return true;
}

void CCWHomeSceneLayer::addTowerEnd( CCObject* pObject )
{
    CGameUnit* pTower = dynamic_cast<CGameUnit*>(pObject);
    if (!pTower)
    {
        return;
    }
    M_DEF_UM(pUm);
    M_DEF_OS(pOs);
    pTower->setForceByIndex(2);
    pTower->setAlly(1<<2);
    pTower->addSkill(CStatusShowPas::create());

    int r = (rand() % 100) / 10;
    switch (r)
    {
    case 0:
        pTower->addSkill(pOs->skill(COrgSkillInfo::kHpChangeAura1));
        break;
    case 1:
    case 2:
        pTower->addSkill(pOs->skill(COrgSkillInfo::kDoubleAttack1));
        break;
    case 3:
    case 4:
    case 5:
        pTower->addSkill(pOs->skill(COrgSkillInfo::kSplash1));
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        break;
    }

    if (r < 2)
    {
        pTower->addSkill(pOs->skill(COrgSkillInfo::kSlowDown1));
    }
    else if (r < 5)
    {
        pTower->addSkill(pOs->skill(COrgSkillInfo::kCritical1));
    }
    else if (r < 7)
    {
        pTower->addSkill(pOs->skill(COrgSkillInfo::kCritical2));
    }
    else if (r < 8)
    {
        pTower->setBaseAttackValue(CAttackValue(1, CAttackValue::kHoly, MAX(pTower->getBaseAttackValue(CAttackValue::kPhysical), pTower->getBaseAttackValue(CAttackValue::kMagical))));
    }
    else if (r < 9)
    {
        pTower->setArmorType(CArmorValue::kHoly);
    }

    pTower->setMaxHp(50);
    pTower->setAttackRange(200);
    pTower->setForceResource(getHeroUnit()->getForceResource());
}

void CCWHomeSceneLayer::onGoldChange( CCObject* pObject )
{
    CForceResouce* pFr = dynamic_cast<CForceResouce*>(pObject);
    if (!pFr)
    {
        return;
    }
    char sz[64];
    sprintf(sz, "%d", pFr->getGold());
    m_oGold.setString(sz);
}

void CCWHomeSceneLayer::onGetBuff( CCNode* pNode )
{
    if (m_oSkillPanel.getCount() + m_iBuffGetting == m_oSkillPanel.getMaxCount())
    {
        return;
    }
    ++m_iBuffGetting;
    CCCommmButton* pBtn = dynamic_cast<CCCommmButton*>(pNode);
    CGameUnit* pHero = getHeroUnit();
    pBtn->setEnabled(false);
    pBtn->stopAllActions();
    pBtn->setOpacity(0xFF);
    pBtn->runAction(CCSequence::create(CCScaleTo::create(0.25, 1.5, 1.5), CCScaleTo::create(0.5, 0, -0), NULL));
    pBtn->runAction(CCSequence::create(CCDelayTime::create(0.25), CCMoveToNode::create(0.5, pHero->getSprite(), false, 0, 1, pHero->getHalfOfHeight()), CCCallFuncN::create(this, callfuncN_selector(CCWHomeSceneLayer::onGetBuffEnd)), NULL));
}

void CCWHomeSceneLayer::onGetBuffEnd( CCNode* pNode )
{
    --m_iBuffGetting;
    CCCommmButton* pBtn = dynamic_cast<CCCommmButton*>(pNode);
    CGameUnit* pHero = getHeroUnit();
    CBuffSkill* pBuff = dynamic_cast<CBuffSkill*>(COrgSkillInfo::sharedOrgSkillInfo()->skill(pBtn->m_iKey));
    pBuff->setSrcKey(pHero->getKey());
    pHero->addBuff(pBuff);
    pNode->removeFromParentAndCleanup(true);
}

void CCWHomeSceneLayer::onHeroSoulTick( float fDt )
{
    m_stReviveInfo.fReviveTimeLeft -= fDt;
    int iLeft = ::ceil(m_stReviveInfo.fReviveTimeLeft);
    if (iLeft > 0)
    {
        char sz[32];
        sprintf(sz, "Revive time %ds", iLeft);
        m_oLeftToRevive.setString(sz);
    }
    else
    {
        M_DEF_OS(pOs);
        m_oLeftToRevive.setVisible(false);
        unschedule(schedule_selector(CCWHomeSceneLayer::onHeroSoulTick));

        CGameUnit* pHero = m_oUipm.unitByIndex(3);
        m_iHero = pHero->getKey();
        addUnit(pHero);
        pHero->setPosition(m_stReviveInfo.oPos);

        pHero->setForceByIndex(2);
        pHero->setAlly(1<<2);
        pHero->addSkill(CStatusShowPas::create());
        pHero->addSkill(pOs->skill(COrgSkillInfo::kHero1));

        pHero->setLevelUpdate(&g_oDemoUpdate);
        pHero->setMaxLevel(m_stReviveInfo.dwMaxLevel);
        pHero->setLevel(m_stReviveInfo.dwLevel);
        pHero->setForceResource(&m_oFr);

        pHero->addExp(m_stReviveInfo.dwExp);
    }
}

void CCWHomeSceneLayer::onUnitDie( CGameUnit* pUnit )
{
    CGameUnit* pHero;
    if (pUnit->getKey() == m_iHero)
    {
        // hero die
        m_stReviveInfo.fReviveTimeLeft = 5;
        m_stReviveInfo.oPos = pUnit->getPosition();
        m_stReviveInfo.dwExp = pUnit->getExp();
        m_stReviveInfo.dwLevel = pUnit->getLevel();
        m_stReviveInfo.dwMaxLevel = pUnit->getMaxLevel();
        m_oLeftToRevive.setVisible(true);
        schedule(schedule_selector(CCWHomeSceneLayer::onHeroSoulTick), 0.2);
        onHeroSoulTick(0);
    }
    else if (pUnit->getRewardExp() && M_RAND_HIT(10) && pUnit->isEnemyOf(dynamic_cast<CUnitForce*>(pHero = getHeroUnit())))
    {
        // Spawn skills
        CCCommmButton* pBtn = CCCommmButton::create(M_SKILL_PATH("skill1"), M_SKILL_PATH("skill1"), NULL, NULL, NULL, 0, this, callfuncN_selector(CCWHomeSceneLayer::onGetBuff), NULL, COrgSkillInfo::kThunderBoltBuff1);
        m_oMenu.addChild(pBtn);
        pBtn->setScale(0);
        pBtn->setPosition(getPosition());
        pBtn->runAction(CCScaleTo::create(0.5, 0.5, 0.5));
        pBtn->runAction(CCJumpBy::create(0.5, ccp(0, 0), 100, 1));
        pBtn->runAction(CCSequence::create(CCScaleTo::create(0.25, 1.0, 1.0), CCScaleTo::create(0.25, 0.75, 0.75), NULL));
        pBtn->runAction(CCDelayRelease::create(5.0));
        pBtn->runAction(CCRepeatForever::create(dynamic_cast<CCActionInterval*>(CCSequence::create(CCOrbitCamera::create(1, 10000, 0, 0, 360, 0, 0), CCDelayTime::create(1.5), NULL))));
    }
    // TODO: Reward
    CGameUnit* u;
    CCObject* pObj;
    M_DEF_GM(pGm);
    CCArray* pArr = getUnits()->getUnitsArray();
    vector<CGameUnit*> vec;

    CCARRAY_FOREACH(pArr, pObj)
    {
        u = dynamic_cast<CGameUnit*>(pObj);
        if (!u->isDead() && u->getMaxLevel() && pUnit->isEnemyOf(u) && pUnit->getDistance(u) < CGameUnit::CONST_MAX_REWARD_RANGE)
        {
            vec.push_back(u);
        }
    }
    int n = vec.size();
    if (n)
    {
        int iG = pUnit->getRewardGold() / n;
        int iE = pUnit->getRewardExp() / n;
        CForceResouce* pRes;
        for (int i = 0; i < n; ++i)
        {
            vec[i]->addExp(iE);
            pRes = vec[i]->getForceResource();
            if (pRes)
            {
                pRes->changeGold(iG);
            }
        }
    }
}
