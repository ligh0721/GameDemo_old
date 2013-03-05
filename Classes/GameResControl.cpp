#include "CommInc.h"

#include "GameLevel.h"
#include "Unit.h"
#include "GameResControl.h"
CGameResController::CGameResController()
{

}

CGameResController::~CGameResController()
{
	LEVEL2PATCHMANAGER::iterator it = m_oTowerLevelPatchManager.begin();
	while (it != m_oTowerLevelPatchManager.end())
	{
		delete it->second;
	}

}

bool CGameResController::init()
{
	return true;
}

CGameResController* CGameResController::sharedGameResController()
{

	static CGameResController* pGRC = NULL;
	if (pGRC)
	{
		return pGRC;
	}
	pGRC = CGameResController::create();
	pGRC->retain();
	return pGRC;
}

CUnitInfoPatchManager* CGameResController::getCurLevelTowerPatchManager()
{
	return getTowerPatchManagerByLevel(CGameLevelManager::sharedGameLevelManager()->getCurLevel());

}

CUnitInfoPatchManager* CGameResController::getTowerPatchManagerByLevel( int level )
{
	LEVEL2PATCHMANAGER::iterator it = m_oTowerLevelPatchManager.find(level);
	if (it != m_oTowerLevelPatchManager.end())
		return it->second;
	return NULL;
}

bool CGameResController::initResWithLevel( int level )
{
	if (getTowerPatchManagerByLevel(level) ==  NULL)
	{
		char arrPath[128];
		sprintf(arrPath, "levels/level%02d/Level.uip", level);
		CUnitInfoPatchManager* pUnitInfoManager = new CUnitInfoPatchManager();
		pUnitInfoManager->initWithFile(arrPath);
		m_oTowerLevelPatchManager.insert(make_pair(level, pUnitInfoManager));
	}
	return true;
	
}

bool CGameResController::initCurLevelRes() 
{
	return initResWithLevel(CGameLevelManager::sharedGameLevelManager()->getCurLevel());
}

