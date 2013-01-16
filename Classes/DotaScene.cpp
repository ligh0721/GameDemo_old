#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "Unit.h"
#include "Skill.h"
#include "Action.h"
#include "BattleScene.h"
#include "DotaScene.h"
#include "MissionInfo.h"
#include "ChildOfGameUnit.h"
#include "WHomeScene.h"
#include "GameCtrl.h"


bool CCDotaScene::init()
{
    CCScene::init();

    CCDotaCtrlLayer* pCtrlLayer = CCDotaCtrlLayer::create();
    addChild(pCtrlLayer, 10);

    m_pDotaSceneLayer = CCDotaSceneLayer::create();
    addChild(m_pDotaSceneLayer);
    pCtrlLayer->setDotaSceneLayer(m_pDotaSceneLayer);

    return true;
}

CCDotaSceneLayer::~CCDotaSceneLayer()
{
}

CCDotaSceneLayer::CCDotaSceneLayer()
{
	 
}

bool CCDotaSceneLayer::init()
{
	CCWinUnitLayer::init();


	CCSprite* pSprite = CCSprite::create("LevelDemo2HD.png");
	pSprite->setScale(1.0);
    setBackGroundSprite(pSprite);

	M_DEF_GM(pGm);
    M_DEF_FC(pFc);
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
	pFc->addSpriteFramesWithFile("unit.plist");

	CCSize oSize = CCDirector::sharedDirector()->getVisibleSize();
	CCLOG("size.x=%.1f,size.y=%.1f", oSize.width, oSize.height);

    pFc->addSpriteFramesWithFile("unit.plist");
    pFc->addSpriteFramesWithFile("UI.plist");
    pFc->addSpriteFramesWithFile("tank.plist");
    pGm->loadUnitAnimation("Malik", "move");
    pGm->loadUnitAnimation("Malik", "die");
    pGm->loadUnitAnimation("Malik", "act1");
    pGm->loadUnitAnimation("Malik", "act2");
    pGm->loadUnitAnimation("Malik", "act3");
    pGm->loadUnitAnimation("Malik", "act4");
    pGm->loadUnitAnimation("Malik", "act5");

    pGm->loadUnitAnimation("Paladin", "move");
    pGm->loadUnitAnimation("Paladin", "die");
    pGm->loadUnitAnimation("Paladin", "act1");
    pGm->loadUnitAnimation("Paladin", "act2");
    pGm->loadUnitAnimation("Paladin", "act3");
    pGm->loadUnitAnimation("Paladin", "act4");

    pGm->loadUnitAnimation("Magnus", "move");
    pGm->loadUnitAnimation("Magnus", "die");
    pGm->loadUnitAnimation("Magnus", "act1");
    pGm->loadUnitAnimation("Magnus", "act2");
    pGm->loadUnitAnimation("Magnus", "act3");
    pGm->loadUnitAnimation("Magnus", "act4");
    pGm->loadUnitAnimation("Magnus", "act5");
    pGm->loadUnitAnimation("Magnus", "act6");

    pGm->loadUnitAnimation("Arcane","act1");
    pGm->loadUnitAnimation("Arcane","die");
   
    CGameUnit* tower;
    tower=CGameUnit::createWithName("Arcane");
    addUnit(tower);
    tower->setPosition(ccp(10,10));

    setUnitTickInterval(0.1);

    pGm->loadUnitAnimation("Jt", "move");
    pGm->loadUnitAnimation("Jt", "die");
    pGm->loadUnitAnimation("Jt", "act1");

    CGameUnit *u = CGameUnit::createWithName("Jt", ccp(0.5, 11.0 / 133));

    u->prepareMoveAnimation("move", 0.1);
    u->prepareDieAnimation("die", 0.1);
    u->prepareAttackAnimation(1,
        CGameUnit::kAnimationAct1, "act1", 0.1
        );

    u->setBaseAttackInterval(5);
    u->setAttackEffectDelay(0.7);
    u->setAttackMinRange(1);
    u->setAttackRange(20);
    u->setWeaponType(CGameUnit::kWTClose);
    CAttackValue oAv;
    oAv.setZero();
    oAv.setAttack(CAttackValue::kPhysical, 33);
    u->setBaseAttackValue(oAv);
    u->setArmorType(CArmorValue::kNormal);
    u->setBaseArmorValue(5);

    u->setHalfOfWidth(46);
    u->setHalfOfHeight(50);
    u->setMaxHp(300);
   
    return true;
}

void CCDotaSceneLayer::onBtnDotaClick(CCObject* pObject)
{
    schedule(schedule_selector(CCDotaSceneLayer::onTick1), 0.1);
}

void CCDotaSceneLayer::onTick1( float fDt )
{
    CCLOG("????");
    MissionInfo *dotaInfo=MissionInfo::getComingMissionInfo();
    M_DEF_GM(pGm);
    static float fS = 0;
    fS += fDt;
    static bool firstTick=true;
    char* s[3];
    s[0]="Paladin";
    s[1]="Malik";
    s[2]="Magnus";
    if(firstTick)
    {
        firstTick=false;
    
    for (int i=0;i<3;i++)
    {
        for (int m=0;m<dotaInfo->getAdvanceInfoByIndex(0)->m_waveOfSoldier[1][i];m++)
        {
            
            CSimpleGameUnit* u=CSimpleGameUnit::createWithName(s[i]);
            addUnit(u);
            setSoldierDataByIndex(u,i);
            u->setForceByIndex(0);
            u->setAlly(1 << 0);
            u->addSkill(CStatusShowPas::create());

            CCPoint appearPoint=dotaInfo->getAdvanceInfoByIndex(0)->getAppearPoint();
            appearPoint.x=appearPoint.x+rand()%50;
            appearPoint.y=appearPoint.y+rand()%50;
            u->setPosition(appearPoint);
           // u->setPosition(ccp(1000,700));
            CCLOG("should appear,%f,%f",appearPoint.x,appearPoint.y);
        }
        for (int n=0;n<dotaInfo->getAdvanceInfoByIndex(1)->m_waveOfSoldier[1][i];n++)
        {
            CSimpleGameUnit* u=CSimpleGameUnit::createWithName(s[i]);
            addUnit(u);
            setSoldierDataByIndex(u,i);
            u->setAppearPointIndex(1);
            u->setForceByIndex(1);
            u->setAlly(1 << 1);


            u->addSkill(CStatusShowPas::create());
            CCPoint appearPoint=dotaInfo->getAdvanceInfoByIndex(1)->getAppearPoint();
            appearPoint.x=appearPoint.x+rand()%50;
            appearPoint.y=appearPoint.y+rand()%50;
            u->setPosition(appearPoint);  
        }
        for (int p=0;p<dotaInfo->getAdvanceInfoByIndex(2)->m_waveOfSoldier[1][i];p++)
        {
            CSimpleGameUnit* u=CSimpleGameUnit::createWithName(s[i]);
            addUnit(u);
            setSoldierDataByIndex(u,i);
            u->setAppearPointIndex(2);
            u->setForceByIndex(2);
            u->setAlly(1<<2);

            u->addSkill(CStatusShowPas::create());
            CCPoint appearPoint=dotaInfo->getAdvanceInfoByIndex(2)->getAppearPoint();
            appearPoint.x=appearPoint.x+rand()%50;
            appearPoint.y=appearPoint.y+rand()%50;
            u->setPosition(appearPoint); 
        }
    }
    }
}

void CCDotaSceneLayer::setSoldierDataByIndex( CSimpleGameUnit* u,int index )
{
    if (index==0)
    {
        u->setHalfOfWidth(9);
        u->setHalfOfHeight(10);
        u->prepareMoveAnimation("move", 0.05);
        u->prepareDieAnimation("die", 0.1);
        u->prepareAttackAnimation(4,
            CGameUnit::kAnimationAct1, "act1", 0.1,
            CGameUnit::kAnimationAct2, "act2", 0.1,
            CGameUnit::kAnimationAct3, "act3", 0.1,
            CGameUnit::kAnimationAct4, "act4", 0.1
            );
        u->setHostilityRange(100);
        u->setBaseMoveSpeed(50);
        u->getSprite()->setAnchorPoint(ccp(0.5,0.25));
        u->setBaseAttackInterval(0.5);
        u->setAttackEffectDelay(0.2);
        u->setAttackMinRange(0);
        u->setAttackRange(13);
        u->setWeaponType(CGameUnit::kWTClose);
        u->setMaxHp(100);
        CAttackValue oAv(1, CAttackValue::kMagical, 5);
        u->setBaseAttackValue(oAv);
    }
    else if (index==1)
    {
        u->setHalfOfWidth(11);
        u->setHalfOfHeight(13);
        u->prepareMoveAnimation("move", 0.05);
        u->prepareDieAnimation("die", 0.1);
        u->prepareAttackAnimation(4,
            CGameUnit::kAnimationAct1, "act1", 0.1,
            CGameUnit::kAnimationAct2, "act2", 0.1,
            CGameUnit::kAnimationAct3, "act3", 0.1,
            CGameUnit::kAnimationAct4, "act4", 0.1
            );

        u->setHostilityRange(100);
        u->getSprite()->setAnchorPoint(ccp(0.5,0.1125));
        u->setMaxHp(100);
        CAttackValue oAv(1, CAttackValue::kMagical, 5);
        u->setBaseAttackValue(oAv);
        u->setBaseMoveSpeed(50);
        u->setBaseAttackInterval(2);

        u->setAttackEffectDelay(0.4); // 攻击前摇延迟
        //u->setExAttackSpeed(CExtraCoeff(2, 0)); // 攻击多项式系数 ax+b
        u->setAttackMinRange(0); // 攻击最短距离，小于该距离不可立即攻击，参考投石车
        u->setAttackRange(15); // 攻击距离，大于该距离不可立即攻击
        u->setWeaponType(CGameUnit::kWTClose); // 武器类型为近战
    }
    else if (index==2)
    {

    }
}

bool CCDotaSceneLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint touchPoint=pTouch->getLocation();
    CCLOG("%f,%f",touchPoint.x,touchPoint.y);
    return true;
}

bool CCDotaCtrlLayer::init()
{
    CCLayer::init();

    m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);

    m_oBtnDota.initWithString("Dota", this, menu_selector(CCDotaCtrlLayer::onBtnDotaClick));
    m_oMenu.addChild(&m_oBtnDota);
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    m_oBtnDota.setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));
    
    return true;
}

void CCDotaCtrlLayer::onBtnDotaClick( CCObject* pObject )
{
    getDotaSceneLayer()->onBtnDotaClick(pObject);
}
