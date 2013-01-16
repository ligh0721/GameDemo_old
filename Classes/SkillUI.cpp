#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "BulletSprite.h"
#include "TankSprite.h"
#include "Skill.h"
#include "BattleGroundScene.h"
#include "Action.h"
#include "SkillUI.h"


bool CCSkillUpdateScene::init()
{
    CCScene::init();
    m_pSkillUpdateLayer = CCSkillUpdateLayer::create();
    addChild(m_pSkillUpdateLayer);
    return true;
}

CCSkillDataSource::CCSkillDataSource()
{
    m_oSel.initWithFile("Sel_160x50.png");
    //m_oSel.setAnchorPoint(CCPointZero);
}

CCSize CCSkillDataSource::cellSizeForTable( CCTableView *table )
{
    return CCSizeMake(152, 44);
}

CCTableViewCell* CCSkillDataSource::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
    CCString *string = CCString::createWithFormat("%d", idx);
    CCTableViewCell *cell = table->dequeueCell();
    if (!cell)
    {
        cell = new CCTableViewCell();
        cell->autorelease();
        cell->setContentSize(cellSizeForTable(table));
        CCSprite* sprite = CCSprite::create("Holder_150x40.png");
        sprite->setAnchorPoint(CCPointZero);
        sprite->setPosition(ccp(0, 0));
        cell->addChild(sprite);
        sprite->setTag(CCSkillDataSource::CONST_SEL_tAG);
        //sprite->setAnchorPoint(ccp(0.5, 0.5));

        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
        label->setPosition(CCPointZero);
        label->setAnchorPoint(CCPointZero);
        label->setTag(123);
        cell->addChild(label);
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
        label->setString(string->getCString());
    }


    return cell;
}

unsigned int CCSkillDataSource::numberOfCellsInTableView( CCTableView *table )
{
    return 20;
}

void CCSkillDataSource::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
    CCLOG("%d touched", cell->getIdx());
    m_oSel.runAction(CCSelected::create(cell/*->getChildByTag(CCSkillDataSource::CONST_SEL_tAG)*/));
}

CCSkillDataSource* CCSkillDataSource::shareSkillDataSource()
{
    static CCSkillDataSource* pSds = NULL;
    if (pSds)
    {
        return pSds;
    }
    pSds = new CCSkillDataSource();
    return pSds;
}

void CCSkillDataSource::scrollViewDidScroll( CCScrollView* view )
{

}

void CCSkillDataSource::scrollViewDidZoom( CCScrollView* view )
{

}

CCSkillUpdateLayer::CCSkillUpdateLayer()
{

}

bool CCSkillUpdateLayer::init()
{
    CCLayer::init();
    CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
    /*
    CCSkillDataSource* pSds = CCSkillDataSource::shareSkillDataSource();
    CCTableView* tableView = CCTableView::create(pSds, CCSizeMake(150, oSz.height * 3 / 4));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setPosition(ccp(5, (oSz.height - oSz.height * 3 / 4) / 2));
    tableView->setDelegate(pSds);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    this->addChild(tableView);
    tableView->reloadData();
    */
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("skill.plist");
    CCSkillInfoManager* pSim = CCSkillInfoManager::sharedSkillInfoManager();
    m_oSp.init(5, 3, 64, 10, 10, NULL);
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

    for (int line = 0; line < 5; line++)
    {
        for (int row = 0; row < 3; row++)
        {
            if (line == 4 && row == 0)
            {
                pSb = CCSkillButtonNormal::create(M_SKILL_PATH("cancel"), M_SKILL_DOWN_PATH("cancel"), M_SKILL_DIS_PATH("cancel"), NULL, NULL, 0, this, callfuncN_selector(CCSkillUpdateLayer::onSkillUpdateCancel), NULL);
            }
            else
            {
                pSb = CCSkillButtonNormalWithInfo::create(NULL, NULL, 0, this, callfuncN_selector(CCSkillUpdateLayer::onSkillImgClick), NULL, apSkillInfo[rand() % 4]->getKey());
            }
            m_oSp.addButton(pSb, maxLine - row, maxRow - line);
        }
    }
    addChild(&m_oSp);
    m_oSp.setPosition(ccp(m_oSp.getContentSize().width / 2 + 20, oSz.height / 2));

    m_oImage.init();
    addChild(&m_oImage);
    m_oImage.setPosition(ccp(350, 400));
    m_oImage.setScale(1.5);

    m_oName.initWithString("", "Arial", 24, CCSizeMake(350, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    addChild(&m_oName);
    m_oName.setPosition(ccp(585, 417));

    m_oDescribe.initWithString("", "Arial", 16, CCSizeMake(350, 64), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    addChild(&m_oDescribe);
    m_oDescribe.setPosition(ccp(585, 380));

    m_oInfo.init();
    addChild(&m_oInfo);
    m_oInfo.setPosition(ccp(300, 300));

    return true;
}

void CCSkillUpdateLayer::onSkillUpdateCancel( CCNode* pNode )
{
    CCDirector::sharedDirector()->popScene();
}

void CCSkillUpdateLayer::onSkillImgClick( CCNode* pNode )
{
    
    CCSkillInfoManager* pSim = CCSkillInfoManager::sharedSkillInfoManager();
    CCSkillButtonNormalWithInfo* pBtn = dynamic_cast<CCSkillButtonNormalWithInfo*>(pNode);
    CSkillInfo* pSi = pSim->getSkillInfo(pBtn->getInfoKey());
    char szTmp[256];
    sprintf(szTmp, "%s/BTN%s.png", pSi->getButtonImage(), pSi->getButtonImage());
    m_oImage.setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(szTmp));
    m_oName.setString(pSi->getName());
    m_oDescribe.setString(pSi->getDescribe());
    //m_oInfo.setString(pSi->());
}
