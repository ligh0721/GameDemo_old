#ifndef __PACKAGE_H_
#define __PACKAGE_H_
 class CProp;
class CPackage : public CLevelExp,public CCObject
{
	friend class CProp;
public:
	CPackage(); 
	virtual ~CPackage();
	
	virtual bool init(size_t iMaxPropCount);

	virtual CCObject* copyWithZone(CCZone* pZone)=0;

	M_SYNTHESIZE(size_t, m_iMaxPropCount, iMaxPropCount);

	virtual bool addProp(CProp* pProp);
	virtual bool delProp(CProp* pProp);
	virtual bool moveProp(CProp* pProp, int iIndex);

	int getCurPropCount();

public:
	CCArray m_oArrProp;

};
class CUnitPackage : public CPackage
{
	friend class CUnit;
public:
	CUnitPackage();
	virtual ~CUnitPackage();
	
	virtual bool init(size_t iMaxPropCount);
	M_CREATE_FUNC_PARAM(CUnitPackage,(size_t iMaxPropCount), iMaxPropCount);

	virtual CCObject* copyWithZone(CCZone* pZone);

	virtual bool addProp(CProp* pProp);
	virtual bool delProp(CProp* pProp);
	virtual bool moveProp(CProp* pProp, int iIndex);

protected:
	virtual void setOwner(CUnit* pOwner);

public:
	// 获取道具属主
	virtual CUnit* getOwner() const ;

private:
	CUnit* m_pOwner;
};

class CStorePackage : public CPackage
{

public:
	CStorePackage();
	virtual ~CStorePackage();

	virtual	bool init(size_t iMaxPropCount);

	virtual bool addProp(CProp* pProp);
	virtual bool delProp(CProp* pProp);
	   
};
#endif