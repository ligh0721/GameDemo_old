#include "CommInc.h"

#include "GameDisplay.h"
#include "Skill.h"
#include "PropUI.h"
#include "Package.h"

bool CCPropUpdateScene::init()
{
	m_pPropUpdateLayer = CCPropUpdateLayer::create();
	addChild(m_pPropUpdateLayer);

	return true;
}


bool CCPropUpdateLayer::init()
{
	CCSize oVis = CCDirector::sharedDirector()->getVisibleSize();

	m_pPropPanel = CCButtonPanel::create(5, 1, 64, 7, 5, "Skill5_78x352_7_5_black.png");
	addChild(m_pPropPanel);
	CCSize oSz = m_pPropPanel->getContentSize();
	m_pPropPanel->setPosition(ccp(oVis.width - oSz.width / 2, oVis.height - oSz.height / 2));
	
	m_pHpLable = CCLabelAtlas::create("645", "fonts/tuffy_bold_italic-charmap.plist");
	addChild(m_pHpLable);
	m_pHpLable->setPosition(ccp(10, oVis.height/3));
	m_pHpLable->setOpacity( 32 );

	//加载物品
    /*
	CGameManager* pGm = CGameManager::sharedGameManager();
	CCTankSprite* pTank = pGm->getTankByKey(pGm->m_iPlayerKey);
	CCAssert(pTank, "pTank is must be not null");
	CUnit* pGameUnit = dynamic_cast<CUnit*>(pTank->getLogicBody());
	CCAssert(pGameUnit, "pGameUnit is must be not null");
	CCAssert(pGameUnit->m_pUnitPackage, "pakcage is must be not null");
	
	//按物品在背包中的顺序加载在物品面板上
	for(size_t i=0; i<pGameUnit->m_pUnitPackage->m_oArrProp.count(); i++)
	{
		CProp* pProp = (CProp*)pGameUnit->m_pUnitPackage->m_oArrProp.objectAtIndex(i);
		CCMenuItemImage *pPropImg = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseNormal.png",
			this,
			NULL);
		
		         
	}
    */
	return true;
}
