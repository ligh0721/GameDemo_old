#pragma once

#include "Unit.h"

// 可以用编辑器替换命令将UnitEditor替换为指定的名字

class CCUnitEditorSceneLayer;

class CCUnitEditorScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCUnitEditorScene);

public:
    CCUnitEditorSceneLayer* m_pUnitEditorSceneLayer;
};

class CCUnitEditorSceneLayer : public CCWinUnitLayer
{
public:
    static const int CONST_MAX_PATH = 5;
    static const ccColor3B CONST_ARR_COLOR[CONST_MAX_PATH];
public:
    CCUnitEditorSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCUnitEditorSceneLayer);

    virtual void draw();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    void onBtnUnitEditorClick(CCObject* pObject);
    void onBtnResetWalkerClick(CCObject* pObject);
    void onBtnResetAllWalkerClick(CCObject* pObject);
    void onBtnClearPathClick(CCObject* pObject);
    void onBtnClearAllPathClick(CCObject* pObject);
    void onBtnSavePathClick(CCObject* pObject);
    void onBtnReloadClick(CCObject* pObject);
    CGameUnit* curWalker(bool bReset = false);

public:
    CCMenu m_oMenu;
    CCMenuItemFont m_oBtnUnitEditor;
    CUnitPath m_aoPath[CONST_MAX_PATH];
    int m_aiWalker[CONST_MAX_PATH];
    int m_iCurPathIndex;
    CCLabelTTF m_oLabel;
    CUnitInfoPatchManager m_oUipm;
    bool m_bHostility;
};

class CCUnitEditorSceneCtrlLayer : public CCLayer
{
public:
    virtual bool init(CCUnitEditorSceneLayer* pMainLayer);
    CREATE_FUNC_PARAM(CCUnitEditorSceneCtrlLayer, (CCUnitEditorSceneLayer* pMainLayer), pMainLayer);

    void onCheckExit(float fDt);
    void onBtnPrevPathClick(CCObject* pObject);
    void onBtnNextPathClick(CCObject* pObject);
    void onBtnHostilityClick(CCObject* pObject);
    void onBtnRunClick(CCObject* pObject);
    void updateColor();

public:
    CCUnitEditorSceneLayer* m_pMainLayer;
    CCMenu m_oMenu;
    CCMenuItemFont m_oBtnResetWalker;
    CCMenuItemFont m_oBtnResetAllWalker;
    CCMenuItemFont m_oBtnClearPath;
    CCMenuItemFont m_oBtnClearAllPath;
    CCMenuItemFont m_oBtnSavePath;
    CCMenuItemFont m_oBtnPrevPath;
    CCMenuItemFont m_oBtnNextPath;
    CCMenuItemFont m_oBtnHostility;
    CCMenuItemFont m_oBtnReload;
    CCMenuItemFont m_oBtnRun;
    static bool m_bExit;
    static bool m_bReady;
};

