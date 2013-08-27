#include "CommInc.h"

#include "GameDisplay.h"
#include "Skill.h"
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
#include "Achieve.h"

bool CCWHomeScene::init()
{
    CCScene::init();

    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    CCSprite* pSprite = NULL;
    
    m_pLl = CCLoadingLayer::create(this, 4, CCSprite::create("background/Dota.png"), oSz);
    addChild(m_pLl);
    m_pLl->setPosition(ccp(oSz.width * 0.5, oSz.height * 0.5));

    M_DEF_FC(pFc);
    pFc->addSpriteFramesWithFile("UI.plist");
    
    m_pPb = CCProgressBar::create(CCSizeMake(oSz.width * 0.7, oSz.height * 0.1), CCSprite::createWithSpriteFrameName("barfill.png"), CCSprite::createWithSpriteFrameName("Loading-BarBorder.png"), 1, 1, false);
    m_pLl->addChild(m_pPb);
    m_pPb->setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));
    
    m_pLb = CCLabelTTF::create("", "Hobo Std", 24);
    m_pLl->addChild(m_pLb);
    m_pLb->setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));
    m_pLb->setColor(ccc3(240, 240, 80));

    m_pLl->startLoading();
    
    return true;
}

void CCWHomeScene::onLoading( int iStage )
{
    M_DEF_FC(pFc);
    switch (iStage)
    {
    case 0:
        pFc->addSpriteFramesWithFile("background.plist");
        break;
    case 1:
        pFc->addSpriteFramesWithFile("tank.plist");
        break;
    case 2:
        pFc->addSpriteFramesWithFile("skill.plist");
        break;
    case 3:
        M_DEF_GM(pGm);
        M_DEF_OS(pOs);
        break;
    }

    //CCLOG("setPercent: %.1f", m_pLl->getPercentage());
    m_pLl->setProgressNodeAndAction(m_pPb->m_pPt, m_pPb->setPercentageAction(m_pLl->getPercentage(), 0.1));
    char szBuf[32];
    sprintf(szBuf, "Loading .. %d%%", (int)m_pLl->getPercentage());
    m_pLb->setString(szBuf);
}

void CCWHomeScene::onLoadingEnd()
{
    m_pLl->removeFromParentAndCleanup(false);
    //m_pPb = NULL;
    //m_pLl = NULL;
    //Sleep(1000);
    //return;

    m_pHomeSceneLayer = CCWHomeSceneLayer::create();
    addChild(m_pHomeSceneLayer, 1);
    //     CGameCtrlLayer* testCtrlLayer=CGameCtrlLayer::create();
    //     addChild(testCtrlLayer,2);
    addChild(&m_pHomeSceneLayer->m_oGameCtrlLayer, 2);
}

CCWHomeSceneLayer::CCWHomeSceneLayer()
{
}

bool CCWHomeSceneLayer::init()
{
    CCWinUnitLayer::initWithColor(ccc4(204, 232, 207, 64));
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    CCSprite* pSprite = NULL;
    
    M_DEF_GM(pGm);
    M_DEF_OS(pOs);
    M_DEF_FC(pFc);
    
    //setBackGroundSprite(CCSprite::create("levels/level01/LevelHD.png"));
    setBackGroundSprite(CCSprite::create("LevelDemo2HD.png"));
    setBufferEffectParam(0.9, 10, 0.1);
    m_oMenu.init();
    addChild(&m_oMenu, 0, 5131115);
    m_oMenu.setPosition(CCPointZero);

    m_iTouchActionFlag = 0;
    m_iHero = 0;

    m_oFr.initWithChangeCallback(this, callfuncO_selector(CCWHomeSceneLayer::onGoldChange)); // 势力资源

    M_DEF_UPM(pUpm);
    //pUpm->addPatches("heroes.uip");
    m_oUipm.initWithFile("heroes.uip");
    // demo code
    M_DEF_UM(pUm);
    CGameUnit* midTower = pUm->unitByInfo(COrgUnitInfo::kArcane);
    addUnit(midTower);
    midTower->setPosition(ccp(1804,793));
    midTower->setForceByIndex(2);
    midTower->setAlly(1<<2);
    midTower->addSkill(CStatusShowPas::create());
    //midTower->addSkill(pOs->skill(COrgSkillInfo::kHpChange2));
    midTower->addSkill(pOs->skill(COrgSkillInfo::kSlowDown1));
    midTower->addSkill(pOs->skill(COrgSkillInfo::kSpeedAura1));
    midTower->setForceResource(&m_oFr);

    midTower = pUm->unitByInfo(COrgUnitInfo::kTesla);
    addUnit(midTower);
    midTower->setPosition(ccp(1704, 793));
    midTower->setForceByIndex(2);
    midTower->setAlly(1<<2);
    midTower->addSkill(CStatusShowPas::create());
    //midTower->addSkill(pOs->skill(COrgSkillInfo::kHpChange2));
    midTower->addSkill(pOs->skill(COrgSkillInfo::kImmo1));
    midTower->setForceResource(&m_oFr);


    CGameUnit* pHeroUnit = getHeroUnit();

    //addUnit(pHeroUnit);
    pHeroUnit->setPosition(ccp(804, 793));
    pHeroUnit->setForceByIndex(2);
    pHeroUnit->setAlly(1<<2);
    pHeroUnit->addSkill(CStatusShowPas::create());
    //pHeroUnit->addSkill(pOs->skill(COrgSkillInfo::kHero1));

    pHeroUnit->setLevelUpdate(&g_oDemoUpdate);
    pHeroUnit->setMaxLevel(100);
    pHeroUnit->setForceResource(&m_oFr);

    m_oGameCtrlLayer.initWithColor(ccc4(0, 0, 0, 0));
    m_oMenuCtrl.init();
    m_oGameCtrlLayer.addChild(&m_oMenuCtrl);
    m_oMenuCtrl.setPosition(CCPointZero);
    m_oGoToTechTree.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png"
        , NULL, this, menu_selector(CCWHomeSceneLayer::onBtnGoClick));
    //m_oGoToTechTree.setPosition(ccp(oSz.width - m_oGoToTechTree.getContentSize().width * 0.5 - 10, m_oGoToTechTree.getContentSize().height * 0.5 + 10));
    //m_oMenuCtrl.addChild(&m_oGoToTechTree);

    m_oCfg.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png", NULL, this, menu_selector(CCWHomeSceneLayer::onBtnCfgClick));
    //m_oMenuCtrl.addChild(&m_oCfg);
    //m_oCfg.setPosition(ccp(oSz.width * 0.9, oSz.height * 0.6));

    m_oStart.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png", NULL, this, menu_selector(CCWHomeSceneLayer::onBtnStartClick));
    m_oMenuCtrl.addChild(&m_oStart);
    m_oStart.setPosition(ccp(oSz.width - m_oStart.getContentSize().width * 0.5 - 10, m_oStart.getContentSize().height * 0.5 + 10));

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

    CActiveSkill* pSkill = NULL;
    CPassiveSkill* pSkillPas = NULL;

    // 初始化技能1
    pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kSwordStorm1));
    //pSkill->setCastAniIndex(CGameUnit::kAnimationAct3);
    pHeroUnit->addSkill(pSkill);
    CCSkillButtonBase* pBtn;
    pBtn = M_CREATE_SKILL("SwordStorm", pHeroUnit->getKey(), pSkill->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);

    // 初始化技能2
    pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kThunderClap1));
    pSkill->setCastAniIndex(CGameUnit::kAnimationAct5);
    pHeroUnit->addSkill(pSkill);
    pBtn = M_CREATE_SKILL("ThunderClap", pHeroUnit->getKey(), pSkill->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);

    // 初始化技能3
    pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kFastStrikeBack1));
    pSkill->setCastAniIndex(CGameUnit::kAnimationAct3);
    pHeroUnit->addSkill(pSkill);
    pBtn = M_CREATE_SKILL("FastStrikeBack", pHeroUnit->getKey(), pSkill->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);

    // 初始化技能4
    pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kShockWave1));
    pSkill->setCastAniIndex(CGameUnit::kAnimationAct2);
    dynamic_cast<CProjectileWaveAct*>(pSkill)->setProjectileBirthOffset(ccp(0, 25));

    //pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kJumpChop1));
    //pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kAddDamage1));

    //pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kRunBuff1));
    //pSkill->setCastAniInfo(CGameUnit::kAnimationAct2, 0.2);
    pHeroUnit->addSkill(pSkill);
    pBtn = M_CREATE_SKILL("ShockWave", pHeroUnit->getKey(), pSkill->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);

    /*
    // 初始化技能5
    pSkillPas = dynamic_cast<CPassiveSkill*>(pOs->skill(COrgSkillInfo::kThrowHit1));
    pHeroUnit->addSkill(pSkillPas);
    pBtn = M_CREATE_SKILL_PAS("ThrowHit", pHeroUnit->getKey(), pSkillPas->getKey(), this);
    m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);
    */

    pSkillPas = dynamic_cast<CPassiveSkill*>(pOs->skill(COrgSkillInfo::kCritical2));
    pHeroUnit->addSkill(pSkillPas);

    pSkillPas = dynamic_cast<CPassiveSkill*>(pOs->skill(COrgSkillInfo::kKnockBack1));
    pHeroUnit->addSkill(pSkillPas);

    pSkillPas = dynamic_cast<CPassiveSkill*>(pOs->skill(COrgSkillInfo::kDoubleAttack1));
    pHeroUnit->addSkill(pSkillPas);

    pSkillPas = dynamic_cast<CPassiveSkill*>(pOs->skill(COrgSkillInfo::kHero1));
    pHeroUnit->addSkill(pSkillPas);

    //m_oBuildBtn.init(M_SKILL_PATH("build"), M_SKILL_DOWN_PATH("build"), M_SKILL_DIS_PATH("build"), M_SKILL_PATH("white"), "mask/mask.png", 15, this, callfuncN_selector(CCWHomeSceneLayer::onBtnBuildClick), NULL);
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
    //m_oGold.setString(UTEXT(""));

    onBtnCfgClick(&m_oCfg);

    pGm->setVoice(false);
    m_iBuffGetting = 0;

    m_oLeftToRevive.initWithString("Revive time: 0s", "", 24);
    m_oGameCtrlLayer.addChild(&m_oLeftToRevive);
    m_oLeftToRevive.setPosition(ccp(oSz.width * 0.8, oSz.height * 0.9));
    m_oLeftToRevive.setVisible(false);

    pHeroUnit->setHp(pHeroUnit->getMaxHp());


    // 初始化目标信息面板
    m_oTargetInfoLayer.initWithColor(ccc4(140, 70, 35, 255));
    m_oGameCtrlLayer.addChild(&m_oTargetInfoLayer);
    m_oTargetInfoLayer.setContentSize(CCSizeMake(700, 48));
    m_oTargetInfoLayer.setPosition(ccp(5, 5));

    pSprite = CCSprite::createWithSpriteFrameName("pos.png");
    m_oGameCtrlLayer.addChild(pSprite);
    pSprite->setPosition(ccp(5, 5));

    // 等级
    float fW0 = 0;
    float fW1 = 32;
    float fBaseX = M_FIX_BASE_X(fW0, fW1, 10);
    pSprite = CCSprite::createWithSpriteFrameName("ph32.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, 24));
    pSprite->setColor(ccc3(240, 160, 120));

    fW0 = fW1;
    fW1 = 64;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_oTargetLv.initWithString("12", "Hobo Std", 24, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_oTargetInfoLayer.addChild(&m_oTargetLv);
    m_oTargetLv.setPosition(ccp(fBaseX, 24));
    m_oTargetLv.setColor(ccc3(250, 190, 100));

    // hp
    fW0 = fW1;
    fW1 = 32;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 4);
    pSprite = CCSprite::createWithSpriteFrameName("ph32.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, 24));
    pSprite->setColor(ccc3(100, 185, 75));

    fW0 = fW1;
    fW1 = 160;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_oTargetHp.initWithString("1320/3208", "Hobo Std", 24, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_oTargetInfoLayer.addChild(&m_oTargetHp);
    m_oTargetHp.setPosition(ccp(fBaseX, 24));
    m_oTargetHp.setColor(ccc3(250, 190, 100));

    // 攻击
    fW0 = fW1;
    fW1 = 32;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 4);
    pSprite = CCSprite::createWithSpriteFrameName("ph32.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, 24));
    pSprite->setColor(ccc3(230, 40, 30));

    /*
    pSprite = CCSprite::createWithSpriteFrameName("pos.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, 24));
    */

    fW0 = fW1;
    fW1 = 0; // 锚点在最左处
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_oTargetAtk.initWithString("", "Hobo Std", 24);
    m_oTargetAtk.setAnchorPoint(ccp(0.0, 0.5));
    m_oTargetInfoLayer.addChild(&m_oTargetAtk);
    m_oTargetAtk.setHorizontalAlignment(kCCTextAlignmentLeft);
    m_oTargetAtk.setString("105 - 110");
    //m_oTargetAtk.setDimensions(CCSizeMake(m_oTargetAtk.getTextureRect().size.width, 32));
    m_oTargetAtk.setPosition(ccp(fBaseX, 24));
    m_oTargetAtk.setColor(ccc3(250, 190, 100));
    m_oTargetAtk.setString("150 - 110");

    /*
    pSprite = CCSprite::createWithSpriteFrameName("pos.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setTag(100);
    pSprite->setPosition(ccp(fBaseX, 24));
    */

    fBaseX += m_oTargetAtk.getTextureRect().size.width;
    m_oTargetAtkEx.initWithString("", "Hobo Std", 24);
    m_oTargetAtkEx.setAnchorPoint(ccp(0.0, 0.5));
    m_oTargetInfoLayer.addChild(&m_oTargetAtkEx);
    m_oTargetAtkEx.setHorizontalAlignment(kCCTextAlignmentLeft);
    m_oTargetAtkEx.setString(" +15000");
    //m_oTargetAtkEx.setDimensions(CCSizeMake(m_oTargetAtkEx.getTextureRect().size.width, 32));
    m_oTargetAtkEx.setPosition(ccp(fBaseX, 24));
    m_oTargetAtk.setString("151 - 167");
    m_oTargetAtkEx.setColor(ccc3(40, 220, 40));

    /*
    pSprite = CCSprite::createWithSpriteFrameName("pos.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setTag(101);
    pSprite->setPosition(ccp(fBaseX, 24));
    */
    // 护甲
    fW1 = 32;
    fBaseX += 240 - m_oTargetAtk.getTextureRect().size.width + fW1 * 0.5;
    pSprite = CCSprite::createWithSpriteFrameName("ph32.png");
    m_oTargetInfoLayer.addChild(pSprite);
    pSprite->setPosition(ccp(fBaseX, 24));
    pSprite->setColor(ccc3(0, 110, 155));

    fW0 = fW1;
    fW1 = 64;
    fBaseX += M_FIX_BASE_X(fW0, fW1, 10);
    m_oTargetDef.initWithString("32", "Hobo Std", 24, CCSizeMake(fW1, 32), kCCTextAlignmentLeft);
    m_oTargetInfoLayer.addChild(&m_oTargetDef);
    m_oTargetDef.setPosition(ccp(fBaseX, 24));
    m_oTargetDef.setColor(ccc3(250, 190, 100));

    m_pLastTarget = NULL;

    updateTargetInfo(pHeroUnit);


#if 0
    //成就管理器初始化
    CAchieveManager::sharedAchieveManager();
    CAchieve* pAchieve =  CAchieveManager::sharedAchieveManager()->push(1);
    if (pAchieve)
    {
        ostringstream out;
        pAchieve->display(out);
        CCLog("reach global : %s\n", out.str().c_str());
    }
#endif

    m_fWaitingNextRound = 0.0;

    m_iLastNum = -1;

    //pHeroUnit->setMaxHp(5);

    //CCDirector::sharedDirector()->setAnimationInterval(0.1);
    m_bMissionEnd = false;

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
    updateTargetInfo(m_pLastTarget);

    enum
    {
        kMalik,
        kMagnus,
        kJt
    };
    M_DEF_GM(pGm);

    M_DEF_UM(pUm);

    if (m_bMissionEnd)
    {
        return;
    }

    if (m_fWaitingNextRound >= FLT_EPSILON)
    {
        m_fWaitingNextRound -= fDt;

        if (m_fWaitingNextRound < 8.0 && m_iLastNum == -1)
        {
            m_iLastNum = 3;
            schedule(schedule_selector(CCWHomeSceneLayer::onNextRoundTick), 2.0);
            CCLOG("!!!!!!!!!!!!!!!!!!!!!");
        }

        if (m_fWaitingNextRound < FLT_EPSILON)
        {
            m_iLastNum = 0;
            // new round start
            if (m_pCurMission->nextRound())
            {
                CGameUnit* u = NULL;
                if((u = getHeroUnit()) != NULL)
                {
                    u->setHp(u->getHp() + (u->getMaxHp() - u->getHp()) * 0.5);
                }
            }
        }
        return;
    }

    //DemoMission
    int r = m_pCurMission->curRound();
    int n = m_pCurMission->rushCount();
    // static int rushFinishedNumber=0;
    bool bRoundEnd = true;
    for (int i = 0; i < n; ++i)
    {
        int iRes = m_pCurMission->rush(i, fDt);
        if (iRes >= 0)
        {
            // TODO: createUnitHere;
            bRoundEnd = false;
            CUnitPath* p = m_pCurMission->pathOfRush(i);
            const CCPoint* pPos = p->getCurTargetPoint(0);
            if (pPos)
            {
                CGameUnit* u = m_pCurMission->m_oUipm.unitByIndex(iRes);
                addUnit(u);
                u->setForceByIndex(3);
                u->setAlly(1 << 3);
                u->setPosition(*pPos);
                u->moveAlongPath(p, false);
                u->addSkill(CStatusShowPas::create());
                //u->setMaxHp(u->getMaxHp() + r * 10);
                //u->setExAttackValue(CAttackValue::kPhysical, CExtraCoeff(1 + r / 10.0, 0));
                //u->setExAttackValue(CAttackValue::kMagical, CExtraCoeff(1 + r / 10.0, 0));
                //u->setExAttackValue(CAttackValue::kHoly, CExtraCoeff(1 + r / 10.0, 0));
            }
        }
        else if (iRes == CGameMission::kNoRound)
        {
            // TODO: roundEnd
            m_bMissionEnd = true;
            break;
        }
        else if (iRes == CGameMission::kNoUnit)
        {
            // TODO: curRushEnd
            if (m_pCurMission->isLastRound())
            {
                m_bMissionEnd = true;
            }
            continue;
        }
        else if (iRes == CGameMission::kWaiting)
        {
            // TODO: waitNextUnit
            bRoundEnd = false;
            continue;
        }
        else
        {
            CCLOG("res(%d) err", iRes);
        }

    }

    if (bRoundEnd)
    {
        m_fWaitingNextRound = 10.0;
        m_iLastNum = -1;
        // TODO: curRoundEnd


    }
}

bool CCWHomeSceneLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCWinUnitLayer::ccTouchBegan(pTouch,pEvent);
    M_DEF_GM(pGm);
    CGameUnit* pHeroUnit = getHeroUnit();
    CCPoint touchPoint = ccpSub(pTouch->getLocation(), getPosition());
#if 0
    if (pHeroUnit!=NULL&&!pHeroUnit->isDead())//incomplete
    {
        CCPoint heroPoint=pHeroUnit->getPosition();
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

    CGameUnit* pHeroUnit = getHeroUnit();
    if (!pHeroUnit || pHeroUnit->isDead())
    {
        return;
    }

    if (!isClickAction())
    {
        return;
    }

    CCPoint oPos = ccpSub(pTouch->getLocation(), getPosition());

    CGameUnit* pUnit = getUnits()->getNearestUnitInRange(oPos, 50);
    updateTargetInfo(pUnit);

    if (touchActionIndex() == kUnitCastTarget)
    {
        CGameUnit* t = NULL;
        CActiveSkill* pSkill = pHeroUnit->getToCastSkill();
        switch (pSkill->getCastTargetType())
        {
        case CActiveSkill::kPointTarget:
            pSkill->setTargetPoint(oPos);
            pHeroUnit->cast();
            endOrderUnitToCast();
            //hideTargetInfo();
            return;

        case CActiveSkill::kUnitTarget:
            t = getUnits()->getNearestUnitInRange(oPos, 50, CONDITION(&CUnitGroup::isLivingEnemyOf), dynamic_cast<CUnitForce*>(pHeroUnit));
            if (t)
            {
                
                pSkill->setTargetUnit(t->getKey());
                pHeroUnit->cast();
                endOrderUnitToCast();
                //updateTargetInfo(t);
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
        CGameUnit* pUnit = getUnits()->getNearestUnitInRange(oPos, 50, CONDITION(&CUnitGroup::isLivingEnemyOf), dynamic_cast<CUnitForce*>(pHeroUnit));
        //updateTargetInfo(pUnit);
        if (pUnit)
        {
            pHeroUnit->attack(pUnit->getKey());
            return;
        }
        pHeroUnit->moveTo(oPos);
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
    CCTechTreeScene* pScene = CCTechTreeScene::create();
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
    CGameUnit* pHeroUnit = getHeroUnit();
    if (!pHeroUnit)
    {
        return;
    }

    //updateTargetInfo(pHeroUnit, 0);

    static CGameUnit* pMemHero = NULL;

    static uint32_t dwOldExp = 0;
    if (pMemHero != pHeroUnit || pHeroUnit->getExp() != dwOldExp)
    {
        if (pHeroUnit!=NULL&&pHeroUnit->getMaxExp()!=0)
        {
            dwOldExp = pHeroUnit->getExp();
            float fPer = pHeroUnit->getExp() * 100 / pHeroUnit->getMaxExp();
            m_oExpBar.setPercentage(fPer, 0);
            m_oExpBar.setFillColor(ccc3(169, 147, 0));
        }

    }

    static uint32_t dwOldHp = 0;
    if (pMemHero != pHeroUnit || pHeroUnit->getHp() != dwOldHp)
    {
        dwOldHp = pHeroUnit->getHp();
        float fPer = pHeroUnit->getHp() * 100 / pHeroUnit->getMaxHp();
        m_oHpBar.setPercentage(fPer, 0);
        m_oHpBar.setFillColor(ccc3(MIN(255, (100.0 - fPer) * 2.56 / 0.5), MIN(255, 2.56 / 0.5  * fPer), 0));
    }

    if (pMemHero != pHeroUnit)
    {
        pMemHero = pHeroUnit;
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
        // hero revive
        M_DEF_OS(pOs);
        m_oLeftToRevive.setVisible(false);
        unschedule(schedule_selector(CCWHomeSceneLayer::onHeroSoulTick));

        CGameUnit* pHero = m_oUipm.unitByIndex(3);
        m_iHero = pHero->getKey();
        addUnit(pHero);
        pHero->setPosition(m_stReviveInfo.oPos);

        pHero->setForceByIndex(2);
        pHero->setAlly(1<<2);
        //pHero->addSkill(CStatusShowPas::create());
        //pHero->addSkill(pOs->skill(COrgSkillInfo::kHero1));

        pHero->setLevelUpdate(&g_oDemoUpdate);
        pHero->setMaxLevel(m_stReviveInfo.dwMaxLevel);
        pHero->setLevel(m_stReviveInfo.dwLevel);
        pHero->setForceResource(&m_oFr);

        pHero->addExp(m_stReviveInfo.dwExp);

        CSkill* pSkill = NULL;
        CCObject* pObj = NULL;
        CCSkillButtonBase* pBtn = NULL;
        CCARRAY_FOREACH(&m_stReviveInfo.oArrSkill, pObj)
        {
            pSkill = dynamic_cast<CSkill*>(pObj);
            pHero->addSkill(pSkill);
            pBtn = dynamic_cast<CCSkillButtonBase*>(pSkill->getDisplayBody());
            if (pBtn)
            {
                dynamic_cast<CCSkillButtonAdvance*>(pBtn)->m_iUnitKey = m_iHero;
                if (pSkill->getCoolDown())
                {
                    CCLOG("CD [%f]", (pSkill->getCoolDown() - skillCoolDownLeft(pSkill->getKey())) / pSkill->getCoolDown() * 100.0);
                    pBtn->coolDown((pSkill->getCoolDown() - skillCoolDownLeft(pSkill->getKey())) / pSkill->getCoolDown() * 100.0);
                }

                m_oSkillPanel.pushAddButtonExAction(pBtn, CCButtonPanel::kTopToBottom);
            }
        }

        //CCSkillButtonBase* pBtn = NULL;
        m_stReviveInfo.oArrSkill.removeAllObjects();

        /*
        CCARRAY_FOREACH(&m_stReviveInfo.oArrSkillBtn, pObj)
        {
        pBtn = dynamic_cast<CCSkillButtonBase*>(pObj);
        m_oSkillPanel.pushAddButtonExAction(pBtn);
        }
        m_stReviveInfo.oArrSkillBtn.removeAllObjects();
        */
    }
}

void CCWHomeSceneLayer::onUnitDie( CGameUnit* pUnit )
{
    if (m_pLastTarget == pUnit)
    {
        hideTargetInfo();
    }

    CGameUnit* pHero;
    if (pUnit->getKey() == m_iHero)
    {
        // hero die
        m_stReviveInfo.fReviveTimeLeft = 5;
        m_stReviveInfo.oPos = pUnit->getPosition();
        m_stReviveInfo.dwExp = pUnit->getExp();
        m_stReviveInfo.dwLevel = pUnit->getLevel();
        m_stReviveInfo.dwMaxLevel = pUnit->getMaxLevel();
        m_stReviveInfo.oArrSkill.initWithArray(&pUnit->m_oArrSkill);
        CCObject* pObj = NULL;
        m_stReviveInfo.oArrSkillBtn.init();
        CCARRAY_FOREACH(&m_stReviveInfo.oArrSkill, pObj)
        {
            CSkill* pSkill = dynamic_cast<CSkill*>(pObj);
            CActiveSkill* pActSkill = dynamic_cast<CActiveSkill*>(pSkill);
            if (pActSkill)
            {
                //addSkillCoolDown(pActSkill);  // 如果是主动技能，挂到cd管理器
            }

            // 如果技能有UI，保持UI
            CCSkillButtonBase* pBtn = dynamic_cast<CCSkillButtonBase*>(pSkill->getDisplayBody());
            if (pBtn)
            {
                m_stReviveInfo.oArrSkillBtn.addObject(pBtn);
                if (pActSkill)
                {
                    //pActSkill->setCoolDownLeft((100.0 - pBtn->getPercentage()) / 100.0 * pActSkill->getCoolDown());
                    //CCLOG("[CD: %f] input", skillCoolDownLeft(pActSkill->getKey()));
                }
            }
        }
        //m_stReviveInfo.oArrSkillBtn.initWithArray(m_oSkillPanel.getSkillMenu()->getChildren());
        m_oLeftToRevive.setVisible(true);

        schedule(schedule_selector(CCWHomeSceneLayer::onHeroSoulTick), 0.2);
        onHeroSoulTick(0);
    }
    else if (pUnit->getRewardExp() && M_RAND_HIT(100) && pUnit->isEnemyOf(dynamic_cast<CUnitForce*>(pHero = getHeroUnit())))
    {
        // Spawn skills
        CCCommmButton* pBtn = CCCommmButton::create(M_SKILL_PATH("skill1"), M_SKILL_PATH("skill1"), NULL, NULL, NULL, 0, this, callfuncN_selector(CCWHomeSceneLayer::onGetBuff), NULL, COrgSkillInfo::kThunderBoltBuff1);
        m_oMenu.addChild(pBtn);
        pBtn->setScale(0);
        pBtn->setPosition(pUnit->getPosition());
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

void CCWHomeSceneLayer::onNextRoundTick( float fDt )
{
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    char szNum[16];
    sprintf(szNum, "%d", m_iLastNum);
    CCSprite* pNode = CCLabelTTF::create(szNum, "fonts/Comic Book.ttf", 128);
    m_oGameCtrlLayer.addChild(pNode);
    pNode->setPosition(ccp(oSz.width * 0.8, oSz.height * 0.5));
    pNode->setOpacity(0);
    CCActionInterval* pAct = CCReleaseAfter::create(CCFadeInOutScale4::create(0.5, 1.2, 0.8,     0.10, 0.1, 1.0, 0.2));
    pNode->runAction(pAct);

    --m_iLastNum;
    if (!m_iLastNum)
    {
        unschedule(schedule_selector(CCWHomeSceneLayer::onNextRoundTick));
        CCLOG("---------------");
    }
}

void CCWHomeSceneLayer::hideTargetInfo()
{
    m_oTargetInfoLayer.setVisible(false);
    m_pLastTarget = NULL;
}

void CCWHomeSceneLayer::updateTargetInfo( CGameUnit* pUnit )
{
    if (!pUnit || pUnit->isDead())
    {
        return;
    }

    if (m_pLastTarget != pUnit)
    {
        m_pLastTarget = pUnit;
    }

    if (!m_oTargetInfoLayer.isVisible())
    {
        m_oTargetInfoLayer.setVisible(true);
    }

    //if (pUnit != m_pLastTarget)

    char szBuf[64];

    uint32_t dwLevel = pUnit->getLevel();
    if (dwLevel != m_stTargetInfo.dwLevel)
    {
        sprintf(szBuf, "%u", pUnit->getLevel());
        m_oTargetLv.setString(szBuf);
        m_stTargetInfo.dwLevel = dwLevel;
    }

    uint32_t dwHp = pUnit->getHp();
    uint32_t dwMaxHp = pUnit->getMaxHp();
    if ((dwHp != m_stTargetInfo.dwHp) || (dwMaxHp != m_stTargetInfo.dwMaxHp))
    {
        sprintf(szBuf, "%u/%u", dwHp, dwMaxHp);
        m_oTargetHp.setString(szBuf);
        m_stTargetInfo.dwHp = dwHp;
        m_stTargetInfo.dwMaxHp = dwMaxHp;
    }

    uint32_t dwAtk0 = 0;
    uint32_t dwAtk1 = 0;
    float fAtk = 0;
    float fAtkRnd = 0;
    uint32_t dwAtkEx = 0;
    CAttackValue::ATTACK_TYPE eAtkType = (CAttackValue::ATTACK_TYPE)0;
    for (int i = 0; i < CAttackValue::CONST_MAX_ATTACK_TYPE; ++i)
    {
        fAtk = pUnit->getBaseAttackValue((CAttackValue::ATTACK_TYPE)i);
        fAtkRnd = pUnit->getExAttackRandomRange();
        if (fAtk)
        {
            dwAtk0 = fAtk * (1 - fAtkRnd);
            dwAtk1 = fAtk * (1 + fAtkRnd);
            eAtkType = (CAttackValue::ATTACK_TYPE)i;
            break;
        }
    }

    if ((uint32_t)fAtk)
    {
        dwAtkEx = fAtk * (pUnit->getExAttackValue(eAtkType).getMulriple() - 1.0) + pUnit->getExAttackValue(eAtkType).getAddend();
    }

    if ((dwAtk0 != m_stTargetInfo.dwAtk0) || (dwAtk1 != m_stTargetInfo.dwAtk1) || (dwAtkEx != m_stTargetInfo.dwAtkEx))
    {
        sprintf(szBuf, "%u - %u", dwAtk0, dwAtk1);
        m_oTargetAtk.setString(szBuf);
        //m_oTargetAtkEx.setPosition();
        m_stTargetInfo.dwAtk0 = dwAtk0;
        m_stTargetInfo.dwAtk1 = dwAtk1;

        //m_oTargetInfoLayer.getChildByTag(100)->setPosition(m_oTargetAtk.getPosition());

        if (dwAtkEx)
        {
            sprintf(szBuf, " +%u", dwAtkEx);
            m_oTargetAtkEx.setString(szBuf);
            //m_oTargetAtkEx.setDimensions(CCSizeMake(m_oTargetAtkEx.getTextureRect().size.width, 32));
            m_oTargetAtkEx.setPosition(ccpAdd(m_oTargetAtk.getPosition(), ccp(m_oTargetAtk.getTextureRect().size.width, 0)));
            //m_oTargetInfoLayer.getChildByTag(101)->setPosition(m_oTargetAtkEx.getPosition());
        }
        else
        {
            m_oTargetAtkEx.setString("");
        }
        m_stTargetInfo.dwAtkEx = dwAtkEx;
    }

    uint32_t dwDef = pUnit->getBaseArmorValue();
    if (dwDef != m_stTargetInfo.dwDef)
    {
        sprintf(szBuf, "%u", dwDef);
        m_oTargetDef.setString(szBuf);
        m_stTargetInfo.dwDef = dwDef;
    }

}

void CCWHomeSceneLayer::onUnitHpChange( CGameUnit* pUnit, float fChanged )
{
    if (m_pLastTarget == pUnit)
    {
        updateTargetInfo(pUnit);
    }
}

void CCWHomeSceneLayer::onLoading( int iStage )
{

}

void CCWHomeSceneLayer::onLoadingEnd()
{

}
