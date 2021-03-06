#pragma once

#include "GameDisplay.h"
#include "Skill.h"
#include "MissionInfo.h"

class CCWHomeSceneLayer;
class CSimpleGameUnit;
class CGameUnit;
class CHpChangeBuff;
class CGameMission;

class CCWHomeScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCWHomeScene);

public:
    CCWHomeSceneLayer* m_pHomeSceneLayer;
};

class CCWHomeSceneLayer : public CCWinUnitLayer
{
public:
    struct REVIVE_INFO
    {
        float fReviveTimeLeft;
        CCPoint oPos;
        uint32_t dwLevel;
        uint32_t dwMaxLevel;
        uint32_t dwExp;
        uint32_t dwMaxExp;
        CCArray oArrSkill;
        CCArray oArrSkillBtn;
    };

public:
    CCWHomeSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCWHomeSceneLayer);
    void onBtnStartClick(CCObject* pObject);
    void onBtnCfgClick(CCObject* pObject);
	
    virtual void onTickEvent(float fDt);
    void onTick(float fDt);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void onEnter();
	virtual void onExit();
    CGameUnit* getHeroUnit();
    void onBtnGoClick(CCObject* pObject);
    void onBtnHeroClick(CCObject* pObject);
    void onBtnBuildClick(CCNode* pObject);
    void onGoldChange(CCObject* pObject);

    bool addTower(const CCPoint& roPos);
    void addTowerEnd(CCObject* pObject);

    void onGetBuff(CCNode* pNode);
    void onGetBuffEnd(CCNode* pNode);

    virtual void onUnitDie(CGameUnit* pUnit);

    void onHeroSoulTick(float fDt);
    void onNextRoundTick(float fDt);


public:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    
    CCMenu m_oMenu;
    CCMenuItemImage m_oStart;
    CCMenuItemImage m_oCfg;
    int tickCount;
    //bool heroMove;
	//int touchMoveTimes;
    CCLayerColor m_oGameCtrlLayer;
    CCMenu m_oMenuCtrl;
    CCMenuItemImage m_oGoToTechTree;
    CCMenuItemImage m_oHeroHead;
    CCProgressBar m_oExpBar;
    CCProgressBar m_oHpBar;
    CCButtonPanel m_oSkillPanel;
    //CCSkillButtonAdvance skillButton[4];
    int m_iTouchActionFlag;
    CGameMission *m_pCurMission;

    CCSkillButtonNormal m_oBuildBtn;
    bool m_bCanBuild;
    CCLabelTTF m_oGold;
    int m_iHero;

    CUnitInfoPatchManager m_oUipm;
    int m_iBuffGetting;

    CForceResouce m_oFr;

    REVIVE_INFO m_stReviveInfo;
    CCLabelTTF m_oLeftToRevive;

    float m_fWaitingNextRound;
    int m_iLastNum;
};

