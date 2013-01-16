#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "Action.h"
#include "GameCtrl.h"
#include "MainScene.h"
#include "MissionInfo.h"
#include "ChildOfGameUnit.h"
#include "UnitInfo.h"
#include "SomeScene.h"
#include "TechTreeScene.h"
#include "SkillInfo.h"
#include "BattleScene.h"
#include "ComplexUnit.h"


CComplexUnit::CComplexUnit()
	: m_pUnitLayer(NULL)
{

}

CComplexUnit::~CComplexUnit()
{

}

bool CComplexUnit::init( CCUnitLayer* pUnit )
{
	 m_pUnitLayer = pUnit;
	 
	 return true;
}

CCUnitLayer* CComplexUnit::getUnitLayer()
{
	return m_pUnitLayer;
}

void CComplexUnit::onTick( float fVal )
{

}

void CComplexUnit::setPosition( const CCPoint& pos )
{

}



bool CSoilderTower::init( CCUnitLayer* pUnitLayer,  int eUnitInfoIndex , int iSoilderCount, const char* pTowerName )
{
	CComplexUnit::init(pUnitLayer);
	m_eUnitInfoIndex = eUnitInfoIndex;
	m_iSoilderCount = iSoilderCount;
	m_pTowerName = pTowerName;

	return true;
}


CSoilderTower::CSoilderTower()
{

}

CSoilderTower::~CSoilderTower()
{

}

void CSoilderTower::onTick( float fVal )
{

}

void CSoilderTower::setPosition(const CCPoint& pos )
{
	m_pTowerSprite->setPosition(pos);
}

const CCPoint& CSoilderTower::getPosition()
{
	return m_pTowerSprite->getPosition();
}

void CSoilderTower::operateDoor(DOOR_ACT eDoorAct, float fDelay)
{
	 M_DEF_GM(pGm);
	 CCAnimation* pAni = pGm->getUnitAnimation(m_pTowerName, "act1");
	 pAni->setDelayPerUnit(fDelay);
	 CCAction* pAct = CCAnimate::create(pAni);
	// switch (fDelay)
	 //{
	 //case 1:
		// pAct = CCSequence::create(dynamic_cast<CCAnimate*>(pAct), pEndAction, NULL);
		//break;
	 //case 2:
		 //pAct = CCSequence::create(dynamic_cast<CCAnimate*>(pAct), NULL);
		//break;
	 //}
	 
	 m_pTowerSprite->runAction(pAct);

}
