#pragma once

#include "GameDisplay.h"

class CCMainSceneLayer;

class CCMainScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCMainScene);

public:
    CCMainSceneLayer* m_pMainSceneLayer;
};

class CCMainSceneLayer : public CCLayerColor
{
public:
    CCMainSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCMainSceneLayer);
    void onBtnHomeClick(CCObject* pObject);
    void onBtnAttackClick(CCObject* pObject);
    void onBtnDefenseClick(CCObject* pObject);
    void onBtnResearchClick(CCObject* pObject);
    void onBtnTroopsClick(CCObject* pObject);
    void onBtnAchievementClick(CCObject* pObject);
    void onBtnDemoClick(CCObject* pObject);
	void onBtnBattleClick(CCObject* pObject);

    void onActEnd(CCNode* pNode);
public:
    CCMenu m_oMenu;

    CCMenuItemFont m_oHome;
    CCMenuItemFont m_oAttack;
    CCMenuItemFont m_oDefense;
    CCMenuItemFont m_oResearch;
    CCMenuItemFont m_oTroops;
    CCMenuItemFont m_oAchievement;
    CCMenuItemFont m_oDemo;
	CCMenuItemFont m_oBattle;
    
};
