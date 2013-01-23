#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "Action.h"
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
	pFc->addSpriteFramesWithFile("background.plist");
	pFc->addSpriteFramesWithFile("tank.plist");
	pFc->addSpriteFramesWithFile("UI.plist");
	pFc->addSpriteFramesWithFile("tower.plist");
	CCSprite* pSprite = CCSprite::createWithSpriteFrameName("kr/LevelDemo_480_800.png");
	setBackGroundSprite(pSprite);
	setBufferEffectParam(0.9, 10, 0.1);
	pSprite->setScale(1.0);

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

	loadTowerPos();

	m_oTowerShowMenu.init(2, 2, 46, 10, 5, NULL);
	addChild(&m_oTowerShowMenu);

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

bool CCBattleSceneLayer::loadUnitRoute()
{
	m_oUnitPath.init();
	CCPoint arrayRoute1[]={
		ccp(830, 340), ccp(630, 376), ccp(448, 374), ccp(298, 340), ccp(315,275),
		ccp(589, 182), ccp(561, 132), ccp(454, 124), ccp(215, 184), ccp(-40, 165)};
	for (int i=0; i< sizeof(arrayRoute1)/sizeof(CCPoint); i++)
	{
		m_oUnitPath.addPoint(arrayRoute1[i]);
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
	static float fTotal= 0;
	M_DEF_GM(pGm);
	M_DEF_FC(pFc);

	
	CCArray* pArrUnit = getUnits()->getUnitsArray();
	//CCAssert(m_vecUnitRoute.size() > 0, "unit route amount must great 0");
	//vector<CCPoint> * pVecPoint =  m_vecUnitRoute[0];

	//for (vector<CGameUnit*>::size_type i = 0; i < m_vecCoalition.size(); i++)
	//{
		/*for (vector<CCPoint>::size_type j = 0; j < pVecPoint->size(); j++)
		{
			CGameUnit* pUnit = m_vecCoalition[i];
			if (pUnit != NULL && !pUnit->isDead())
			{
				if (pUnit != NULL && !pUnit->isDead() && (*pVecPoint)[j].equals(pUnit->getPosition()))
				{
					if(j == pVecPoint->size() - 1)
					{
						pUnit->setPosition((*pVecPoint)[0]);
						pUnit->moveTo((*pVecPoint)[1], true, false);
					}
					else
					{
						pUnit->moveTo((*pVecPoint)[j+1], true, false);
					}

					continue;
				}
			}
			else
			{
				m_vecCoalition[i] = NULL;
			}
			
 		}*/
	//}

	if((fTotal += fDt) > 2 && m_vecCoalition.size() < 6)
	{
		fTotal = 0;
		CGameUnit* u = randomSoldiers(1);
		m_vecCoalition.push_back(u);
		//CCAssert(pVecPoint->size() > 1, "unit route point amount must great 1");
		u->setPosition(ccp(830, 340));
		u->moveAlongPath(&m_oUnitPath, false, true);
		//u->setPosition(m_oUnitPath.m_oVecPoints[0]);
		//u->setBaseMoveSpeed(40);
	}
	
}

CGameUnit* CCBattleSceneLayer::randomSoldiers(int iForce)
{
	M_DEF_UM(pUm);
	CGameUnit* u=pUm->unitByInfo(COrgUnitInfo::kJt);
	addUnit(u);
	u->setPosition(ccp(1000,600));
	//heroKey=u->getKey();
	u->setForceByIndex(2);
	u->setAlly(1<<2);
	u->addSkill(CStatusShowPas::create());
	u->prepareAttackAnimation(1,CGameUnit::kAnimationAct1,"act1",0.1);
	u->setBaseAttackInterval(0.4);
	u->setMaxHp(100);
	//heroUnit->setBaseMoveSpeed(100);
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
	CTowerBuilder::sharedTowerBuilder()->buildTower(rand()%2, m_oCurTowerPos, this, NULL, NULL);
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
	tower->setForceByIndex(2);
	tower->setAlly(1<<2);
	tower->addSkill(CStatusShowPas::create());
	tower->prepareAttackAnimation(1,CGameUnit::kAnimationAct1,"act1",0.1);
	tower->addSkill(pOs->skill(COrgSkillInfo::kHpChangeAura1));
}
