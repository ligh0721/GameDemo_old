#include "CommInc.h"

#include "GameDisplay.h"
#include "Skill.h"
#include "Action.h"
#include "GameChapterScene.h"
#include "MainScene.h"
#include "SomeScene.h"
#include "BattleScene.h"
#include "GameLevel.h"
#include "GameCtrl.h"
#include "UnitInfo.h"
#include "GameResControl.h"


bool CCGameChapterScene::init()
{
    CCScene::init();
    m_pGameChapterSceneLayer = CCGameChapterSceneLayer::create();
    addChild(m_pGameChapterSceneLayer);
    return true;
}

bool CCGameChapterScene::initWithRole( int iRole )
{
	CCScene::init();
	m_pGameChapterSceneLayer = CCGameChapterSceneLayer::createWithRole(iRole);
	addChild(m_pGameChapterSceneLayer);

	return true;
}

CCGameChapterSceneLayer::CCGameChapterSceneLayer()
	: m_iRole(kAttack)
{

}

bool CCGameChapterSceneLayer::init()
{
    M_DEF_FC(pFc);
	M_DEF_OU(pOu);
	CGameLevelManager* pLevelManage = CGameLevelManager::sharedGameLevelManager();

    pFc->addSpriteFramesWithFile("game_chapter.plist");
    pFc->addSpriteFramesWithFile("skill.plist");
    CCLayerColor::initWithColor(ccc4(204, 232, 207, 64));
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();

	m_oSp.init(3, 6, 100, 10, 10, NULL);
	CCSkillButtonNormal* pSb;
	unsigned int maxRow = m_oSp.getRow() - 1;
	unsigned int maxLine = m_oSp.getLine() - 1;

	unsigned int iGameLevelSize = pLevelManage->getGameSimpleLeveInfoSize();
    //iGameLevelSize = 18;
	unsigned int rowCount = iGameLevelSize%(maxLine+1) > 0 ? iGameLevelSize/(maxLine+1) + 1 : iGameLevelSize/(maxLine+1);
    //unsigned int rowCount = 3;
	for (unsigned int row = 0; row < rowCount; row++)
	{
		unsigned int lineCount = (row + 1 == rowCount) ?  ((iGameLevelSize % (maxLine + 1)) == 0 ? maxLine + 1 : iGameLevelSize % (maxLine + 1)) : maxLine + 1;
        //unsigned int lineCount = 6;
		for (unsigned int line = 0; line < lineCount; line++)
		{
			CGameSimpleLeveInfo* pLevelInfo = pLevelManage->getGameSimpleLeveInfoByLevel(maxLine * (row) + line + 1);
			CGameLeveMarkInfo* pLeveMarkInfo = pLevelManage->getGameLevelMarkInfoByLevel(maxLine * (row) + line + 1);
			if(pLeveMarkInfo->m_cUnlock == 1)
			{
				
				pSb = CCGameChapterButton::create(
					"map_completed.png", 
					"map_completed.png", 
					"map_completed.png", 
					NULL, 
					NULL, 
					0, 
					this, 
					callfuncN_selector(CCGameChapterSceneLayer::onChapterItemClick), 
					NULL, 
					pLevelInfo->m_iLevel);
			}
			else
			{
				pSb = CCGameChapterButton::create(
					"map_unavailiable.png", 
					"map_unavailiable.png", 
					"map_unavailiable.png", 
					NULL, 
					NULL, 
					0, 
					NULL, 
					NULL, 
					NULL, 
					pLevelInfo->m_iLevel);
			}
			//m_oSp.addButton(pSb, line, rowCount - row - 1);
            m_oSp.pushAddButtonExAction(pSb, CCButtonPanel::kTopToBottom);
		}
	}
    
	addChild(&m_oSp);
	CCLog("charpter panel size width=%d, height=%d" , m_oSp.getSkillMenu()->getContentSize().width, m_oSp.getSkillMenu()->getContentSize().height);
	m_oSp.setPosition(ccp(oSz.width/2 , oSz.height/2));

    //m_oSp.delButton(9);
    //m_oSp.clearUpSlot(CCButtonPanel::kBottomToTop);
    //m_oSp.clearUpSlot(CCButtonPanel::kTopToBottom);
    
    char* szTmp[] = {M_SKILL_PATH("skill1"), M_SKILL_PATH("skill2"), M_SKILL_PATH("skill3"), M_SKILL_PATH("skill4")};
    for (int i = 0; i < 5; ++i)
    {
        m_oSp.pushDelButtonAction(rand() % 12 + 6, CCButtonPanel::kTopToBottom);
        m_oSp.pushAddButtonExAction(CCSkillButtonNormal::create(szTmp[rand() % 4], szTmp[rand() % 4], NULL, NULL, NULL, 0, NULL, NULL, NULL), CCButtonPanel::kTopToBottom);
        
        //m_oSp.pushClearUpSlotAction(CCButtonPanel::kBottomToTop, CCButtonPanel::kRightToLeft);
        //m_oSp.pushClearUpSlotAction(CCButtonPanel::kTopToBottom);
    }

    m_oSp.pushClearUpSlotAction(CCButtonPanel::kBottomToTop, CCButtonPanel::kRightToLeft);
    for (int i = 0; i < 5; ++i)
    {
        m_oSp.pushDelButtonAction(rand() % 12, CCButtonPanel::kBottomToTop, CCButtonPanel::kRightToLeft);
        m_oSp.pushAddButtonExAction(CCSkillButtonNormal::create(szTmp[rand() % 4], szTmp[rand() % 4], NULL, NULL, NULL, 0, NULL, NULL, NULL), CCButtonPanel::kBottomToTop, CCButtonPanel::kRightToLeft);

        //m_oSp.pushClearUpSlotAction(CCButtonPanel::kBottomToTop, CCButtonPanel::kRightToLeft);
        //m_oSp.pushClearUpSlotAction(CCButtonPanel::kTopToBottom);
    }

    
    m_oSp.pushClearUpSlotAction(CCButtonPanel::kBottomToTop, CCButtonPanel::kLeftToRight);
    for (int i = 0; i < 5; ++i)
    {
        m_oSp.pushDelButtonAction(rand() % 12, CCButtonPanel::kBottomToTop, CCButtonPanel::kLeftToRight);
        m_oSp.pushAddButtonExAction(CCSkillButtonNormal::create(szTmp[rand() % 4], szTmp[rand() % 4], NULL, NULL, NULL, 0, NULL, NULL, NULL), CCButtonPanel::kBottomToTop, CCButtonPanel::kLeftToRight);

        //m_oSp.pushClearUpSlotAction(CCButtonPanel::kBottomToTop, CCButtonPanel::kRightToLeft);
        //m_oSp.pushClearUpSlotAction(CCButtonPanel::kTopToBottom);
    }
    
    m_oSp.pushClearUpSlotAction(CCButtonPanel::kTopToBottom, CCButtonPanel::kRightToLeft);
    for (int i = 0; i < 5; ++i)
    {
        m_oSp.pushDelButtonAction(rand() % 12 + 6, CCButtonPanel::kTopToBottom, CCButtonPanel::kRightToLeft);
        m_oSp.pushAddButtonExAction(CCSkillButtonNormal::create(szTmp[rand() % 4], szTmp[rand() % 4], NULL, NULL, NULL, 0, NULL, NULL, NULL), CCButtonPanel::kTopToBottom, CCButtonPanel::kRightToLeft);

        //m_oSp.pushClearUpSlotAction(CCButtonPanel::kBottomToTop, CCButtonPanel::kRightToLeft);
        //m_oSp.pushClearUpSlotAction(CCButtonPanel::kTopToBottom);
    }
    

	m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);

	
	CCMenuItemImage* pBack = CCMenuItemImage::create("button_back.png", "button_back.png", "button_back.png", this, menu_selector(CCGameChapterSceneLayer::onBackClick));
	m_oMenu.addChild(pBack);
	pBack->setPosition(ccp(oSz.width * 0.05 + pBack->getContentSize().width * 0.5, oSz.height * 0.9));

/*	
	for(int i=0; i<12; i++)
	{
		 CCMenuItemImage* pItem = CCMenuItemImage::create ("button01.png", "button01.png", "button01.png", this, (SEL_MenuHandler)&CCGameChapterSceneLayer::onChapterItemClick);
		 m_oMenu.addChild(pItem);
		 pItem->setPosition(ccp(oSz.width * 0.2 + pItem->getContentSize().width * 0.5  * ((i%4)*2) + ((i%4)*30), oSz.height * 0.8 - pItem->getContentSize().width * 0.5 * ((int)(i/4)*2) - (i/4)*30));

	}  
*/
    return true;
}

bool CCGameChapterSceneLayer::initWithRole(int iRole )
{
	m_iRole = iRole;
	return init();
	
}

void CCGameChapterSceneLayer::onShiftLeftClick( CCObject* pObject )
{

}
void CCGameChapterSceneLayer:: onChapterItemClick(CCNode* pNode)
{
	CCGameChapterButton* pButton = dynamic_cast<CCGameChapterButton*>(pNode);
	CGameLevelManager::sharedGameLevelManager()->setCurLevel(pButton->m_iLevel);
	CGameResController::sharedGameResController()->initCurLevelRes();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, CCBattleScene::create()));
}
void CCGameChapterSceneLayer::onShiftRightClick( CCObject* pObject )
{
    
}

void CCGameChapterSceneLayer::onBackClick( CCObject* pObject )
{
	//CCGameManager::sharedGameManager()->popSceneWithTrans(0.5);
   CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, CCMainScene::create()));
}

