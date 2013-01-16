#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "Unit.h"
#include "Skill.h"
#include "Action.h"
#include "UnitEditorScene.h"
#include "UnitInfo.h"
#include "SkillInfo.h"
#include "GameCtrl.h"


bool CCUnitEditorScene::init()
{
    CCScene::init();
    
    m_pUnitEditorSceneLayer = CCUnitEditorSceneLayer::create();
    addChild(m_pUnitEditorSceneLayer);

    CCUnitEditorSceneCtrlLayer* pCtrlLayer = CCUnitEditorSceneCtrlLayer::create(m_pUnitEditorSceneLayer);
    addChild(pCtrlLayer, 10);

    CCUnitEditorSceneCtrlLayer::m_bReady = true;

    return true;
}

CCUnitEditorSceneLayer::CCUnitEditorSceneLayer()
{
}

bool CCUnitEditorSceneLayer::init()
{
    CCWinUnitLayer::initWithColor(ccc4(204, 232, 207, 64));
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();

    M_DEF_FC(pFc);

    m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);

    m_oBtnUnitEditor.initWithString("UnitEditor", this, menu_selector(CCUnitEditorSceneLayer::onBtnUnitEditorClick));
    m_oMenu.addChild(&m_oBtnUnitEditor);
    m_oBtnUnitEditor.setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));

    CCSprite* pSprite = CCSprite::create("LevelDemo2HD.png");
    //pSprite->setOpacity(0x7F);
    setBackGroundSprite(pSprite);
    reorderChild(pSprite, -10);
    setBufferEffectParam(0.9, 0, 0.1);

    m_oPath.init();
    ccDrawColor4B(0xFF, 0x00, 0x00, 255);
    glLineWidth(3);
    glPointSize(10);

    g_oOrgUnitInfo.init();
    g_oOrgSkillInfo.init();
    M_DEF_UM(pUm);
    m_pWalker = pUm->pathUnitByInfo(COrgUnitInfo::kMalik);
    addUnit(m_pWalker);
    m_pWalker->getSprite()->setVisible(false);

    m_oLabel.initWithString("(x, y)", "", 12);
    addChild(&m_oLabel, 20);
    m_oLabel.setColor(ccRED);
    m_oLabel.setVisible(false);

    return true;
}

void CCUnitEditorSceneLayer::draw()
{
    int n = m_oPath.m_vecPoints.size();
    CCPoint* pPs = m_oPath.m_vecPoints.data();
    ccDrawPoly(pPs, n, false);
    ccDrawPoints(pPs, n);
}

void CCUnitEditorSceneLayer::onBtnUnitEditorClick(CCObject* pObject)
{
}

bool CCUnitEditorSceneLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
    CCPoint oPos = ccpSub(pTouch->getLocation(), getPosition());
    char sz[256];
    sprintf(sz, "(%.1f, %.1f)", oPos.x, oPos.y);
    m_oLabel.setString(sz);
    m_oLabel.setPosition(oPos);
    m_oLabel.setVisible(true);
    return CCWinUnitLayer::ccTouchBegan(pTouch, pEvent);
}

void CCUnitEditorSceneLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
    CCWinUnitLayer::ccTouchMoved(pTouch, pEvent);
}

void CCUnitEditorSceneLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
    CCWinUnitLayer::ccTouchEnded(pTouch, pEvent);
    CCPoint oPos = ccpSub(pTouch->getLocation(), getPosition());
    m_oLabel.setVisible(false);
    if (isClickAction())
    {
        CUnitPath::VEC_POINTS& vec = m_oPath.m_vecPoints;
        int n = vec.size();
        for (CUnitPath::VEC_POINTS::iterator it = vec.begin(); it != vec.end();)
        {
            if (ccpDistance(*it, oPos) <= 5)
            {
                it = vec.erase(it);
            }
            else
            {
                ++it;
            }
        }

        if (n == vec.size())
        {
            m_oPath.addPoint(oPos);
            if (!m_pWalker->getSprite()->isVisible())
            {
                m_pWalker->setPosition(oPos);
                m_pWalker->getSprite()->setVisible(true);
                m_pWalker->moveAlongPath(&m_oPath, true, false, 5);
            }
            else
            {
                m_pWalker->moveAlongPath(&m_oPath, false, false, 5);
            }
        }
        else
        {
            if (m_pWalker->getCurPos() > vec.size())
            {
                m_pWalker->setCurPos(vec.size());
            }

            if (!vec.size())
            {
                m_pWalker->getSprite()->setVisible(false);
                m_pWalker->moveAlongPath(NULL);
            }
        }
    }
    
}

void CCUnitEditorSceneLayer::onBtnResetWalkerClick( CCObject* pObject )
{
    if (m_oPath.m_vecPoints.size())
    {
        m_pWalker->setPosition(*m_oPath.m_vecPoints.begin());
    }
    m_pWalker->moveAlongPath(&m_oPath, true, false, 5);
}

void CCUnitEditorSceneLayer::onBtnClearPathClick( CCObject* pObject )
{
    m_oPath.m_vecPoints.clear();
    m_pWalker->getSprite()->setVisible(false);
    m_pWalker->moveAlongPath(NULL);
}

void CCUnitEditorSceneLayer::onBtnSavePathClick( CCObject* pObject )
{
    // TODO: save m_oPath
    m_oPath.saveAsFile("LevelDemo2HD.pth");
}

bool CCUnitEditorSceneCtrlLayer::m_bExit = false;
bool CCUnitEditorSceneCtrlLayer::m_bReady = false;

bool CCUnitEditorSceneCtrlLayer::init(CCUnitEditorSceneLayer* pMainLayer)
{
    CCLayer::init();

    m_pMainLayer = pMainLayer;

    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    
    m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);
    
    m_oBtnResetWalker.initWithString("ResetWalker", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnResetWalkerClick));
    m_oBtnResetWalker.setColor(ccRED);
    m_oMenu.addChild(&m_oBtnResetWalker);
    m_oBtnResetWalker.setPosition(ccp(oSz.width * 0.12, oSz.height * 0.95));
    
    m_oBtnClearPath.initWithString("ClearPath", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnClearPathClick));
    m_oBtnClearPath.setColor(ccRED);
    m_oMenu.addChild(&m_oBtnClearPath);
    m_oBtnClearPath.setPosition(ccp(oSz.width * 0.40, oSz.height * 0.95));

    m_oBtnSavePath.initWithString("SavePath", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnSavePathClick));
    m_oBtnSavePath.setColor(ccRED);
    m_oMenu.addChild(&m_oBtnSavePath);
    m_oBtnSavePath.setPosition(ccp(oSz.width * 0.65, oSz.height * 0.95));

    schedule(schedule_selector(CCUnitEditorSceneCtrlLayer::onCheckExit));

    return true;
}

void CCUnitEditorSceneCtrlLayer::onCheckExit(float fDt)
{
    if (m_bExit)
    {
        CCDirector::sharedDirector()->end();
    }
}

