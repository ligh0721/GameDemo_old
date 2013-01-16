#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "Action.h"
#include "DemoScene.h"
#include "HomeScene.h"


bool CCDemoScene::init()
{
    CCScene::init();
    m_pDemoSceneLayer = CCDemoSceneLayer::create();
    addChild(m_pDemoSceneLayer);

    return true;
}

const char* g_apUnit[] = {
    "Malik",
    "Magnus",
    "Paladin",
    "Bertha",
    "Arcane",
    "Tesla"
};

const char* g_apAct[] = {
    "act1",
    "act2",
    "act3",
    "act4",
    "act5",
    "act6",
    "move",
    "die"
};

CCDemoSceneLayer::CCDemoSceneLayer()
{
}

bool CCDemoSceneLayer::init()
{
    CCLayerColor::initWithColor(ccc4(204, 232, 207, 64));
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();

    CCAnimationCache* pAc = CCAnimationCache::sharedAnimationCache();
    CCSpriteFrameCache* pFc = CCSpriteFrameCache::sharedSpriteFrameCache();
    pFc->addSpriteFramesWithFile("unit.plist");

    m_oMenu.init();
    addChild(&m_oMenu);
    m_oMenu.setPosition(CCPointZero);

    int m = sizeof(m_aoBtn) / sizeof(CCMenuItemFont);
    for (int i = 0; i < m; ++i)
    {
        m_aoBtn[i].setTag(i);
    }

    m_aoBtn[kHome].initWithString("Home", this, menu_selector(CCDemoSceneLayer::onBtnClick));
    m_oMenu.addChild(&m_aoBtn[kHome]);
    m_aoBtn[kHome].setPosition(ccp(oSz.width * 0.85 + m_aoBtn[kHome].getContentSize().width * 0.5, oSz.height * 0.1));

    m_aoBtn[kPrev].initWithString("Prev", this, menu_selector(CCDemoSceneLayer::onBtnClick));
    m_oMenu.addChild(&m_aoBtn[kPrev]);
    m_aoBtn[kPrev].setPosition(ccp(oSz.width * 0.4, oSz.height * 0.2));

    m_aoBtn[kNext].initWithString("Next", this, menu_selector(CCDemoSceneLayer::onBtnClick));
    m_oMenu.addChild(&m_aoBtn[kNext]);
    m_aoBtn[kNext].setPosition(ccp(oSz.width * 0.6, oSz.height * 0.2));

    int n = sizeof(g_apAct) / sizeof(const char*);
    for (int i = 0; i < n; ++i)
    {
        m_aoBtn[i].initWithString(g_apAct[i], this, menu_selector(CCDemoSceneLayer::onBtnClick));
        m_oMenu.addChild(&m_aoBtn[i]);
        m_aoBtn[i].setPosition(ccp(oSz.width * 0.05 + m_aoBtn[i].getContentSize().width * 0.5, oSz.height * (0.9 - i * 0.08)));
    }

    m_iUnitPos = 0;
    m_oSprite.init();
    addChild(&m_oSprite);
    m_oSprite.setPosition(ccp(oSz.width * 0.5, oSz.height * 0.5));

    setUnit(g_apUnit[m_iUnitPos]);
    loadAllAnimation();

    return true;
}

void CCDemoSceneLayer::onBtnClick( CCObject* pObject )
{
    int iBtnPos = dynamic_cast<CCMenuItem*>(pObject)->getTag();
    int iUnitCount = sizeof(g_apUnit) / sizeof(const char*);

    switch (iBtnPos)
    {
    case kHome:
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, CCHomeScene::create()));
        break;

    case kAct1:
    case kAct2:
    case kAct3:
    case kAct4:
    case kAct5:
    case kAct6:
    case kMove:
    case kDie:
        {
            CCAnimationCache* pAc = CCAnimationCache::sharedAnimationCache();
            char sz[256];
            sprintf(sz, "%s/%s", g_apUnit[m_iUnitPos], g_apAct[iBtnPos]);
            CCAnimation* pAni = pAc->animationByName(sz);
            if (!pAni)
            {
                return;
            }
            m_oSprite.stopAllActions();
            m_oSprite.runAction(CCSequence::create(CCAnimate::create(pAni), CCCallFuncN::create(this, callfuncN_selector(CCDemoSceneLayer::onActEnd)), NULL));
        }
        break;

    case kPrev:
    case kNext:
        m_iUnitPos = (m_iUnitPos + iUnitCount + (iBtnPos == kPrev ? -1 : 1)) % iUnitCount;
        setUnit(g_apUnit[m_iUnitPos]);
        break;
    }
    
}

void CCDemoSceneLayer::setUnit(const char* pUnit)
{
    CCSpriteFrameCache* pFc = CCSpriteFrameCache::sharedSpriteFrameCache();
    char sz[256];

    sprintf(sz, "kr/%s/%s.png", pUnit, pUnit);
    m_oSprite.stopAllActions();
    m_oSprite.setDisplayFrame(pFc->spriteFrameByName(sz));
}

void CCDemoSceneLayer::loadAllAnimation()
{
    int n = sizeof(g_apUnit) / sizeof(const char*);
    int m = sizeof(g_apAct) / sizeof(const char*);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            loadAnimation(g_apUnit[i], g_apAct[j], 0.08);
        }
    }
}

void CCDemoSceneLayer::loadAnimation( const char* pUnit, const char* pAct, float fDelay )
{
    CCSpriteFrameCache* pFc = CCSpriteFrameCache::sharedSpriteFrameCache();
    CCAnimationCache* pAc = CCAnimationCache::sharedAnimationCache();
    char sz[256];
    CCSpriteFrame* pSf;
    CCAnimation* pAni = CCAnimation::create();
    for (int i = 0; ; ++i)
    {
        sprintf(sz, "kr/%s/%s/%s_%02d.png", pUnit, pAct, pAct, i);
        pSf = pFc->spriteFrameByName(sz);
        if (!pSf)
        {
            if (i == 0)
            {
                return;
            }
            break;
        }
        pAni->addSpriteFrame(pSf);
    }
    pAni->setDelayPerUnit(fDelay);
    sprintf(sz, "%s/%s", pUnit, pAct);
    pAc->addAnimation(pAni, sz);
}

void CCDemoSceneLayer::onActEnd( CCNode* pNode )
{
    setUnit(g_apUnit[m_iUnitPos]);
}

