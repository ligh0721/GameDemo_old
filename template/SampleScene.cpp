#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "Unit.h"
#include "Skill.h"
#include "Action.h"
#include "SampleScene.h"


bool CCSampleScene::init()
{
    CCScene::init();
    m_pSampleSceneLayer = CCSampleSceneLayer::create();
    addChild(m_pSampleSceneLayer);

    return true;
}

CCSampleSceneLayer::CCSampleSceneLayer()
{
}

bool CCSampleSceneLayer::init()
{
    CCLayerColor::initWithColor(ccc4(204, 232, 207, 64));
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();

    M_DEF_FC(pFc);

    m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);

    m_oBtnSample.initWithString("Sample", this, menu_selector(CCSampleSceneLayer::onBtnSampleClick));
    m_oMenu.addChild(&m_oBtnSample);
    m_oBtnSample.setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));
    
    return true;
}

void CCSampleSceneLayer::onBtnSampleClick(CCObject* pObject)
{
}
