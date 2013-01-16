#include "CommInc.h"

#include "GameDisplay.h"
#include "Prop.h"
#include "Package.h"

CPackage::CPackage()
	:m_iMaxPropCount(0)
{

}
CPackage::~CPackage()
{
	m_oArrProp.removeAllObjects();
}
bool CPackage::init(size_t iMaxPropCount)
{
	m_iMaxPropCount = iMaxPropCount;
	return true;
}

bool CPackage::addProp(CProp* pProp)
{
	return true;
}
bool CPackage::delProp(CProp* pProp)
{
	return true;
}
bool CPackage::moveProp(CProp* pProp, int iIndex)
{
	return true;
}

CUnitPackage::CUnitPackage()
{

}
CUnitPackage::~CUnitPackage()
{

}

bool CUnitPackage::init(size_t iMaxPropCount)
{
	return CPackage::init(iMaxPropCount);
}

bool CUnitPackage::addProp(CProp* pProp)
{
	if(m_oArrProp.count() < m_iMaxPropCount)
	{
		size_t i=0;
		for(; i<m_oArrProp.count(); i++)
		{
			CProp* pLocalProp = dynamic_cast<CProp*>(m_oArrProp.objectAtIndex(i));
			CCAssert(pLocalProp, "pProp is null");
			if (pLocalProp->getKey() == pProp->getKey())
			{
				if (pLocalProp->getMaxStackCount() > pLocalProp->getStackCount())
				{
					pLocalProp->setStackCount(pLocalProp->getStackCount() + 1);
				}
				else
				{
					
					m_oArrProp.addObject(pProp);	
					
				}
			}
			
			m_oArrProp.addObject(pProp);
		


		}
		if (i >= m_oArrProp.count())
		{
			m_oArrProp.addObject(pProp);

		}
		pProp->setOwner(getOwner());
		pProp->onPropAdd();
		return true;

	}
	else
	{
		return false;
	}
}
bool CUnitPackage::delProp(CProp* pProp)
{
	for(size_t i=0; i<m_oArrProp.count(); i++)
	{
		CProp* pLocalProp = dynamic_cast<CProp*>(m_oArrProp.objectAtIndex(i));
		CCAssert(pLocalProp, "pProp is null");
		if (pLocalProp->getKey() == pProp->getKey())
		{
			if (pLocalProp->getStackCount() - 1 > 0)
			{
				pLocalProp->setStackCount(pLocalProp->getStackCount() - 1);
			}
			else
			{
				m_oArrProp.removeObject(pProp);
			}
			return true;
		}
		pProp->onPropDel();
		return false;
	}
	return false;
}
bool CUnitPackage::moveProp(CProp* pProp, int iIndex)
{
	return  true;
}

CCObject* CUnitPackage::copyWithZone(CCZone* pZone)
{
	return CUnitPackage::create(m_iMaxPropCount);
}

void CUnitPackage::setOwner(CUnit* pOwner)
{
	m_pOwner = pOwner;
}

CUnit* CUnitPackage::getOwner() const
{
	return m_pOwner;
}