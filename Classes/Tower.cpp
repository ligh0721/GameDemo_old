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
#include "GameResControl.h"
#include "Tower.h"

CTowerInfo::CTowerInfo( int iUnitInfoIndex, int iMoneyCost, int iLowGameLevel, int iLowExperience )
	: m_iUnitInfoIndex(iUnitInfoIndex)
	, m_iMoneyCost(iMoneyCost)
	, m_iLowGameLevel(iLowGameLevel)
	, m_iLowExperience(iLowExperience)
{

}

CTowerManager* CTowerManager::sharedTowerManager()
{
	
		static CTowerManager* pTm = NULL;
		if (pTm)
		{
			return pTm;
		}
		pTm = CTowerManager::create();
		pTm->retain();
		return pTm;
}

CTowerManager::CTowerManager()
{

}

CTowerManager::~CTowerManager()
{

}

bool CTowerManager::init()
{
	return true;
}

bool CTowerManager::canUpgrade( int kind, int iUnitInfoIndex )
{
	return true;
}

CTowerInfo* CTowerManager::getUpgradeTower(int kind, int iUnitInfoIndex )
{
	vector<CTowerInfo*>* pVec = m_oUpgradeChain[kind];
	if (canUpgrade(kind, iUnitInfoIndex) && pVec)
	{
			for (unsigned int i = 0; i < pVec->size(); i++)
			{
				if((*pVec)[i]->m_iUnitInfoIndex == iUnitInfoIndex)
				{
					return (*pVec)[i];
				}
			}
				
	}
	return NULL;
}

int CAbstractTowerBuilder::buildTower(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTarget, SEL_CallFuncO pCallFun)
{		

	if(buildBefore(iUnitInfoIndex, roPos, pContext, pTarget, pCallFun) 
		&& building(iUnitInfoIndex, roPos, pContext, pTarget, pCallFun))
	{
		return buildAfter(iUnitInfoIndex, roPos, pContext, pTarget, pCallFun);
	}
	return -1;

}

bool CTowerBuilder::buildBefore(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTarget, SEL_CallFuncO pCallFun)
{
	return true;
}



int CTowerBuilder::buildAfter(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTarget, SEL_CallFuncO pCallFun)
{
	CTowerBuilderSlaver* pSlaver = CTowerBuilderSlaver::create(iUnitInfoIndex, roPos, pContext, pTarget, pCallFun);

	CCProgressBar* pBuildBar = CCProgressBar::create(CCSizeMake(60, 6), CCSprite::createWithSpriteFrameName("bar_white.png")
		, CCSprite::createWithSpriteFrameName("healthbar_border.png"), 1, 1, true);
	pBuildBar->setPosition(roPos);
	CCWinUnitLayer* pLayer = (CCWinUnitLayer*)pContext;
	pLayer->addChild(pBuildBar);
	pBuildBar->setPercentage(100, 0.8, CCCallFuncN::create(pSlaver, callfuncN_selector(CTowerBuilderSlaver::execBuildTower)));
	return iUnitInfoIndex;
}


bool CTowerBuilder::building(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTarget, SEL_CallFuncO pCallFun)
{
	return true;
}


CTowerBuilder* CTowerBuilder::sharedTowerBuilder()
{
	static CTowerBuilder* pTb = NULL;
	if (pTb)
	{
		return pTb;
	}
	pTb = CTowerBuilder::create();
	pTb->retain();
	return pTb;
}

CTowerBuilder::CTowerBuilder()
{

}

CTowerBuilder::~CTowerBuilder()
{

}

bool CTowerBuilder::init()
{
	return true;
}

void CTowerBuilderSlaver::execBuildTower( CCNode* pNode )
{
	CCWinUnitLayer* pLayer = (CCWinUnitLayer*)m_pContext;
	pNode->getParent()->setVisible(false);

	//M_DEF_UM(pUm);
	//CGameUnit* tower= pUm->unitByInfo(m_iUnitInfoIndex);
	CUnitInfoPatchManager * pPatchManager = CGameResController::sharedGameResController()->getCurLevelTowerPatchManager();
	CGameUnit* tower = pPatchManager->unitByIndex(m_iUnitInfoIndex);
	pLayer->addUnit(tower);
	tower->setPosition(m_oPos);
	if(m_pTarget != NULL && m_pCallFun != NULL)
	{
		(m_pTarget->*m_pCallFun)(tower);
	}
}

CTowerBuilderSlaver::CTowerBuilderSlaver()
{

}

CTowerBuilderSlaver::~CTowerBuilderSlaver()
{

}

bool CTowerBuilderSlaver::init( int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTarget, SEL_CallFuncO pCallFun )
{
	m_iUnitInfoIndex = iUnitInfoIndex;
	m_oPos = roPos;
	m_pContext = pContext;
	m_pTarget = pTarget;
	m_pCallFun = pCallFun;
	return true;
}
