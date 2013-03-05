#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "MainScene.h"
#include "MissionInfo.h"
#include "ChildOfGameUnit.h"
#include "UnitInfo.h"
#include "SomeScene.h"
#include "TechTreeScene.h"
#include "SkillInfo.h"
#include "BattleScene.h"
#include "Unit.h"
#include "GameCtrl.h"
#include "Tower.h"
#include "GameChapterScene.h"

bool CCBattleScene::init()
{
    CCScene::init();
    m_pBattleSceneLayer = CCBattleSceneLayer::create();
    addChild(m_pBattleSceneLayer);

    return true;
}

CCBattleSceneLayer::~CCBattleSceneLayer()
{
}
CCBattleSceneLayer::CCBattleSceneLayer()
{
	 
}

bool CCBattleSceneLayer::init()
{
	CCWinUnitLayer::initWithColor(ccc4(204, 232, 207, 64));
	CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GM(pGm);
	M_DEF_FC(pFc);
    M_DEF_OS(pOs);
	pFc->addSpriteFramesWithFile("background.plist");
	pFc->addSpriteFramesWithFile("tank.plist");
	pFc->addSpriteFramesWithFile("UI.plist");
	pFc->addSpriteFramesWithFile("tower.plist");
	CCSprite* pSprite = CCSprite::createWithSpriteFrameName("kr/LevelDemo_480_800.png");
	setBackGroundSprite(pSprite);
	setBufferEffectParam(0.9, 10, 0.1);
	pSprite->setScale(2.0);

	m_oMenu.init();
	addChild(&m_oMenu);
	m_oMenu.setPosition(CCPointZero);

    m_oBtnBattle.initWithString("start", this, menu_selector(CCBattleSceneLayer::onBtnBattleClick));
    m_oMenu.addChild(&m_oBtnBattle);
    m_oBtnBattle.setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));
	
	
	m_oBtnUpGrade.initWithString("upgrade", this, menu_selector(CCBattleSceneLayer::onBtnUpgradeClick));
	m_oMenu.addChild(&m_oBtnUpGrade);
	m_oBtnUpGrade.setPosition(ccp(oSz.width * 0.3, oSz.height * 0.3));

	m_oBtnHome.initWithString("home", this, menu_selector(CCBattleSceneLayer::onBtnHomeClick));
	m_oMenu.addChild(&m_oBtnHome);
	m_oBtnHome.setPosition(ccp(oSz.width * 0.6, oSz.height * 0.3));
    
    m_oBtnHome.initWithString("lighting", this, menu_selector(CCBattleSceneLayer::onBtnLightgingSkillClick));
	m_oMenu.addChild(&m_oBtnLightingSkill);
	m_oBtnLightingSkill.setPosition(ccp(oSz.width * 0.2, oSz.height * 0.5));
    

	loadUnitRoute();
	loadTowerPos();
	randomSoldiers(1);
	m_oTowerShowMenu.init(2, 2, 46, 10, 5, NULL);
	addChild(&m_oTowerShowMenu);
    
    
    m_oSkillPanel.init(5, 1, 64, 7, 5, "Skill5_78x352_7_5_black.png");
    this->addChild(&m_oSkillPanel);
    m_oSkillPanel.setPosition(ccp(100, 400));
    
//    CActiveSkill* pSkill = dynamic_cast<CActiveSkill*>(pOs->skill(COrgSkillInfo::kChainLighting1));
//    pSkill->setCastAniInfo(CGameUnit::kAnimationAct5, 0.4);
//    CGameUnit* heroUnit = getUnitByKey(m_iHeroUnitKey);
//    heroUnit->addSkill(pSkill);
//    CCSkillButtonAdvance* pBtn;
//    pBtn = M_CREATE_SKILL("skill2", heroUnit->getKey(), pSkill->getKey(), this);
//    m_oSkillPanel.addButton(pBtn, 0, 4);

	CCCommmButton*  pTowerIcon = CCCommmButton::create(
		"Image 314.png", 
		"Image 314.png", 
		"Image 314.png",
		NULL,
		NULL,
		0,
		this, 
		(SEL_CallFuncN)&CCBattleSceneLayer::buildTower, NULL, kTagArrowTower);
	m_oTowerShowMenu.addButton(pTowerIcon, 0, 1);

	pTowerIcon = CCCommmButton::create(
		"Image 312.png", 
		"Image 312.png", 
		"Image 312.png",
		NULL,
		NULL,
		0,
		this, 
		(SEL_CallFuncN)&CCBattleSceneLayer::buildTower, NULL, kTagMagicTower);
	m_oTowerShowMenu.addButton(pTowerIcon, 1, 1);


	pTowerIcon = CCCommmButton::create(
		"Image 313.png", 
		"Image 313.png", 
		"Image 313.png",
		NULL,
		NULL,
		0,
		this, 
		(SEL_CallFuncN)&CCBattleSceneLayer::buildTower, NULL, kTagSoilderTower);
	m_oTowerShowMenu.addButton(pTowerIcon, 0, 0);

	pTowerIcon = CCCommmButton::create(
		"Image 311.png", 
		"Image 311.png", 
		"Image 311.png",
		NULL,
		NULL,
		0,
		this, 
		(SEL_CallFuncN)&CCBattleSceneLayer::buildTower, NULL, kTagGunTower);
	m_oTowerShowMenu.addButton(pTowerIcon, 1, 0);

	m_oTowerShowMenu.setVisible(false);


	
	//for (int  i = 0; i < 6; i++)
	//{
		//CGameUnit* u = randomSoldiers(2);
		//u->setPosition(ccp(446 + (i/3)*30, 104 + (i%3)*20));
		
	//}

	CCSize oSize = CCDirector::sharedDirector()->getWinSize();
	CCLOG("size.x=%d,size.y=%d", oSize.width, oSize.height);

	oSize = CCDirector::sharedDirector()->getWinSizeInPixels();
	CCLOG("size.x=%d,size.y=%d", oSize.width, oSize.height);

	oSize = CCDirector::sharedDirector()->getVisibleSize();
	CCLOG("size.x=%d,size.y=%d", oSize.width, oSize.height);
    return true;
}

void CCBattleSceneLayer::onBtnBattleClick(CCObject* pObject)
{
	if (m_iGameStatus == kTagStart)
		return;
	m_iGameStatus = kTagStart;

	M_DEF_GM(pGm);
	schedule(schedule_selector(CCBattleSceneLayer::onTick), 0.2);
	
}


void CCBattleSceneLayer::onBtnHomeClick(CCObject* pObject)
{
	
	CCDirector::sharedDirector()->replaceScene( CCGameChapterScene::create());

}

void CCBattleSceneLayer::onBtnLightgingSkillClick(CCObject* pObject)
{
	
	
    
}

bool CCBattleSceneLayer::loadUnitRoute()
{
	m_oUnitPath.init();
	CCPoint arrayRoute1[]={
		ccp(830, 340), ccp(630, 376), ccp(448, 374), ccp(298, 340), ccp(315,275),
		ccp(589, 182), ccp(561, 132), ccp(454, 124), ccp(215, 184), ccp(-40, 165)};
	for (int i=0; i < sizeof(arrayRoute1)/sizeof(CCPoint); i++)
	{
		m_oUnitPath.addPoint(arrayRoute1[i]);
	}
    
    for (int i = sizeof(arrayRoute1)/sizeof(CCPoint)-1; i >= 0; i--)
	{
		m_oReverseUnitPath.addPoint(arrayRoute1[i]);
	}
    
	
	/*
	CCPoint arrayRoute2[]={
			ccp(796, 257), ccp(586, 294), ccp(458, 285), ccp(446, 238),
			ccp(596, 182), ccp(454, 124), ccp(215, 184), ccp(-1, 186)};
	vector<CCPoint>* vecRoute2 = new vector<CCPoint>();
	for (int i=0; i< sizeof(arrayRoute2)/sizeof(CCPoint); i++)
	{
		vecRoute2->push_back(arrayRoute2[i]);
	}
	m_oUnitPath
	m_vecUnitRoute.push_back(vecRoute2);
	*/
	return true;
}

void CCBattleSceneLayer::onTick( float fDt )
{
	static int iTotal = 0;
	M_DEF_GM(pGm);
	M_DEF_FC(pFc);
    
    if (iTotal < 40 && iTotal % 8 == 0)
    {
        randomSoldiers(2);
    }
    iTotal++;
	/*
	CCArray* pArrUnit = getUnits()->getUnitsArray();

	if((fTotal += fDt) > 2 && m_vecCoalition.size() < 6)
	{
		fTotal = 0;
		CGameUnit* u = randomSoldiers(1);
		m_vecCoalition.push_back(u);
		u->setPosition(ccp(830, 340));
		u->moveAlongPath(&m_oUnitPath, false, true);
	}
	*/
	
}

CGameUnit* CCBattleSceneLayer::randomSoldiers(int iForce)
{
	M_DEF_UM(pUm);
    M_DEF_OS(pOs);
	CGameUnit* u = NULL;
    if(iForce == 1)
    {
        u = pUm->unitByInfo(COrgUnitInfo::kMatchstick);
        m_iHeroUnitKey = u->getKey();
        u->moveAlongPath(&m_oUnitPath, false);
        u->addSkill(pOs->skill(COrgSkillInfo::kSwordStorm1));
        u->setPosition(ccp(1000,600));
        u->setMaxHp(500);
    }
    else if(iForce == 2)
    {
        u = pUm->unitByInfo(COrgUnitInfo::kMagnus);
        u->moveAlongPath(&m_oReverseUnitPath, false);
        u->setPosition(ccp(-40, 165));
    }
    
	addUnit(u);
	
	//heroKey=u->getKey();
	u->setForceByIndex(iForce);
	u->setAlly(1<<iForce);
	u->addSkill(CStatusShowPas::create());
	u->prepareAttackAnimation(1,CGameUnit::kAnimationAct1,"act1",0.1);
	u->setBaseAttackInterval(2.0);
	u->setMaxHp(1000);
	u->setBaseMoveSpeed(50);
	u->setAttackMinRange(0);
  
	return u;
}

bool CCBattleSceneLayer::loadTowerPos()
{
	CCPoint arrPos[] = {
		ccp(618, 331), ccp(510, 337), 
		ccp(400, 324), ccp(545, 422), 
		ccp(387, 413), ccp(218, 277), 
		ccp(331, 212), ccp(662, 116),
		ccp(626, 243), ccp(304, 112)};
	m_oTowerPosMenu.init();
	m_oTowerPosMenu.setPosition(CCPointZero);
	addChild(&m_oTowerPosMenu);
	for (int i = 0; i < 10; i++)
	{
		CCMenuItemImage* pPosImg = CCMenuItemImage::create("tower_pos_min.png", "tower_pos_min.png",
			"tower_pos_min.png", this, (SEL_MenuHandler)&CCBattleSceneLayer::showTower);
		m_oTowerPosMenu.addChild(pPosImg);
		pPosImg->setPosition(arrPos[i]);

	}
	return true;

}

void CCBattleSceneLayer::showTower( CCObject* pObject )
{
	CCNode* pNode = dynamic_cast<CCNode*>(pObject);
	m_oTowerShowMenu.setPosition(ccpAdd(pNode->getPosition(), ccp(60, 10)));
	m_oTowerShowMenu.setVisible(true);
	m_oCurTowerPos = pNode->getParent()->convertToWorldSpace(pNode->getPosition());

}

void CCBattleSceneLayer::buildTower( CCNode* pNode)
{
	m_oTowerShowMenu.setVisible(false);
	CTowerBuilder::sharedTowerBuilder()->buildTower(rand()%2, m_oCurTowerPos, this, this, callfuncO_selector(CCBattleSceneLayer::initTower));
	/*
	m_oTowerShowMenu.setVisible(false);
	M_DEF_UM(pUm);
	CGameUnit* tower= pUm->unitByInfo(COrgUnitInfo::kArcane);
	tower->getSprite()->setAnchorPoint(ccp(0.60, 0.30));
	addUnit(tower);
	tower->setPosition(m_oCurTowerPos);
	tower->setForceByIndex(2);
	tower->setAlly(1<<2);
	tower->addSkill(CStatusShowPas::create());
	tower->prepareAttackAnimation(1,CGameUnit::kAnimationAct1,"act1",0.1);
	tower->addSkill(pOs->copySkill(COrgSkillInfo::kHpChangeAura1));
	*/
}

bool CCBattleSceneLayer::ccTouchBegan( CCTouch* touch, CCEvent* event )
{   
	CCWinUnitLayer::ccTouchBegan(touch, event);
	if(m_oTowerShowMenu.isVisible())
	{

		m_oTowerShowMenu.setVisible(false);
	}
	return true;
}

void CCBattleSceneLayer::onBtnUpgradeClick( CCObject* pObject )
{
	CCLOG("upgrade");
	M_DEF_UM(pUm);
    M_DEF_OS(pOs);
	CGameUnit* tower= pUm->unitByInfo(COrgUnitInfo::kArcane);
	addUnit(tower);
	tower->setPosition(ccp(200, 200));
	tower->setForceByIndex(1);
	tower->setAlly(1<<1);
	tower->addSkill(CStatusShowPas::create());
	tower->prepareAttackAnimation(1,CGameUnit::kAnimationAct1,"act1",0.1);
	tower->addSkill(pOs->skill(COrgSkillInfo::kHpChangeAura1));
}

void CCBattleSceneLayer::initTower( CCObject* pObject )
{
	CGameUnit* pTower = dynamic_cast<CGameUnit*>(pObject);
	M_DEF_OS(pOs);
	pTower->addSkill(pOs->skill(COrgSkillInfo::kTransmit1));
	pTower->setForceByIndex(2);
	pTower->setAlly(1<<2);
}
