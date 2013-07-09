//
//  AchieveScene.h
//  GameDemo
//
//  Created by qiuhelin on 13-5-19.
//
//

#ifndef GameDemo_AchieveScene_h
#define GameDemo_AchieveScene_h

class CAchieveSceneLayer;

class CAchieveScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CAchieveScene);
    
public:
    CAchieveSceneLayer* m_pSceneLayer;
};

class CAchieveSceneLayer : public CCLayer
{
public:
    CAchieveSceneLayer();
    
    virtual bool init();
    CREATE_FUNC(CAchieveSceneLayer);
    
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
