#pragma once


#include "PlayerInfo.h"

class CGameManager : public CCObject
{
public:
    enum CMD_RECV
    {
        kCmdNoting,
        kCmdRun,
        kCmdEdit
    };

public:
    CGameManager();
    virtual bool init();
    M_CREATE_FUNC_PARAM(CGameManager, ());
    static CGameManager* sharedGameManager();

    const CCSpriteFrameCache* Fc() const;
    const CCAnimationCache* Ac() const;
    const CCDirector* Dr() const;
    void pushScene(CCScene* pScene);
    void popScene();

    void cmdRecv(float fDt);

    M_SYNTHESIZE(float, m_fScaleY, ScaleY);
    CCSpriteFrame* getUnitFrame(const char* pUnit, const char* pFrame);
    CCAnimation* getUnitAnimation(const char* pUnit, const char* pAnimation);
    CCAnimation* loadUnitAnimation(const char* pUnit, const char* pAnimation);

    inline float getDistance(const CCPoint& roPos1, const CCPoint& roPos2) { float fDeltaX = roPos1.x - roPos2.x; float fDeltaY = (roPos1.y - roPos2.y) / getScaleY(); return sqrtf(fDeltaX * fDeltaX + fDeltaY * fDeltaY); }

    static int keygen();

    void addUnitToEventDriver(CGameUnit* pUnit);
    CUnitGroup* getUnits();
    CGameUnit* getUnitByKey(int iKey);

    void addProjectileToEventDriver(CProjectile* pProjectile);
    CUnitGroup* getProjectiles();
    CProjectile* getProjectileByKey(int iKey);

    void moveUnitToDustbin(CGameUnit* pToDel);
    void moveProjectileToDustbin(CProjectile* pToDel);
    CCArray* getUnitDustbin();
    CCArray* getProjectileDustbin();

    void setVoice(bool bTurnOn);
    bool isVoiceEnabled() const;
    void preloadEffectSound(const char* pEffect);
    void preloadBackgroundSound(const char* pBackground);
    void playEffectSound(const char* pEffect, bool bLoop = false);
    void playBackgroundSound(const char* pBackground, bool bLoop = true);

    int getTotalChapter(){ return m_iTotalChapter; }
    int getCompletedChapter() { return m_iCompletedChapter; }

    void setCmd(CMD_RECV eCmd);

protected:
    CCSpriteFrameCache* m_pFc;
    CCAnimationCache* m_pAc;
    CCDirector* m_pDr;
    CCArray m_oArrSceneStack;
    CUnitGroup m_oArrUnit;
    CUnitGroup m_oArrProjectile;
    CCArray m_oUnitDustbin;
    CCArray m_oProjectileDustbin;
    SimpleAudioEngine* m_pAudio;
    bool m_bTurnOnVoice;
    int m_iTotalChapter;
    int m_iCompletedChapter;
    CMD_RECV m_eCmdRecv;

public:
    CPlayerInfo m_oPlayerInfo;

};
