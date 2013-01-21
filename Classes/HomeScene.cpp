#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "Action.h"
#include "MainScene.h"
#include "HomeScene.h"
#include "MissionInfo.h"
#include "UnitInfo.h"
#include "SkillInfo.h"
#include "PlayerInfo.h"
#include "GameCtrl.h"


bool CCHomeScene::init()
{
    CCScene::init();
    m_pHomeSceneLayer = CCHomeSceneLayer::create();
    addChild(m_pHomeSceneLayer);

    return true;
}

CCHomeSceneLayer::CCHomeSceneLayer()
{

}

static int g_iSlowDownKey = 0;

static CUnitPath g_oPath;

bool CCHomeSceneLayer::init()
{
    CCWinUnitLayer::initWithColor(ccc4(204, 232, 207, 64));
    //setUnitTickInterval(0.1);
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();

    M_DEF_FC(pFc);
    M_DEF_UM(pUm);
    M_DEF_SM(pSm); // 定义技能管理器
    M_DEF_GM(pGm); // 定义游戏管理器
    M_DEF_OS(pOs);
    pFc->addSpriteFramesWithFile("background.plist");
    pFc->addSpriteFramesWithFile("tank.plist");
    pFc->addSpriteFramesWithFile("UI.plist");
    //CCSprite* pSprite = CCSprite::createWithSpriteFrameName("kr/LevelDemo.png");
    CCSprite* pSprite = CCSprite::create("LevelDemo2HD.png");
    pSprite->setScale(0.5);
    setBackGroundSprite(pSprite);
    setBufferEffectParam(0.9, 50, 0.1);

    m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);

    m_oStart.initWithNormalImage("UI/start01.png", "UI/start01DOWN.png", NULL, this, menu_selector(CCHomeSceneLayer::onBtnStartClick));
    m_oMenu.addChild(&m_oStart);
    m_oStart.setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));
    
    m_oCfg.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png", NULL, this, menu_selector(CCHomeSceneLayer::onBtnCfgClick));
    m_oMenu.addChild(&m_oCfg);
    m_oCfg.setPosition(ccp(oSz.width * 0.9, oSz.height * 0.6));

    // demo code

    CGameUnit* u;
    
    u = pUm->unitByInfo(COrgUnitInfo::kMalik);
    addUnit(u); // 加到UnitLayer中

    u->setForceByIndex(1); // 所在势力1
    u->setAlly((1 << 1) | (1 << 2)); // 势力1跟势力2结盟
    u->setPosition(ccpAdd(getAnchorPointInPoints(), ccp(rand() % 50 - 25, rand() % 50 - 25))); // 设置位置
    u->setHostilityRange(400);

    CSkill* pSkill = CSpeedBuff::create(1, true, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
    g_iSlowDownKey = pSkill->getKey();
    pSm->addSkill(pSkill); // 将模板技能添加到技能管理器中

    u->addSkill(CStatusShowPas::create()); // 血条被动
    u->addSkill(pOs->skill(COrgSkillInfo::kHpChange1)); // 每0.1秒恢复0.2%HP
    u->addSkill(CVampirePas::create(0.3)); // 攻击30%吸血
    u->addSkill(CThumpPas::create(20, CExtraCoeff(2, 0), 0)); // 20%造成2倍致命一击
    u->addSkill(CThumpPas::create(20, CExtraCoeff(5, 0), 0)); // 20%造成5倍致命一击
    u->addSkill(pOs->skill(COrgSkillInfo::kSlowDown1)); // 100%降低10%目标攻击/移动速度，可叠加
    u->addSkill(CDoubleAttackPas::create(20)); // 50%发动连续攻击
    u->addSkill(pOs->skill(COrgSkillInfo::kImmo1));
    u->setLevelUpdate(&g_oDemoUpdate);
    u->setMaxLevel(60);
    u->addExp(0);

    // 远程攻击单位
    u = pUm->unitByInfo(COrgUnitInfo::kMagnus);
    addUnit(u);
    u->setForceByIndex(2);
    u->setAlly((1 << 1) | (1 << 2));
    u->setPosition(ccpAdd(getAnchorPointInPoints(), ccp(rand() % 50 - 25, rand() % 50 - 25))); // 设置位置
    u->setAttackMinRange(50);
    u->setHostilityRange(400);

    u->addSkill(CStatusShowPas::create()); // 血条被动
    u->addSkill(CHpChangePas::create(0.1, 0.2, true, -1)); // 每0.1秒恢复0.2%HP
    u->addSkill(CVampirePas::create(0.2)); // 攻击20%吸血
    u->addSkill(CThumpPas::create(20, CExtraCoeff(20, 0), 0)); // 20%造成20倍致命一击
    u->addSkill(CAttackBuffMakerPas::create(100, pSkill->getKey(), 1, CExtraCoeff(1, 0))); // 100%降低10%目标攻击/移动速度，可叠加
    u->addSkill(CDoubleAttackPas::create(50)); // 50%发动连续攻击
    u->setLevelUpdate(&g_oDemoUpdate);
    u->setMaxLevel(60);
    u->addExp(0);

    // 塔
    
    u = pUm->unitByInfo(COrgUnitInfo::kTesla);
    addUnit(u); // 加到UnitLayer中
    u->setForceByIndex(1); // 所在势力1
    u->setAlly((1 << 1) | (1 << 2)); // 势力1跟势力2结盟
    u->setPosition(ccpAdd(getAnchorPointInPoints(), ccp(50, 0))); // 设置位置

    u->addSkill(CStatusShowPas::create()); // 血条被动
    u->addSkill(pOs->skill(COrgSkillInfo::kHpChange1)); // 每0.1秒恢复0.2%HP
    u->addSkill(pOs->skill(COrgSkillInfo::kHpChangeAura1));

    u = pUm->unitByInfo(COrgUnitInfo::kArcane);
    addUnit(u); // 加到UnitLayer中
    u->setForceByIndex(2); // 所在势力1
    u->setAlly((1 << 1) | (1 << 2)); // 势力1跟势力2结盟
    u->setPosition(ccpAdd(getAnchorPointInPoints(), ccp(-50, 0))); // 设置位置
    
    u->addSkill(CStatusShowPas::create()); // 血条被动
    u->addSkill(pOs->skill(COrgSkillInfo::kHpChange1)); // 每0.1秒恢复0.2%HP
    u->addSkill(pOs->skill(COrgSkillInfo::kSpeedAura1));
    

    //CCLOG("%d", sizeof(CGameUnit));
    g_oPath.init();
    g_oPath.addPoint(ccp(200, 200));
    g_oPath.addPoint(ccp(200, 400));
    g_oPath.addPoint(ccp(100, 400));
    g_oPath.addPoint(getAnchorPointInPoints());


    return true;
}

void CCHomeSceneLayer::onBtnStartClick(CCObject* pObject)
{
    M_DEF_GM(pGm);
    pGm->pushScene(CCMainScene::create());
}

void CCHomeSceneLayer::onBtnCfgClick(CCObject* pObject)
{
    M_DEF_GM(pGm);
    M_DEF_UM(pUm);
    schedule(schedule_selector(CCHomeSceneLayer::onTick), 0.8);
    
    CPathGameUnit* u = NULL;
    int iIndex = rand() % 3;
    int iForce = rand() % 2 + 3;
    CAttackValue oAv;
    switch (iIndex)
    {
    case 0:
        u = pUm->pathUnitByInfo(COrgUnitInfo::kJt);

        break;

    case 1:
        u = pUm->pathUnitByInfo(COrgUnitInfo::kPaladin);

        u->addSkill(CThumpPas::create(20, CExtraCoeff(2, 0), 0));
        break;

    case 2:
        u = pUm->pathUnitByInfo(COrgUnitInfo::kVeznan);

        u->addSkill(CAttackBuffMakerPas::create(100, g_iSlowDownKey, 1, CExtraCoeff(1, 0))); // 100%降低10%目标攻击/移动速度，可叠加
        break;
    }

    addUnit(u);

    u->setForceByIndex(iForce);
    u->setAlly(1 << iForce);
    u->setPosition(ccp(50 + rand() % 700, 50 + rand() % 380));

    u->addSkill(CStatusShowPas::create());

    u->moveAlongPath(&g_oPath, true, true, 0);

}

void CCHomeSceneLayer::onTick( float fDt )
{
    M_DEF_GM(pGm);
    static float fS = 0;
    fS += fDt;

    if (!pGm->getUnits()->getUnitsCount())
    {
        return;
    }
    CGameUnit* pU = dynamic_cast<CGameUnit*>(pGm->getUnits()->getUnitsArray()->lastObject());

    if (fS > 5 && fS < 6)
    {
        pU->setExAttackSpeed(CExtraCoeff((rand() % 500) * 0.01, 0));
        pU->setMaxHp(rand() % 2000 + 1000);
        fS = 0;
    }
}
