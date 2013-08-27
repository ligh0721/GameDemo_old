#pragma once

#include "GameDisplay.h"
#include "Skill.h"
#include "MissionInfo.h"

class CCWHomeSceneLayer;
class CSimpleGameUnit;
class CGameUnit;
class CHpChangeBuff;
class CGameMission;

class CCWHomeScene : public CCScene, public CAsyncLoadingInterface
{
public:
    virtual bool init();
    CREATE_FUNC(CCWHomeScene);

    virtual void onLoading(int iStage);
    inline virtual void onLoadingEnd();

public:
    CCWHomeSceneLayer* m_pHomeSceneLayer;

protected:
    CCLoadingLayer* m_pLl;
    CCProgressBar* m_pPb;
    CCLabelTTF* m_pLb;
};

class CCWHomeSceneLayer : public CCWinUnitLayer, public CAsyncLoadingInterface
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

    enum TARGET_INFO_UPDATE_BIT
    {
        kLevel = 0,
        kHp = 1,
        kAtk = 2,
        kDef = 3
    };

    struct TARGET_INFO
    {
        uint32_t dwLevel;
        uint32_t dwHp;
        uint32_t dwMaxHp;
        uint32_t dwAtk0;
        uint32_t dwAtk1;
        uint32_t dwAtkEx;
        uint32_t dwDef;

        TARGET_INFO() { memset(this, 0, sizeof(TARGET_INFO)); }
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
    virtual void onUnitHpChange(CGameUnit* pUnit, float fChanged);

    void onHeroSoulTick(float fDt);
    void onNextRoundTick(float fDt);

    void hideTargetInfo();
    void updateTargetInfo(CGameUnit* pUnit);

    virtual void onLoading(int iStage);
    inline virtual void onLoadingEnd();


public:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         

    CCMenu m_oMenu;
    CCMenuItemImage m_oStart;
    CCMenuItemImage m_oCfg;
    int tickCount;
    //bool heroMove;
    //int touchMoveTimes;
    CCLayerColor m_oGameCtrlLayer;

    CCLayerColor m_oTargetInfoLayer;
    CCLabelTTF m_oTargetLv;
    CCLabelTTF m_oTargetHp;
    CCLabelTTF m_oTargetAtk;
    CCLabelTTF m_oTargetAtkEx;
    CCLabelTTF m_oTargetDef;
    CGameUnit* m_pLastTarget;
    TARGET_INFO m_stTargetInfo;

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
    bool m_bMissionEnd;
};

