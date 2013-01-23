#ifndef __GAMERESCONTROL_H_
#define  __GAMERESCONTROL_H_

class CUnitInfoPatchManager;

class  CGameResController : public  CCObject
{
	typedef map<int, CUnitInfoPatchManager*>  LEVEL2PATCHMANAGER;

public:
	CGameResController();
	virtual ~CGameResController();
	
	bool init();
	CREATE_FUNC(CGameResController);
	static CGameResController* sharedGameResController();

	bool initCurLevelRes();
	CUnitInfoPatchManager* getCurLevelTowerPatchManager();

protected:
	CUnitInfoPatchManager* getTowerPatchManagerByLevel(int level);
	bool initResWithLevel(int level);

private:
	LEVEL2PATCHMANAGER m_oTowerLevelPatchManager;
};
#endif