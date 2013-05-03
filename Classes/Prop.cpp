#include "CommInc.h"

#include "GameDisplay.h"
#include "Prop.h"
#include "Unit.h"
#include "Skill.h"


CProp::CProp()
	: m_iKey(CGameManager::sharedGameManager()->keygen())
	, m_pOwner(NULL)
	, m_iPrice(0)
	, m_iSellPrice(0)
	, m_iMaxStackCount(0)
	, m_iStackCount(0)
	, m_iFallDownMoment(0)
	, m_iIndexOfPackage(0){}

CProp::~CProp()
{

}

bool CProp::init(int iPrice, int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage)
{
	m_iPrice = iPrice;
	m_iSellPrice = iSellPrice;
	m_iMaxStackCount = iMaxStackCount;
	m_iStackCount = iStackCount;
	m_iFallDownMoment = iFallDownMoment;
	return true;
}


void CProp::addSkill(CSkill* skill)
{
	m_arrSkill.addObject(skill);
}

void CProp::delSkill(CSkill* skill)
{
	m_arrSkill.removeObject(skill);

}

void CProp::setOwner(CUnit* pOwner)
{
	m_pOwner = pOwner;
}

CUnit* CProp::getOwner()const
{
	return m_pOwner;
}

CConsumeProp::CConsumeProp()
	:m_iUseUpCount(0)
	,m_iCurUsedCount(0)

{

}
CConsumeProp::~CConsumeProp()
{

}

bool CConsumeProp::init(int iPrice, int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage, int iUseUpCount)
{
	CProp::init(iPrice, iSellPrice, iMaxStackCount, iStackCount, iFallDownMoment, iIndexOfPackage);
	m_iUseUpCount = m_iUseUpCount;
	return true;
}

CCObject* CConsumeProp::copyWithZone(CCZone* pZone)
{
	return create(m_iPrice, m_iSellPrice, m_iMaxStackCount, m_iStackCount, m_iFallDownMoment, m_iIndexOfPackage, m_iUseUpCount);

}

void CConsumeProp::onUse()
{
	for (size_t i=0; i<m_arrSkill.count(); i++)
	{	
		CActiveSkill *pSkill = dynamic_cast<CActiveSkill*>(m_arrSkill.objectAtIndex(i));
		pSkill->setOwner(getOwner());
		pSkill->cast();
	}
	if(++m_iCurUsedCount >= m_iUseUpCount)
	{
		m_iCurUsedCount = 0;
		m_iUseUpCount--;
	}
	else
	{
		m_iCurUsedCount++;
	}

}

char* CConsumeProp::onPropShow()
{
	return "²¹³ä100µãÑªÁ¿";
}


CAttributeProp::CAttributeProp()
{

}
CAttributeProp::~CAttributeProp()
{

}

bool CAttributeProp::init(int iPrice, int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage)
{
	CPassiveProp::init(iPrice, iSellPrice, iMaxStackCount, iStackCount, iFallDownMoment, iIndexOfPackage);
	return true;
}

CCObject* CAttributeProp::copyWithZone(CCZone* pZone)
{
	return create(m_iPrice, m_iSellPrice, m_iMaxStackCount, m_iStackCount, m_iFallDownMoment, m_iIndexOfPackage);

}

CActiveProp::CActiveProp()
{

}

CActiveProp::~CActiveProp()
{

}

bool CActiveProp::init( int iPrice, int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage)
{
	return CProp::init(iPrice,iSellPrice, iMaxStackCount, iStackCount, iFallDownMoment, iIndexOfPackage);
}

void CActiveProp::onPropAdd()
{

}

void CActiveProp::onPropDel()
{

}

void CActiveProp::onUse()
{

}

char* CActiveProp::onPropShow()
{
	return "";
}

CPassiveProp::CPassiveProp()
{

}

CPassiveProp::~CPassiveProp()
{

}

void CPassiveProp::onPropAdd()
{
	CCAssert(getOwner(), "GameUnit is must not null!");
	for (size_t i=0; i<m_arrSkill.count(); i++)
	{	
		getOwner()->addSkill(dynamic_cast<CSkill*>(m_arrSkill.objectAtIndex(i)));
	}
}

void CPassiveProp::onPropDel()
{
	CCAssert(getOwner(), "GameUnit is must not null!");
	for (size_t i=0; i<m_arrSkill.count(); i++)
	{	
		getOwner()->delSkill(dynamic_cast<CSkill*>(m_arrSkill.objectAtIndex(i)));
	}
}

bool CPassiveProp::init( int iPrice, int iSellPrice, int iMaxStackCount, int iStackCount, int iFallDownMoment, int iIndexOfPackage)
{
	return CProp::init(iPrice, iSellPrice, iMaxStackCount, iStackCount, iFallDownMoment, iIndexOfPackage);
}

char* CPassiveProp::onPropShow()
{
	return "";
}


CPermanentAttributeProp::CPermanentAttributeProp()
{

}

CPermanentAttributeProp::~CPermanentAttributeProp()
{

}

bool CPermanentAttributeProp::init( int iPrice )
{
	return CAttributeProp::init(iPrice,0, 0, 0, 0, -1);
}


CCObject* CPermanentAttributeProp::copyWithZone( CCZone* pZone )
{
	return create(m_iPrice);
}

void CPermanentAttributeProp::onPropDel()
{

}
