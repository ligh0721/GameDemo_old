#ifndef __UNITSHOWSCENE_H_
#define __UNITSHOWSCENE_H_


#include "GameDisplay.h"

class CCUnitShowSceneLayer;

class CCUnitShowScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCUnitShowScene);

public:
    CCUnitShowSceneLayer* m_pUnitShowSceneLayer;
};

class CCUnitShowSceneLayer : public CCLayer
{
public:
    CCUnitShowSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCUnitShowSceneLayer);

    void onSkillImgClick(CCNode* pNode);
    void onSkillUpdateCancel(CCNode* pNode);

public:
	CCButtonPanel m_oSp;
    CCSprite m_oImage;
    CCLabelTTF m_oName;
    CCLabelTTF m_oHP;
	CCLabelTTF m_oMP;
    CCLabelTTF m_oAttackPower;
	CCLabelTTF m_oDefensePower;

};

#endif