#pragma once

#include "GameDisplay.h"

class CCDemoSceneLayer;

class CCDemoScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCDemoScene);

public:
    CCDemoSceneLayer* m_pDemoSceneLayer;
};

class CCDemoSceneLayer : public CCLayerColor
{
protected:
    enum
    {
        kAct1 = 0,
        kAct2 = 1,
        kAct3 = 2,
        kAct4 = 3,
        kAct5 = 4,
        kAct6 = 5,
        kMove = 6,
        kDie = 7,
        kHome,
        kPrev,
        kNext
    };

    void onBtnClick(CCObject* pObject);

public:
    CCDemoSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCDemoSceneLayer);

    void addDemo(const CCPoint& roPos, const char* pBaseName, const char* pAction, float fDelay);
    void setUnit(const char* pUnit);
    void loadAllAnimation();
    void loadAnimation(const char* pUnit, const char* pAct, float fDelay);
    void onActEnd(CCNode* pNode);

public:
    CCMenu m_oMenu;

    CCMenuItemFont m_aoBtn[11];

    CCSprite m_oSprite;
    int m_iUnitPos;
    
};

