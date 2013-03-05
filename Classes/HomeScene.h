#pragma once

#include "GameDisplay.h"

class CCHomeSceneLayer;
class CSimpleGameUnit;

class CCHomeScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCHomeScene);

public:
    CCHomeSceneLayer* m_pHomeSceneLayer;
};

class CCHomeSceneLayer : public CCWinUnitLayer
{
public:
    CCHomeSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCHomeSceneLayer);
    void onBtnStartClick(CCNode* pNode);
    void onBtnUpgradesClick(CCNode* pNode);
    void onBtnAchievementsClick(CCNode* pNode);
    void onBtnStartClick(CCObject* pObject);
    void onBtnCfgClick(CCObject* pObject);
    void onTick(float fDt);
	void setSoldierDataByIndex(CSimpleGameUnit* u,int index);

    static void updateHeroMaxExp(CLevelExp* pLevel);

public:
    CCMenu m_oMenu;
    CCMenuItemImage m_oStart;
    CCMenuItemImage m_oCfg;
    //MissionInfo* infoExample;
    int tickCount;
};
