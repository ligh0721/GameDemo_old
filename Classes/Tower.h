#ifndef __TOWER_H_
#define __TOWER_H_

class CGameUnit;
class CAbstractTowerBuilder : public CCObject
{
public:
	virtual int buildTower(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTarget=NULL, SEL_CallFuncO pCallFun=NULL);

protected:
	virtual bool buildBefore(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTargent, SEL_CallFuncO pCallFun)=0;
	virtual bool building(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTargent, SEL_CallFuncO pCallFun)=0;
	virtual int buildAfter(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTargent, SEL_CallFuncO pCallFun)=0;
protected:
};

class CTowerBuilder : public CAbstractTowerBuilder
{
public:
	CTowerBuilder();
	virtual ~CTowerBuilder();

	bool init();
	CREATE_FUNC(CTowerBuilder);

	static CTowerBuilder* sharedTowerBuilder();

	
protected:
	bool buildBefore(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTargent, SEL_CallFuncO pCallFun);
	bool building(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTargent, SEL_CallFuncO pCallFun);
	int buildAfter(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTargent, SEL_CallFuncO pCallFun);
private:
};
class CTowerBuilderSlaver : public CCObject
{
public:
	CTowerBuilderSlaver();
	virtual ~CTowerBuilderSlaver();

	bool init(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTargent, SEL_CallFuncO pCallFun);
	CREATE_FUNC_PARAM(CTowerBuilderSlaver,
		(int iUnitInfoIndex, const CCPoint& roPos, void* pContext, CCObject* pTargent, SEL_CallFuncO pCallFun),
		iUnitInfoIndex, 
		roPos, 
		pContext,
		pTargent, 
		pCallFun)

	virtual void execBuildTower(CCNode* pNode);

public:
	int m_iUnitInfoIndex;
	CCPoint m_oPos;
	void* m_pContext;
	CCObject* m_pTarget;
	SEL_CallFuncO m_pCallFun;
};

class CTowerInfo : public CCObject
{
public:
	CTowerInfo(int iUnitInfoIndex, int iMoneyCost, int iLowGameLevel, int iLowExperience);

public:
	int m_iUnitInfoIndex;
	int m_iMoneyCost;
	int m_iLowGameLevel;
	int m_iLowExperience;

};
class CTowerManager : public CCObject
{
public:
	CTowerManager();
	virtual ~CTowerManager();

	bool init();
	CREATE_FUNC(CTowerManager);

	static CTowerManager* sharedTowerManager();

	bool canUpgrade(int kind, int iUnitInfoIndex);

	CTowerInfo* getUpgradeTower(int kind, int iUnitInfoIndex);

private:
	map<int, vector<CTowerInfo*>* > m_oUpgradeChain;
};
#endif
