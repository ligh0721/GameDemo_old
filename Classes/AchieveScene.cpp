//
//  AchieveScene.cpp
//  GameDemo
//
//  Created by qiuhelin on 13-5-19.
//
//

#include "CommInc.h"

#include "GameDisplay.h"
#include "AchieveScene.h"


bool CAchieveScene::init()
{
	CCScene::init();
    m_pSceneLayer = CAchieveSceneLayer::create();
    addChild(m_pSceneLayer);
    return true;
}
CAchieveSceneLayer::CAchieveSceneLayer()
{
    
}

bool CAchieveSceneLayer::init()
{
	
	CCLayer::init();
    
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("skill.plist");
    //CCSkillInfoManager* pSim = CCSkillInfoManager::sharedSkillInfoManager();
    m_oSp.init(2, 8, 64, 10, 10, NULL);
    CCSkillButtonNormal* pSb;
    int maxRow = m_oSp.getRow() - 1;
    int maxLine = m_oSp.getLine() - 1;
    
    for (int line = 0; line < 2; line++)
    {
        for (int row = 0; row < 8; row++)
        {
            if (line == 1 && row == 0)
            {
                pSb = CCCommmButton::create(M_SKILL_PATH("cancel"), M_SKILL_DOWN_PATH("cancel"), M_SKILL_DIS_PATH("cancel"), NULL, NULL, 0, this, callfuncN_selector(CAchieveSceneLayer::onSkillUpdateCancel), NULL, 0);
            }
            else
            {
                pSb = CCCommmButton::create(M_SKILL_PATH("cancel"), M_SKILL_DOWN_PATH("cancel"), M_SKILL_DIS_PATH("cancel"), NULL, NULL, 0, this, callfuncN_selector(CAchieveSceneLayer::onSkillUpdateCancel), NULL, 0);
            }
    
            m_oSp.addButton(pSb, maxLine - row, maxRow - line);
        }
    }
    addChild(&m_oSp);
    m_oSp.setPosition(ccp(m_oSp.getContentSize().width / 2 + 20, oSz.height / 4));
	
    m_oImage.initWithFile("nice-cooker.jpeg");
    addChild(&m_oImage);
    m_oImage.setPosition(ccp(150, 350));
    m_oImage.setScale(0.5);
	
    m_oName.initWithString("name:nice-cooker", "Arial", 24, CCSizeMake(350, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    addChild(&m_oName);
    m_oName.setPosition(ccp(630, 450));
    
    m_oHP.initWithString("hp:2000", "Arial", 20, CCSizeMake(350, 64), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    addChild(&m_oHP);
    m_oHP.setPosition(ccp(630, 400));
    
    m_oMP.initWithString("mp:1500", "Arial", 20, CCSizeMake(350, 64), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    addChild(&m_oMP);
    m_oMP.setPosition(ccp(630, 350));
    
	m_oAttackPower.initWithString("dp:1500", "Arial", 20, CCSizeMake(350, 64), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    addChild(&m_oAttackPower);
    m_oAttackPower.setPosition(ccp(630, 300));
    
	m_oDefensePower.initWithString("df:1500", "Arial", 20, CCSizeMake(350, 64), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    addChild(&m_oDefensePower);
    m_oDefensePower.setPosition(ccp(630, 250));
    
    return true;
}

void CAchieveSceneLayer::onSkillUpdateCancel( CCNode* pNode )
{
	//CCGameManager::sharedGameManager()->popSceneWithTrans(0.5);
    
   // CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, CCMainScene::create()));
}

void CAchieveSceneLayer::onSkillImgClick( CCNode* pNode )
{
    
    /*  CCSkillInfoManager* pSim = CCSkillInfoManager::shareSkillInfoManager();
     CCSkillButtonNormalWithInfo* pBtn = dynamic_cast<CCSkillButtonNormalWithInfo*>(pNode);
     CSkillInfo* pSi = pSim->getSkillInfo(pBtn->getInfoKey());
     char szTmp[256];
     sprintf(szTmp, "%s/BTN%s.png", pSi->getButtonImage(), pSi->getButtonImage());
     m_oImage.setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szTmp));
     m_oName.setString(pSi->getName());
     //m_oInfo.setString(pSi->());
     */
}
