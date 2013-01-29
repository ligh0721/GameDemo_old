#ifndef __GOODS_H_
#define __GOODS_H_

/************************************************************************/
/*��Ʒ��*/
/************************************************************************/

class CLevelExp;
class CSkill; 

class CProp : public CLevelExp, public CCObject
{
	friend class CUnit;
	friend class CUnitPackage;
public:
	CProp();
	virtual ~CProp();

	virtual bool init(int iPrice, 
		int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage);

	//M_CREATE_FUNC_PARAM(CAttachedProp,(int iPrice, 
		//int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment), 
		//iPrice, iSellPriceint int iMaxStackCount, int iStackCount, int iFallDownMoment);

	virtual CCObject* copyWithZone(CCZone* pZone)=0;

	void addSkill(CSkill* skill);
	void delSkill(CSkill* skill);

public:
	const int getKey() { return m_iKey; }

protected:
	virtual void setOwner(CUnit* pOwner);

public:
	// ��ȡ��������
	virtual CUnit* getOwner() const ;

	virtual void onPropAdd()=0;
	virtual void onPropDel()=0;
	virtual char* onPropShow()=0;

	M_SYNTHESIZE(int, m_iPrice, Price)
	M_SYNTHESIZE(int, m_iSellPrice, SellPrice)
	M_SYNTHESIZE(int, m_iMaxStackCount, MaxStackCount)
	M_SYNTHESIZE(int, m_iStackCount, StackCount)
	M_SYNTHESIZE(int, m_iFallDownMoment, FallDownMoment)
	M_SYNTHESIZE(int, m_iIndexOfPackage, IndexOfPackage)
private:
	const int m_iKey;                
	CUnit* m_pOwner;

protected:
	CCArray   m_arrSkill;
};

class CActiveProp : public CProp
{
public:
	CActiveProp();
	virtual ~CActiveProp();

	virtual bool init(int iPrice, 
		int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage);

	virtual void onPropAdd();
	virtual void onPropDel();
	virtual char* onPropShow();
	virtual void onUse()=0;
};

class CPassiveProp : public CProp
{
public:
	CPassiveProp();
	virtual ~CPassiveProp();

	virtual bool init(int iPrice, 
		int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage);

	virtual void onPropAdd();
	virtual void onPropDel();
	virtual char* onPropShow();
};

/************************************************************************/
/* ��������Ʒ ���ҩ,ħƿ                                              */
/************************************************************************/
class CConsumeProp : public CActiveProp
{
public:
	CConsumeProp();
	virtual ~CConsumeProp();
	
	virtual bool init(int iPrice, 
		int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage, int iUseUpCount);


	M_CREATE_FUNC_PARAM(CConsumeProp,(int iPrice, 
		int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage, int iUseUpCount), 
	iPrice, iSellPrice ,iMaxStackCount, iStackCount,  iFallDownMoment, iIndexOfPackage, iUseUpCount);

	CCObject* copyWithZone(CCZone* pZone);

	virtual char* onPropShow();
	virtual void onUse();
	int getUseUpCount();
	int getCurUsedCount();

private:
	int m_iCurUsedCount;
	int m_iUseUpCount;
};

/************************************************************************/
/* ���ӵ�λ���Եı�������Ʒ												*/
/************************************************************************/
class CAttributeProp : public CPassiveProp
{
public:
	CAttributeProp();
	virtual ~CAttributeProp();

	virtual bool init(int iPrice, 
		int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage);


	M_CREATE_FUNC_PARAM(CAttributeProp,(int iPrice, 
		int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage), 
		iPrice, iSellPrice ,iMaxStackCount, iStackCount,  iFallDownMoment, iIndexOfPackage);

	CCObject* copyWithZone(CCZone* pZone);
};

/************************************************************************/
/* �����Ե����� ��λ������ �Ӿ���ֵ,��hp,ħ��ֵ ����ռ�õ�λ�����ռ�                       */
/************************************************************************/
class CPermanentAttributeProp : public CAttributeProp
{
public:
	CPermanentAttributeProp();
	virtual ~CPermanentAttributeProp();

	virtual bool init(int iPrice);

	M_CREATE_FUNC_PARAM(CPermanentAttributeProp,(int iPrice), iPrice);

	CCObject* copyWithZone(CCZone* pZone);

	virtual void onPropDel();
};
#endif