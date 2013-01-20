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

const ccColor3B CCUnitEditorSceneLayer::CONST_ARR_COLOR[CONST_MAX_PATH] = {ccRED, ccGREEN, ccYELLOW, ccBLUE, ccGRAY};

CCUnitEditorSceneLayer::CCUnitEditorSceneLayer()
{
}

bool CCUnitEditorSceneLayer::init()
{
    CCWinUnitLayer::initWithColor(ccc4(204, 232, 207, 64));
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    M_DEF_GM(pGm);

    M_DEF_FC(pFc);
    pFc->addSpriteFramesWithFile("UI.plist");

    m_bHostility = false;

    m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);

    m_oBtnUnitEditor.initWithString("UnitEditor", this, menu_selector(CCUnitEditorSceneLayer::onBtnUnitEditorClick));
    m_oMenu.addChild(&m_oBtnUnitEditor);
    m_oBtnUnitEditor.setPosition(ccp(oSz.width * 0.5, oSz.height * 0.2));

    CCSprite* pSprite = CCSprite::create("level00/LevelHD.png");
    //pSprite->setOpacity(0x7F);
    setBackGroundSprite(pSprite);
    reorderChild(pSprite, -10);
    setBufferEffectParam(0.9, 0, 0.1);

    m_oLabel.initWithString("(x, y)", "", 12);
    addChild(&m_oLabel, 20);
    m_oLabel.setColor(CONST_ARR_COLOR[0]);
    m_oLabel.setVisible(false);

    for (int i = 0; i < CONST_MAX_PATH; ++i)
    {
        char sz[256];
        sprintf(sz, "level00/LevelHD%02d.pth", i);
        m_aoPath[i].initWithFile(sz);
    }
    //ccDrawColor4B(0xFF, 0x00, 0x00, 255);
    glLineWidth(3);
    glPointSize(10);

    m_oUipm.initWithFile("level00/Level.uip");
    M_DEF_UM(pUm);
    for (m_iCurPathIndex = 0; m_iCurPathIndex < CONST_MAX_PATH; ++m_iCurPathIndex)
    {
        m_aiWalker[m_iCurPathIndex] = 0;
        if (!m_aoPath[m_iCurPathIndex].m_vecPoints.empty())
        {
            CPathGameUnit* pWalker = curWalker();
        }
    }

    m_iCurPathIndex = 0;

    return true;
}

void CCUnitEditorSceneLayer::draw()
{
    for (int i = 0; i < CONST_MAX_PATH; ++i)
    {
        const ccColor3B& c = CONST_ARR_COLOR[i];
        ccDrawColor4B(c.r, c.g, c.b, 0xFF);
        int n = m_aoPath[i].m_vecPoints.size();
        CCPoint* pPs = m_aoPath[i].m_vecPoints.data();
        ccDrawPoly(pPs, n, false);
        ccDrawPoints(pPs, n);
    }
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
    M_DEF_GM(pGm);
    CCPoint oPos = ccpSub(pTouch->getLocation(), getPosition());
    m_oLabel.setVisible(false);
    if (isClickAction())
    {
        CUnitPath::VEC_POINTS& vec = m_aoPath[m_iCurPathIndex].m_vecPoints;
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

        CPathGameUnit* pWalker = curWalker();
        if (n == vec.size())
        {
            m_aoPath[m_iCurPathIndex].addPoint(oPos);
            if (!pWalker->getSprite()->isVisible())
            {
                pWalker->setPosition(oPos);
                pWalker->getSprite()->setVisible(true);
                pWalker->moveAlongPath(&m_aoPath[m_iCurPathIndex], true, m_bHostility, 5);
            }
            else
            {
                pWalker->moveAlongPath(&m_aoPath[m_iCurPathIndex], false, m_bHostility, 5);
            }
        }
        else
        {
            if (pWalker->getCurPos() > vec.size())
            {
                pWalker->setCurPos(vec.size());
            }

            if (!vec.size())
            {
                pWalker->getSprite()->setVisible(false);
                pWalker->setHp(0);
                //pWalker->moveAlongPath(NULL);
            }
        }
    }
    
}

void CCUnitEditorSceneLayer::onBtnResetWalkerClick( CCObject* pObject )
{
    M_DEF_GM(pGm);
    CPathGameUnit* pWalker = curWalker();
    if (!m_aoPath[m_iCurPathIndex].m_vecPoints.empty())
    {
        pWalker->setPosition(*m_aoPath[m_iCurPathIndex].m_vecPoints.begin());
    }
    pWalker->moveAlongPath(&m_aoPath[m_iCurPathIndex], true, m_bHostility, 5);
}

void CCUnitEditorSceneLayer::onBtnClearPathClick( CCObject* pObject )
{
    M_DEF_GM(pGm);
    CPathGameUnit* pWalker = curWalker();
    m_aoPath[m_iCurPathIndex].m_vecPoints.clear();
    pWalker->getSprite()->setVisible(false);
    pWalker->setHp(0);
    //pWalker->moveAlongPath(NULL);
}

void CCUnitEditorSceneLayer::onBtnSavePathClick( CCObject* pObject )
{
    // TODO: save m_oPath
    for (int i = 0; i < CONST_MAX_PATH; ++i)
    {
        char sz[256];
        sprintf(sz, "level00/LevelHD%02d.pth", i);
        m_aoPath[i].saveAsFile(sz);
    }
}

CPathGameUnit* CCUnitEditorSceneLayer::curWalker(bool bReset)
{
    M_DEF_GM(pGm);
    CPathGameUnit* pWalker = dynamic_cast<CPathGameUnit*>(pGm->getUnitByKey(m_aiWalker[m_iCurPathIndex]));
    if (pWalker && !pWalker->isDead())
    {
        pWalker->setHp(0);
        pWalker->getSprite()->setVisible(false);
        pWalker = NULL;
    }
    if (!pWalker || pWalker->isDead())
    {
        pWalker = m_oUipm.pathUnitByIndex(m_iCurPathIndex);
        m_aiWalker[m_iCurPathIndex] = pWalker->getKey();
        addUnit(pWalker);
        pWalker->addSkill(CStatusShowPas::create());
        pWalker->setForceByIndex(m_iCurPathIndex);
        pWalker->setAlly(pWalker->getForce());
        if (!m_aoPath[m_iCurPathIndex].m_vecPoints.empty())
        {
            pWalker->setPosition(*m_aoPath[m_iCurPathIndex].m_vecPoints.begin());
            pWalker->moveAlongPath(&m_aoPath[m_iCurPathIndex], true, m_bHostility, 5);
        }
        else
        {
            pWalker->getSprite()->setVisible(false);
        }
    }
    return pWalker;
}

void CCUnitEditorSceneLayer::onBtnResetAllWalkerClick( CCObject* pObject )
{
    m_oUipm.initWithFile("level00/Level.uip");
    int t = m_iCurPathIndex;
    for (m_iCurPathIndex = 0; m_iCurPathIndex < CONST_MAX_PATH; ++m_iCurPathIndex)
    {
        onBtnResetWalkerClick(pObject);
    }
    m_iCurPathIndex = t;
}

void CCUnitEditorSceneLayer::onBtnClearAllPathClick( CCObject* pObject )
{
    int t = m_iCurPathIndex;
    for (m_iCurPathIndex = 0; m_iCurPathIndex < CONST_MAX_PATH; ++m_iCurPathIndex)
    {
        onBtnClearPathClick(NULL);
    }
    m_iCurPathIndex = t;
}

void CCUnitEditorSceneLayer::onBtnReloadClick( CCObject* pObject )
{
    for (int i = 0; i < CONST_MAX_PATH; ++i)
    {
        char sz[256];
        sprintf(sz, "level00/LevelHD%02d.pth", i);
        m_aoPath[i].initWithFile(sz);
    }
    onBtnResetAllWalkerClick(NULL);
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
    
    m_oBtnResetWalker.initWithString("Reset", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnResetWalkerClick));
    m_oBtnResetWalker.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[pMainLayer->m_iCurPathIndex]);
    m_oMenu.addChild(&m_oBtnResetWalker);
    m_oBtnResetWalker.setPosition(ccp(oSz.width * 0.12, oSz.height * 0.95));

    m_oBtnResetAllWalker.initWithString("ResetAll", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnResetAllWalkerClick));
    m_oBtnResetAllWalker.setColor(ccBLACK);
    m_oMenu.addChild(&m_oBtnResetAllWalker);
    m_oBtnResetAllWalker.setPosition(ccp(oSz.width * 0.12, oSz.height * 0.85));
    
    m_oBtnClearPath.initWithString("Clear", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnClearPathClick));
    m_oBtnClearPath.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[pMainLayer->m_iCurPathIndex]);
    m_oMenu.addChild(&m_oBtnClearPath);
    m_oBtnClearPath.setPosition(ccp(oSz.width * 0.325, oSz.height * 0.95));

    m_oBtnClearAllPath.initWithString("ClearAll", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnClearAllPathClick));
    m_oBtnClearAllPath.setColor(ccBLACK);
    m_oMenu.addChild(&m_oBtnClearAllPath);
    m_oBtnClearAllPath.setPosition(ccp(oSz.width * 0.325, oSz.height * 0.85));

    m_oBtnPrevPath.initWithString("Prev", this, menu_selector(CCUnitEditorSceneCtrlLayer::onBtnPrevPathClick));
    m_oBtnPrevPath.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[(CCUnitEditorSceneLayer::CONST_MAX_PATH - 1) % CCUnitEditorSceneLayer::CONST_MAX_PATH]);
    m_oMenu.addChild(&m_oBtnPrevPath);
    m_oBtnPrevPath.setPosition(ccp(oSz.width * 0.50, oSz.height * 0.95));

    m_oBtnNextPath.initWithString("Next", this, menu_selector(CCUnitEditorSceneCtrlLayer::onBtnNextPathClick));
    m_oBtnNextPath.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[1 % CCUnitEditorSceneLayer::CONST_MAX_PATH]);
    m_oMenu.addChild(&m_oBtnNextPath);
    m_oBtnNextPath.setPosition(ccp(oSz.width * 0.60, oSz.height * 0.95));

    m_oBtnHostility.initWithString("Peaceful", this, menu_selector(CCUnitEditorSceneCtrlLayer::onBtnHostilityClick));
    m_oBtnHostility.setColor(ccBLACK);
    m_oMenu.addChild(&m_oBtnHostility);
    m_oBtnHostility.setPosition(ccp(oSz.width * 0.55, oSz.height * 0.85));

    m_oBtnReload.initWithString("Reload", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnReloadClick));
    m_oBtnReload.setColor(ccBLACK);
    m_oMenu.addChild(&m_oBtnReload);
    m_oBtnReload.setPosition(ccp(oSz.width * 0.85, oSz.height * 0.95));
    
    m_oBtnSavePath.initWithString("SavePaths", m_pMainLayer, menu_selector(CCUnitEditorSceneLayer::onBtnSavePathClick));
    m_oBtnSavePath.setColor(ccBLACK);
    m_oMenu.addChild(&m_oBtnSavePath);
    m_oBtnSavePath.setPosition(ccp(oSz.width * 0.85, oSz.height * 0.85));

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

void CCUnitEditorSceneCtrlLayer::onBtnPrevPathClick( CCObject* pObject )
{
    m_pMainLayer->m_iCurPathIndex = (m_pMainLayer->m_iCurPathIndex + CCUnitEditorSceneLayer::CONST_MAX_PATH - 1) % CCUnitEditorSceneLayer::CONST_MAX_PATH;
    updateColor();
}

void CCUnitEditorSceneCtrlLayer::onBtnNextPathClick( CCObject* pObject )
{
    m_pMainLayer->m_iCurPathIndex = (m_pMainLayer->m_iCurPathIndex + 1) % CCUnitEditorSceneLayer::CONST_MAX_PATH;
    updateColor();
}

void CCUnitEditorSceneCtrlLayer::onBtnHostilityClick( CCObject* pObject )
{
    m_pMainLayer->m_bHostility = !m_pMainLayer->m_bHostility;
    m_oBtnHostility.setString(m_pMainLayer->m_bHostility ? "Hostile" : "Peaceful");
    m_pMainLayer->onBtnResetAllWalkerClick(NULL);
}

void CCUnitEditorSceneCtrlLayer::updateColor()
{
    m_pMainLayer->m_oLabel.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[m_pMainLayer->m_iCurPathIndex]);
    m_oBtnResetWalker.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[m_pMainLayer->m_iCurPathIndex]);
    m_oBtnClearPath.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[m_pMainLayer->m_iCurPathIndex]);
    //m_oBtnSavePath.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[m_pMainLayer->m_iCurPathIndex]);
    m_oBtnPrevPath.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[(m_pMainLayer->m_iCurPathIndex + CCUnitEditorSceneLayer::CONST_MAX_PATH - 1) % CCUnitEditorSceneLayer::CONST_MAX_PATH]);
    m_oBtnNextPath.setColor(CCUnitEditorSceneLayer::CONST_ARR_COLOR[(m_pMainLayer->m_iCurPathIndex + 1) % CCUnitEditorSceneLayer::CONST_MAX_PATH]);
}
