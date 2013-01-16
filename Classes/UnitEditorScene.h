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
    CCUnitEditorSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCUnitEditorSceneLayer);

    virtual void draw();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    void onBtnUnitEditorClick(CCObject* pObject);
    void onBtnResetWalkerClick(CCObject* pObject);
    void onBtnClearPathClick(CCObject* pObject);
    void onBtnSavePathClick(CCObject* pObject);

public:
    CCMenu m_oMenu;
    CCMenuItemFont m_oBtnUnitEditor;
    CUnitPath m_oPath;
    CPathGameUnit* m_pWalker;
    CCLabelTTF m_oLabel;
};

class CCUnitEditorSceneCtrlLayer : public CCLayer
{
public:
    virtual bool init(CCUnitEditorSceneLayer* pMainLayer);
    CREATE_FUNC_PARAM(CCUnitEditorSceneCtrlLayer, (CCUnitEditorSceneLayer* pMainLayer), pMainLayer);

    void onCheckExit(float fDt);

public:
    CCUnitEditorSceneLayer* m_pMainLayer;
    CCMenu m_oMenu;
    CCMenuItemFont m_oBtnResetWalker;
    CCMenuItemFont m_oBtnClearPath;
    CCMenuItemFont m_oBtnSavePath;
    static bool m_bExit;
    static bool m_bReady;
};

