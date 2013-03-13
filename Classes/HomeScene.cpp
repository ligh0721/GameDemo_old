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
#include "WHomeScene.h"
#include "TechTreeScene.h"


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
    M_DEF_SM(pSm); // ���弼�ܹ�����
    M_DEF_GM(pGm); // ������Ϸ������
    M_DEF_OS(pOs);
    pFc->addSpriteFramesWithFile("background.plist");
    pFc->addSpriteFramesWithFile("UI.plist");
    //CCSprite* pSprite = CCSprite::createWithSpriteFrameName("kr/LevelDemo.png");
    CCSprite* pSprite = CCSprite::create("Home.png");
    //pSprite->setScale(0.5);
    setBackGroundSprite(pSprite);
    //setBufferEffectParam(0.9, 50, 0.1);
    setBufferEffectParam(0, 0, 0);

    /*
    m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);
    */
    CCButtonPanel* pPanel = CCButtonPanel::create(1, 3, 256, 256, 50, NULL);
    addChild(pPanel);
    pPanel->setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));
    
    CCSkillButtonNormal* pBtn = CCSkillButtonNormal::create("Start.png", "Start_ON.png", NULL, NULL, NULL, 0, this, callfuncN_selector(CCHomeSceneLayer::onBtnStartClick), NULL);
    pPanel->addButton(pBtn, 0, 0);

    pBtn = CCSkillButtonNormal::create("Upgrades.png", "Upgrades_ON.png", NULL, NULL, NULL, 0, this, callfuncN_selector(CCHomeSceneLayer::onBtnUpgradesClick), NULL);
    pPanel->addButton(pBtn, 1, 0);

    pBtn = CCSkillButtonNormal::create("Achievements.png", "Achievements_ON.png", NULL, NULL, NULL, 0, this, callfuncN_selector(CCHomeSceneLayer::onBtnAchievementsClick), NULL);
    pPanel->addButton(pBtn, 2, 0);

    /*
    m_oStart.initWithNormalImage("UI/start01.png", "UI/start01DOWN.png", NULL, this, menu_selector(CCHomeSceneLayer::onBtnStartClick));
    m_oMenu.addChild(&m_oStart);
    m_oStart.setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));
    
    m_oCfg.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png", NULL, this, menu_selector(CCHomeSceneLayer::onBtnCfgClick));
    m_oMenu.addChild(&m_oCfg);
    m_oCfg.setPosition(ccp(oSz.width * 0.9, oSz.height * 0.6));
    */
    // demo code
    /*
    CGameUnit* u;
    
    u = pUm->unitByInfo(COrgUnitInfo::kMalik);
    addUnit(u); // �ӵ�UnitLayer��

    u->setForceByIndex(1); // ��������1
    u->setAlly((1 << 1) | (1 << 2)); // ����1������2����
    u->setPosition(ccpAdd(getAnchorPointInPoints(), ccp(rand() % 50 - 25, rand() % 50 - 25))); // ����λ��
    u->setHostilityRange(400);

    CSkill* pSkill = CSpeedBuff::create(1, true, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
    g_iSlowDownKey = pSkill->getKey();
    pSm->addSkill(pSkill); // ��ģ�弼����ӵ����ܹ�������

    u->addSkill(CStatusShowPas::create()); // Ѫ������
    u->addSkill(pOs->skill(COrgSkillInfo::kHpChange1)); // ÿ0.1��ָ�0.2%HP
    u->addSkill(CVampirePas::create(0.3)); // ����30%��Ѫ
    u->addSkill(CThumpPas::create(20, CExtraCoeff(2, 0), 0)); // 20%���2������һ��
    u->addSkill(CThumpPas::create(20, CExtraCoeff(5, 0), 0)); // 20%���5������һ��
    u->addSkill(pOs->skill(COrgSkillInfo::kSlowDown1)); // 100%����10%Ŀ�깥��/�ƶ��ٶȣ��ɵ���
    u->addSkill(CDoubleAttackPas::create(20)); // 50%������������
    u->addSkill(pOs->skill(COrgSkillInfo::kImmo1));
    u->setLevelUpdate(&g_oDemoUpdate);
    u->setMaxLevel(60);
    u->addExp(0);

    // Զ�̹�����λ
    u = pUm->unitByInfo(COrgUnitInfo::kMagnus);
    addUnit(u);
    u->setForceByIndex(2);
    u->setAlly((1 << 1) | (1 << 2));
    u->setPosition(ccpAdd(getAnchorPointInPoints(), ccp(rand() % 50 - 25, rand() % 50 - 25))); // ����λ��
    u->setAttackMinRange(50);
    u->setHostilityRange(400);

    u->addSkill(CStatusShowPas::create()); // Ѫ������
    u->addSkill(CHpChangePas::create(0.1, 0.2, true, -1)); // ÿ0.1��ָ�0.2%HP
    u->addSkill(CVampirePas::create(0.2)); // ����20%��Ѫ
    u->addSkill(CThumpPas::create(20, CExtraCoeff(20, 0), 0)); // 20%���20������һ��
    u->addSkill(CAttackBuffMakerPas::create(100, pSkill->getKey(), 1, CExtraCoeff(1, 0))); // 100%����10%Ŀ�깥��/�ƶ��ٶȣ��ɵ���
    u->addSkill(CDoubleAttackPas::create(50)); // 50%������������
    u->setLevelUpdate(&g_oDemoUpdate);
    u->setMaxLevel(60);
    u->addExp(0);

    // ��
    
    u = pUm->unitByInfo(COrgUnitInfo::kTesla);
    addUnit(u); // �ӵ�UnitLayer��
    u->setForceByIndex(1); // ��������1
    u->setAlly((1 << 1) | (1 << 2)); // ����1������2����
    u->setPosition(ccpAdd(getAnchorPointInPoints(), ccp(50, 0))); // ����λ��

    u->addSkill(CStatusShowPas::create()); // Ѫ������
    u->addSkill(pOs->skill(COrgSkillInfo::kHpChange1)); // ÿ0.1��ָ�0.2%HP
    u->addSkill(pOs->skill(COrgSkillInfo::kHpChangeAura1));

    u = pUm->unitByInfo(COrgUnitInfo::kArcane);
    addUnit(u); // �ӵ�UnitLayer��
    u->setForceByIndex(2); // ��������1
    u->setAlly((1 << 1) | (1 << 2)); // ����1������2����
    u->setPosition(ccpAdd(getAnchorPointInPoints(), ccp(-50, 0))); // ����λ��
    
    u->addSkill(CStatusShowPas::create()); // Ѫ������
    u->addSkill(pOs->skill(COrgSkillInfo::kHpChange1)); // ÿ0.1��ָ�0.2%HP
    u->addSkill(pOs->skill(COrgSkillInfo::kSpeedAura1));
    

    //CCLOG("%d", sizeof(CGameUnit));
    g_oPath.init();
    g_oPath.addPoint(ccp(200, 200));
    g_oPath.addPoint(ccp(200, 400));
    g_oPath.addPoint(ccp(100, 400));
    g_oPath.addPoint(getAnchorPointInPoints());

    */
    return true;
}

void CCHomeSceneLayer::onBtnStartClick(CCObject* pObject)
{
    M_DEF_GM(pGm);
    pGm->pushScene(CCWHomeScene::create());
}

void CCHomeSceneLayer::onBtnCfgClick(CCObject* pObject)
{
    M_DEF_GM(pGm);
    M_DEF_UM(pUm);
    schedule(schedule_selector(CCHomeSceneLayer::onTick), 0.8);
    
    CGameUnit* u = NULL;
    int iIndex = rand() % 3;
    int iForce = rand() % 2 + 3;
    CAttackValue oAv;
    switch (iIndex)
    {
    case 0:
        u = pUm->unitByInfo(COrgUnitInfo::kJt);

        break;

    case 1:
        u = pUm->unitByInfo(COrgUnitInfo::kPaladin);

        u->addSkill(CThumpPas::create(20, CExtraCoeff(2, 0), 0, 1));
        break;

    case 2:
        u = pUm->unitByInfo(COrgUnitInfo::kVeznan);

        u->addSkill(CAttackBuffMakerPas::create(100, g_iSlowDownKey, 1, CExtraCoeff(1, 0))); // 100%����10%Ŀ�깥��/�ƶ��ٶȣ��ɵ���
        break;
    }

    addUnit(u);

    u->setForceByIndex(iForce);
    u->setAlly(1 << iForce);
    u->setPosition(ccp(50 + rand() % 700, 50 + rand() % 380));

    u->addSkill(CStatusShowPas::create());

    u->moveAlongPath(&g_oPath, false, true, 0);

}

void CCHomeSceneLayer::onTick( float fDt )
{
    static float fS = 0;
    fS += fDt;

    if (!getUnits()->getUnitsCount())
    {
        return;
    }
    CGameUnit* pU = dynamic_cast<CGameUnit*>(getUnits()->getUnitsArray()->lastObject());

    if (fS > 5 && fS < 6)
    {
        pU->setExAttackSpeed(CExtraCoeff((rand() % 500) * 0.01, 0));
        pU->setMaxHp(rand() % 2000 + 1000);
        fS = 0;
    }
}


void CCHomeSceneLayer::onBtnStartClick( CCNode* pNode )
{
    M_DEF_DR(pDr);
    pDr->replaceScene(CCTransitionFade::create(0.5, CCWHomeScene::create()));
}

void CCHomeSceneLayer::onBtnUpgradesClick( CCNode* pNode )
{
    M_DEF_DR(pDr);
    pDr->replaceScene(CCTransitionFade::create(0.5,CCTechTreeScene::create()));
}

void CCHomeSceneLayer::onBtnAchievementsClick( CCNode* pNode )
{

}
