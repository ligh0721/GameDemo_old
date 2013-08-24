#include "CommInc.h"

#include "GameCtrl.h"
#include "UnitInfo.h"
#include "SkillInfo.h"
#include "WHomeScene.h"
#include "DemoScene.h"
#include "UnitEditorScene.h"


CGameManager::CGameManager()
{
    srand(time(NULL));
}

bool CGameManager::init()
{
    m_iTotalChapter = 18;
    m_iCompletedChapter = 1;
    m_pFc = CCSpriteFrameCache::sharedSpriteFrameCache();
    m_pAc = CCAnimationCache::sharedAnimationCache();
    m_pDr = CCDirector::sharedDirector();
    setScaleY(1);
    m_oArrSceneStack.init();
    m_bTurnOnVoice = true;

    m_eCmdRecv = kCmdNoting;
    M_DEF_DR(pDr);
#if defined QTPROJ
    pDr->getScheduler()->scheduleSelector(schedule_selector(CGameManager::cmdRecv), this, 0.1, false);
#endif

    return true;
}

CGameManager* CGameManager::sharedGameManager()
{
    static CGameManager* pGm = NULL;
    if (pGm)
    {
        return pGm;
    }
    pGm = CGameManager::create();
    pGm->retain();
    return pGm;
}

const CCSpriteFrameCache* CGameManager::Fc() const
{
    return m_pFc;
}

const CCAnimationCache* CGameManager::Ac() const
{
    return m_pAc;
}

const CCDirector* CGameManager::Dr() const
{
    return m_pDr;
}

CCSpriteFrame* CGameManager::getUnitFrame( const char* pUnit, const char* pFrame )
{
    char sz[256];
    if (!pFrame)
    {
        pFrame = pUnit;
    }
    sprintf(sz, "kr/%s/%s.png", pUnit, pFrame);
    return m_pFc->spriteFrameByName(sz);
}

CCAnimation* CGameManager::getUnitAnimation( const char* pUnit, const char* pAnimation )
{
    char sz[256];
    sprintf(sz, "kr/%s/%s", pUnit, pAnimation);
    return m_pAc->animationByName(sz);
}

CCAnimation* CGameManager::loadUnitAnimation( const char* pUnit, const char* pAnimation )
{
    char sz[256];
    CCSpriteFrame* pSf;
    CCAnimation* pAni = NULL;
    for (int i = 0; ; ++i)
    {
        sprintf(sz, "kr/%s/%s/%s_%02d.png", pUnit, pAnimation, pAnimation, i);
        pSf = m_pFc->spriteFrameByName(sz);
        if (!pSf)
        {
            if (i == 0)
            {
                return pAni;
            }
            break;
        }
        if (!pAni)
        {
            pAni = CCAnimation::create();
        }
        pAni->addSpriteFrame(pSf);
    }

    sprintf(sz, "kr/%s/%s", pUnit, pAnimation);
    m_pAc->addAnimation(pAni, sz);
    return pAni;
}

void CGameManager::pushScene( CCScene* pScene )
{
    m_oArrSceneStack.addObject(m_pDr->getRunningScene());
    m_pDr->replaceScene(CCTransitionFade::create(0.5, pScene), false);
}

void CGameManager::popScene()
{
    CCScene* pScene = dynamic_cast<CCScene*>(m_oArrSceneStack.lastObject());
    if (!pScene)
    {
        m_pDr->end();
    }
    else
    {
        m_pDr->replaceScene(CCTransitionFade::create(0.5, pScene));
        m_oArrSceneStack.removeLastObject();
    }
}

int CGameManager::keygen()
{
    static int iKeygen = rand() % 10000;
    return iKeygen++;
}

void CGameManager::setVoice( bool bTurnOn )
{
    m_bTurnOnVoice = bTurnOn;
}

bool CGameManager::isVoiceEnabled() const
{
    return m_bTurnOnVoice;
}

void CGameManager::preloadEffectSound( const char* pEffect )
{
    m_pAudio->preloadEffect(pEffect);
}

void CGameManager::preloadBackgroundSound( const char* pBackground )
{
    m_pAudio->preloadBackgroundMusic(pBackground);
}

void CGameManager::playEffectSound( const char* pEffect, bool bLoop )
{
    if (isVoiceEnabled())
    {
        m_pAudio->playEffect(pEffect, bLoop);
    }
}

void CGameManager::playBackgroundSound( const char* pBackground, bool bLoop )
{
    if (isVoiceEnabled())
    {
        m_pAudio->playBackgroundMusic(pBackground, bLoop);
    }
}

void CGameManager::cmdRecv( float fDt )
{
    M_DEF_DR(pDr);

    switch (m_eCmdRecv)
    {
    case kCmdNoting:
        return;
        break;

    case kCmdRun:
        pDr->replaceScene(CCWHomeScene::create());
        break;

    case kCmdEdit:
        pDr->replaceScene(CCUnitEditorScene::create());
        break;

    default:
        CCLOG("unknown cmd(%02X)", (uint8_t)m_eCmdRecv);
    }

    m_eCmdRecv = kCmdNoting;
}

void CGameManager::setCmd( CMD_RECV eCmd )
{
    m_eCmdRecv = eCmd;
}
