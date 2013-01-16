#ifndef __COMPLEXUNIT_H_
#define  __COMPLEXUNIT_H_

class CCUnitLayer;

class CComplexUnit :public CCObject
{
public:
	CComplexUnit();
	virtual ~CComplexUnit();

	virtual bool init(CCUnitLayer* pUnit);
	//CREATE_FUNC_PARAM(CComplexUnit, (CCUnitLayer* pUnit), pUnit);

	CCUnitLayer* getUnitLayer();

	virtual void onTick(float fVal);

	virtual void setPosition(const CCPoint& pos);

	virtual const  CCPoint& getPosition()=0;
protected:
	CCUnitLayer* m_pUnitLayer;

};

class CSoilderTower : public CComplexUnit
{
public:
	enum DOOR_ACT
	{
		kOpen = 1,
		kClopse
	};

	CSoilderTower();
	virtual ~CSoilderTower();

	virtual bool init(CCUnitLayer* pUnitLayer,  int eUnitInfoIndex , int iSoilderCount, const char* pTowerName);
	CREATE_FUNC_PARAM(CSoilderTower, (CCUnitLayer* pUnitLayer,   int eUnitInfoIndex, int iSoilderCount, const char* pTowerName), pUnitLayer, eUnitInfoIndex, iSoilderCount, pTowerName);

	void onTick(float fVal);

	void setPosition(const CCPoint& pos);

	const CCPoint& getPosition();

private:

	void operateDoor(DOOR_ACT eDoorAct, float fDelay);

private:
	int m_eUnitInfoIndex;
	int m_iSoilderCount;
	CCSprite* m_pTowerSprite;
	const char* m_pTowerName;
};
#endif