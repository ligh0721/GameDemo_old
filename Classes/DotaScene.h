#pragma once

// 可以用编辑器替换命令将Dota替换为指定的名字
class CCDotaSceneLayer;
class CSimpleGameUnit;
class CCDotaCtrlLayer;

#include "BattleScene.h"

class CCDotaScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCDotaScene);

public:
    CCDotaSceneLayer* m_pDotaSceneLayer;
};

class CCDotaSceneLayer;

class CCDotaCtrlLayer : public CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(CCDotaCtrlLayer);
    M_SYNTHESIZE(CCDotaSceneLayer*, m_pDotaSceneLayer, DotaSceneLayer);

    void onBtnDotaClick(CCObject* pObject);

public:
    CCMenu m_oMenu;
    CCMenuItemFont m_oBtnDota;
};

class CCDotaSceneLayer : public CCWinUnitLayer
{
public:
	
    CCDotaSceneLayer();
	virtual ~CCDotaSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCDotaSceneLayer);

    void onTick1(float fDt);
    void setSoldierDataByIndex(CSimpleGameUnit* u,int index);
    void onBtnDotaClick(CCObject* pObject);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
};
