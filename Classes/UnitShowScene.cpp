#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "Action.h"
#include "UnitShowScene.h"
#include "MainScene.h"


bool CCUnitShowScene::init()
{
	CCScene::init();
    m_pUnitShowSceneLayer = CCUnitShowSceneLayer::create();
    addChild(m_pUnitShowSceneLayer);
    return true;
}
CCUnitShowSceneLayer::CCUnitShowSceneLayer()
{

}

bool CCUnitShowSceneLayer::init()
{
	
	CCLayer::init();

    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();

    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("skill.plist");
    CCSkillInfoManager* pSim = CCSkillInfoManager::sharedSkillInfoManager();
    m_oSp.init(2, 8, 64, 10, 10, NULL);
    CCSkillButtonNormal* pSb;
    int maxRow = m_oSp.getRow() - 1;
    int maxLine = m_oSp.getLine() - 1;
    CSkillInfo* apSkillInfo[4];
    apSkillInfo[0] = CSkillInfo::create(1, 2, "Recover", "Harvey Cheyne is the over-indulged son of a millionaire. When he falls overboard from an ocean liner her is rescued by a Portuguese fisherman and ...", "skill1");
    apSkillInfo[1] = CSkillInfo::create(1, 2, "Power Shield", "Through the medium of an exciting adventure story, Captain's Courageous", "skill2");
    apSkillInfo[2] = CSkillInfo::create(1, 2, "Ice Amor", "It was the forty-fathom slumber that clears the soul and eye and heart, and sends you to breakfast ravening", "skill3");
    apSkillInfo[3] = CSkillInfo::create(1, 2, "Fire Ball", "It was another perfect day-soft, mild, and clear; and Harvey breathed to the very bottom of his lungs", "skill4");
    pSim->addSkillInfo(apSkillInfo[0]);
    pSim->addSkillInfo(apSkillInfo[1]);
    pSim->addSkillInfo(apSkillInfo[2]);
    pSim->addSkillInfo(apSkillInfo[3]);

    for (int line = 0; line < 2; line++)
    {
        for (int row = 0; row < 8; row++)
        {
            if (line == 1 && row == 0)
            {
                pSb = CCSkillButtonNormal::create(M_SKILL_PATH("cancel"), M_SKILL_DOWN_PATH("cancel"), M_SKILL_DIS_PATH("cancel"), NULL, NULL, 0, this, callfuncN_selector(CCUnitShowSceneLayer::onSkillUpdateCancel), NULL);
            }
            else
            {
                pSb = CCSkillButtonNormalWithInfo::create(NULL, NULL, 0, this, callfuncN_selector(CCUnitShowSceneLayer::onSkillImgClick), NULL, apSkillInfo[rand() % 4]->getKey());
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

void CCUnitShowSceneLayer::onSkillUpdateCancel( CCNode* pNode )
{
	//CCGameManager::sharedGameManager()->popSceneWithTrans(0.5);

    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, CCMainScene::create()));
}

void CCUnitShowSceneLayer::onSkillImgClick( CCNode* pNode )
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
