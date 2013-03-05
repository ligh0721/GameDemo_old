#include "CommInc.h"

#include "TechTreeScene.h"
#include "Unit.h"
#include "WHomeScene.h"
#include "Skill.h"

bool CCTechTreeScene::init()
{
	//m_pTechTreeLayer=CTechTreeLayer::singleTechTree();
    m_pTalentInfo=CTalentInfo::singleTalentInfo();
    addChild(m_pTalentInfo);
	//addChild(m_pTechTreeLayer);
	return true;
}


bool CCTechTreeLayer::init()
{
	CCLayer::init();

// 	M_DEF_FC(pFc);
// 	pFc->addSpriteFramesWithFile("background.plist");
// 	pFc->addSpriteFramesWithFile("tank.plist");
// 	pFc->addSpriteFramesWithFile("UI.plist");
	
	zeroPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	visibleSize=CCDirector::sharedDirector()->getVisibleSize();
	techArray[0]=1;
	for(int i=1;i<10;i++)
	{
		techArray[i]=0;
	}
	
	pMenu.init();
	pMenu.setPosition(CCPointZero);
	this->addChild(&pMenu,1);

	techOneItem.initWithString("Tech1",this,menu_selector(CCTechTreeLayer::techDevelopCallback));
	techOneItem.setPosition(zeroPoint.x+visibleSize.width/10,zeroPoint.y+visibleSize.height/2);
	techOneItem.setTag(1);
	techTwoItem.initWithString("Tech2",this,menu_selector(CCTechTreeLayer::techDevelopCallback));
	techTwoItem.setPosition(zeroPoint.x+visibleSize.width/4,zeroPoint.y+visibleSize.height/2);
	techTwoItem.setTag(2);	
	pMenu.addChild(&techOneItem);
	pMenu.addChild(&techTwoItem);
	m_oBackToGame.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png", NULL, this, menu_selector(CCTechTreeLayer::onBtnBackClick));
	m_oBackToGame.setPosition(ccp(zeroPoint.x+visibleSize.width/10,zeroPoint.y+visibleSize.height*0.9));
	pMenu.addChild(&m_oBackToGame);

	pMenu2.init();
	this->addChild(&pMenu2,2);
	pMenu2.setPosition(CCPointZero);
	pMenu2.setVisible(false);
	pMenu2.addChild(&learnItem);
	pMenu2.addChild(&cancelItem);
	learnItem.initWithString("learn",this,menu_selector(CCTechTreeLayer::learnCallback));
	cancelItem.initWithString("cancel",this,menu_selector(CCTechTreeLayer::cancelCallback));
	
	return true;
}

cocos2d::CCScene* CCTechTreeLayer::scene()
{
	CCScene* pScene=CCScene::create();
	CCTechTreeLayer* pLayer=CCTechTreeLayer::create();
	pScene->addChild(pLayer,1);
	return pScene;
}

void CCTechTreeLayer::techDevelopCallback(CCObject* pSender)
{
	CCMenuItemFont* temp=(CCMenuItemFont*)pSender;
	flagStore=temp->getTag();
	showDetail();
	if(techArray[flagStore]==1)
	{
		learnItem.setEnabled(false);
	}
	else if(techArray[flagStore-1]==0)
	{
		learnItem.setEnabled(false);
	}
	else
	{
		learnItem.setEnabled(true);
	}
}

void CCTechTreeLayer::showDetail()
{
	if(flagStore==1)
	{
		learnItem.setPosition(zeroPoint.x+visibleSize.width/10,zeroPoint.y+visibleSize.height/4);
		cancelItem.setPosition(zeroPoint.x+visibleSize.width/10,zeroPoint.y+visibleSize.height/5);
	}
	if (flagStore==2)
	{
		learnItem.setPosition(zeroPoint.x+visibleSize.width/4,zeroPoint.y+visibleSize.height/4);
		cancelItem.setPosition(zeroPoint.x+visibleSize.width/4,zeroPoint.y+visibleSize.height/5);
	}
	pMenu.onExit();
	pMenu2.setVisible(true);
}

void CCTechTreeLayer::learnCallback( CCObject* pSender )
{
	pMenu.onEnter();
	techArray[flagStore]=1;
	CCMenuItemFont* tempItem=(CCMenuItemFont*)pMenu.getChildByTag(flagStore);
	tempItem->setColor(ccc3(128,128,0));
	pMenu2.setVisible(false);
}

void CCTechTreeLayer::cancelCallback( CCObject* pSender )
{
	pMenu.onEnter();
	pMenu2.setVisible(false);
}

bool CCTechTreeLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLog("it received");
	return true;
}

CCTechTreeLayer::CCTechTreeLayer()
{

}

void CCTechTreeLayer::onBtnBackClick( CCObject* pSender )
{
	CCDirector::sharedDirector()->popScene();
}

bool CTechTreeLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    CCPoint zeroPoint=CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize winSize=CCDirector::sharedDirector()->getVisibleSize();


    m_oMenuMainTechTree.init();
    addChild(&m_oMenuMainTechTree);
    m_oMenuMainTechTree.setPosition(CCPointZero);

    //         m_oFirstTechTree.initWithString("Tech Tree 1",this,menu_selector(CTechTreeLayer::onBtnChooseTechTreeClick));
    //         m_oMenuMainTechTree.addChild(&m_oFirstTechTree,1);


    m_oFirstTechNode1.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTechTreeLayer::onBtnFirstTechNodeClick));
    m_oFirstTechNode2.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTechTreeLayer::onBtnFirstTechNodeClick));
    m_oFirstTechNode3.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTechTreeLayer::onBtnFirstTechNodeClick));
    m_oFirstTechNode4.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTechTreeLayer::onBtnFirstTechNodeClick));
    m_oFirstTechNode5.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTechTreeLayer::onBtnFirstTechNodeClick));
    m_oFirstTechNode6.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTechTreeLayer::onBtnFirstTechNodeClick));


    m_oMenuMainTechTree.addChild(&m_oFirstTechNode1);
    m_oMenuMainTechTree.addChild(&m_oFirstTechNode2);
    m_oMenuMainTechTree.addChild(&m_oFirstTechNode3);
    m_oMenuMainTechTree.addChild(&m_oFirstTechNode4);
    m_oMenuMainTechTree.addChild(&m_oFirstTechNode5);
    m_oMenuMainTechTree.addChild(&m_oFirstTechNode6);

    m_oFirstTechNode1.setTag(1);
    m_oFirstTechNode2.setTag(2);
    m_oFirstTechNode3.setTag(3);
    m_oFirstTechNode4.setTag(4);
    m_oFirstTechNode5.setTag(5);
    m_oFirstTechNode6.setTag(6);
    m_oFirstTechNode1.setPosition(zeroPoint.x+winSize.width*0.1,zeroPoint.y+winSize.height*0.6);
    m_oFirstTechNode2.setPosition(zeroPoint.x+winSize.width*0.25,zeroPoint.y+winSize.height*0.6);
    m_oFirstTechNode3.setPosition(zeroPoint.x+winSize.width*0.4,zeroPoint.y+winSize.height*0.6);
    m_oFirstTechNode4.setPosition(zeroPoint.x+winSize.width*0.55,zeroPoint.y+winSize.height*0.6);
    m_oFirstTechNode5.setPosition(zeroPoint.x+winSize.width*0.7,zeroPoint.y+winSize.height*0.6);
    m_oFirstTechNode6.setPosition(zeroPoint.x+winSize.width*0.85,zeroPoint.y+winSize.height*0.6);
    

    m_oButtonLearn.initWithString("Learn",this,menu_selector(CTechTreeLayer::onBtnLearnClick));
    m_oButtonCancel.initWithString("Cancel",this,menu_selector(CTechTreeLayer::onBtnCancelClick));

    m_oMenuMainTechTree.addChild(&m_oButtonLearn);
    m_oMenuMainTechTree.addChild(&m_oButtonCancel);
    m_oButtonLearn.setPosition(zeroPoint.x+winSize.width*0.7,zeroPoint.y+winSize.height*0.05);
    m_oButtonCancel.setPosition(zeroPoint.x+winSize.width*0.85,zeroPoint.y+winSize.height*0.05);
    //2代表已学，1代表选中，经过learn如果可以学习，则变为2
    m_nodeLine[0]=2;
    for (int i=1;i<7;i++)
    {
        m_nodeLine[i]=0;
    }
    m_oBtnBack.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTechTreeLayer::onBtnBackClick));
    m_oMenuMainTechTree.addChild(&m_oBtnBack);
    m_oBtnBack.setPosition(zeroPoint.x+winSize.width*0.85,zeroPoint.y+winSize.height*0.85);

    return true;
}

void CTechTreeLayer::onBtnFirstTechNodeClick( CCObject* pObject )
{
    CCMenuItemImage* menuClicked=(CCMenuItemImage*)pObject;
    CCLOG("clicked %d",menuClicked->getTag());
    int nodeNumber=menuClicked->getTag();
    int preNodeNumber=nodeNumber-1;
    //显示技能信息,
    if (m_nodeLine[nodeNumber]==2)
    {
        CCLOG("already learned");
    }

    if (m_nodeLine[nodeNumber]==0&&m_nodeLine[preNodeNumber]>0)
    {
        m_nodeLine[nodeNumber]=1;
    }
}

void CTechTreeLayer::onBtnLearnClick( CCObject *pObject )
{
    for (int i=1;i<7;i++)
    {
        if (m_nodeLine[i]==0)
        {
            return;
        }
        if(m_nodeLine[i]==1&&m_nodeLine[i-1]==2)
        {
            m_nodeLine[i]=2;
        }
    }
}

void CTechTreeLayer::onBtnCancelClick( CCObject *pObject )
{
    for (int i=1;i<7;i++)
    {
        if (m_nodeLine[i]==0)
        {
            return;
        }
        if (m_nodeLine[i]==1)
        {
            m_nodeLine[i]=0;
        }
    }

}

CTechTreeLayer::CTechTreeLayer()
{

}

CTechTreeLayer* CTechTreeLayer::singleTechTree()
{
    static CTechTreeLayer* singleLayer;
    if (singleLayer==NULL)
    {
        singleLayer=new CTechTreeLayer();
        singleLayer->init();
    }
    return singleLayer;
}

void CTechTreeLayer::onBtnBackClick( CCObject *pObject )
{
    /*
    CGameUnit* heroUnit = getHeroUnit();
    if (m_nodeLine[1]==2)
    {
        heroUnit->addSkill(CVampirePas::create(10.0));
    }*/
    CCDirector::sharedDirector()->popScene();
}

CTalentInfo* CTalentInfo::singleTalentInfo()
{
    static CTalentInfo* singleLayer;
    if (singleLayer==NULL)
    {
       // singleLayer=CTalentInfo::create();
        singleLayer=new CTalentInfo();
        singleLayer->init();
    }
    return singleLayer;
}

bool CTalentInfo::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    //generate forest
    //
    m_oArrTalentNode.init();
    TalentDemo();

    //draw MenuItem
    m_spaceLeft=100;
    m_spaceBottom=100;
    m_spaceLine=100;

    m_menu.init();
    m_menu.setPosition(CCPointZero);
    this->addChild(&m_menu);
    int lineNum=m_talentForest.size();
    M_DEF_FC(pFc);
    for (int line=0;line<lineNum;line++)
    {
        m_retPair[line]=0;
        int nodeNum=m_talentForest[line].size();
        for (int node=0;node<nodeNum;node++)
        {
            CCMenuItemImage* pItem = CCMenuItemImage::create( m_talentForest[line][node].sNormalImage,
                m_talentForest[line][node].sSelectedImage, m_talentForest[line][node].sDisabledImage
                , this, menu_selector(CTalentInfo::onTalentNodeClick));
            m_oArrTalentNode.addObject(pItem);
            if (node==0&&m_talentForest[line][node].iStatus!=2)
            {
                m_talentForest[line][node].iStatus=1;
            }
            else if (node!=0)
            {
                 m_talentForest[line][node].iStatus=0;
            }
               
            m_talentForest[line][node].pNodeImage = pItem;
            m_talentForest[line][node].pNodeImage->setPosition(
                m_spaceLeft+line*m_spaceLine,m_spaceBottom+node*m_spaceLine);
            m_menu.addChild(m_talentForest[line][node].pNodeImage);
        }

    }


    m_itemOk.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTalentInfo::onItemOkClick));
    m_menu.addChild(&m_itemOk);
    m_itemOk.setPosition(600,50);

    m_itemReset.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTalentInfo::onItemResetClick));
    m_menu.addChild(&m_itemReset);
    m_itemReset.setPosition(700,50);

    m_itemLearn.initWithNormalImage("UI/button01.png", "UI/button01DOWN.png",
        NULL, this, menu_selector(CTalentInfo::onItemLearnClick));
    m_menu.addChild(&m_itemLearn);
    m_itemLearn.setPosition(700,150);
    m_itemLearn.setEnabled(false);

    m_talentInfo.initWithString("Talent Info","Arial",20);
    this->addChild(&m_talentInfo);
    m_talentInfo.setPosition(ccp(600,400));
    //从全局获得
    //
    
    m_iSkillPoint=5;
    m_skillPointShow.init();
    this->addChild(&m_skillPointShow);
    m_skillPointShow.setPosition(ccp(600,450));
    setSkillPointShow();

    m_iFocusLine=-1;
    m_iFocusNode=-1;

    return true;
}

void CTalentInfo::addTalentTree( TALENT_TREE newTalent )
{
    m_talentForest.push_back(newTalent);
}

void CTalentInfo::addTalentNode( int treeNumber,TALENT_NODE newNode )
{
    m_talentForest[treeNumber].push_back(newNode);
}

void CTalentInfo::onTalentNodeClick( CCObject* pObject )
{
    CCMenuItemImage *pItem=(CCMenuItemImage*)pObject;
    CCPoint pos=pItem->getPosition();
    m_iFocusLine=(pos.x-m_spaceLeft)/m_spaceLine;
    m_iFocusNode=(pos.y-m_spaceBottom)/m_spaceLine;
    char* str;
    int len=strlen(m_talentForest[m_iFocusLine][m_iFocusNode].sInfo)*16/15+1;
    str=new char[len];
    for (int i=0,j=0;i<len;i++,j++)
    {
        if (j%15==14)
        {
            str[i]='\n';
            i++;
        }
        str[i]=m_talentForest[m_iFocusLine][m_iFocusNode].sInfo[j];
        
    }
    str[len-1]='\0';
    //m_talentInfo.setString(m_talentForest[m_iFocusLine][m_iFocusNode].sInfo);
    m_talentInfo.setString(str);
    if(m_talentForest[m_iFocusLine][m_iFocusNode].iStatus==1
        &&m_talentForest[m_iFocusLine][m_iFocusNode].iCost<=m_iSkillPoint)//可学
    {
        CCLOG("can Learn");
        m_itemLearn.setEnabled(true);
    }

    else if (m_talentForest[m_iFocusLine][m_iFocusNode].iStatus==0)
    {
        m_itemLearn.setEnabled(false);
        return;
    }

    else if (m_talentForest[m_iFocusLine][m_iFocusNode].iStatus==2)
    {
        m_itemLearn.setEnabled(false);
        return;
    }

    if (m_talentForest[m_iFocusLine][m_iFocusNode].iCost>m_iSkillPoint)
    {
        m_itemLearn.setEnabled(false);
        return;
    }
}

void CTalentInfo::onItemOkClick( CCObject* pObject )
{
    //依据天赋情况，获得技能

    CCDirector::sharedDirector()->popScene();
}

void CTalentInfo::onItemResetClick( CCObject* pObject )
{
    //重置
    int lineNum=m_talentForest.size();
    for (int line=0;line<lineNum;line++)
    {
        m_retPair[line]=0;
        int nodeNum=m_talentForest[line].size();
        for (int node=0;node<nodeNum;node++)
        {
            if (m_talentForest[line][node].iStatus==2)
            {
                m_iSkillPoint+=m_talentForest[line][node].iCost;
                CCSprite* pLearnSprite=CCSprite::create("UI/button01.png");
                m_talentForest[line][node].pNodeImage->setNormalImage(pLearnSprite);
            }
            if (node==0)
                m_talentForest[line][node].iStatus=1;
            else
                m_talentForest[line][node].iStatus=0;
        }

    }
    setSkillPointShow();
}

void CTalentInfo::onItemLearnClick( CCObject* pObject )
{
    //学习
    assert(m_iFocusLine>=0&&m_iFocusNode>=0);
    if (m_talentForest[m_iFocusLine][m_iFocusNode].iStatus!=1)
    {
        return;
    }
    m_talentForest[m_iFocusLine][m_iFocusNode].iStatus=2;
    CCSprite* pLearnSprite=CCSprite::create("UI/button01DISABLE.png");
    m_talentForest[m_iFocusLine][m_iFocusNode].pNodeImage->setNormalImage(pLearnSprite);
    int skillPoint=(int)m_iSkillPoint-m_talentForest[m_iFocusLine][m_iFocusNode].iCost;
    setSkillPoint(skillPoint);
    setSkillPointShow();
    if ((m_iFocusNode+1)<(int)m_talentForest[m_iFocusLine].size())
    {
        m_talentForest[m_iFocusLine][m_iFocusNode+1].iStatus=1;
    }
    m_retPair[m_iFocusLine]++;
}

void CTalentInfo::setSkillPointShow( void )
{
    //只能显示ascII码字符
    char* number;
    number=new char[4];
    number[2]=m_iSkillPoint%10+48;
    number[1]=(m_iSkillPoint/10)%10+48;
    number[0]=(m_iSkillPoint/100)%10+48;
    number[3]='\0';
    m_skillPointShow.setFontSize(30);
    m_skillPointShow.setString(number);
}

void CTalentInfo::TalentDemo()
{
    TALENT_NODE node11;
    TALENT_NODE node12;
    node12.sInfo="GoodBye";
    TALENT_NODE node21;
    node21.sInfo="Hello,abcdefghijklmnopqrstuvwxyz";
    node21.iCost=2;
    TALENT_NODE node31;
    TALENT_NODE node32;
    TALENT_NODE node33;
    TALENT_TREE line1;
    TALENT_TREE line2;
    TALENT_TREE line3;
    line1.push_back(node11);
    line1.push_back(node12);
    line2.push_back(node21);
    line3.push_back(node31);
    line3.push_back(node32);
    line3.push_back(node33);
    m_talentForest.push_back(line1);
    m_talentForest.push_back(line2);
    m_talentForest.push_back(line3);
}

void CTalentInfo::setSkillPoint( int skillPoint )
{
    m_iSkillPoint=skillPoint;
}

CTalentInfo::TALENT_NODE::TALENT_NODE()
    : iUnitIndex(0)
{
    iEffectIndex=0;
    iCost=1;
    iStatus=0;
    sInfo="We live to DIE ,BUT we die for LIFE";
    pNodeImage = NULL;
    sNormalImage="UI/button01.png";
    sSelectedImage="UI/button01DOWN.png";
    sDisabledImage="UI/button01DISABLE.png";
    //node11.pNodeImage->setNormalSpriteFrame(pFc->spriteFrameByName(M_SKILL_PATH("cancel")));
   // pNodeImage->initWithNormalImage(pFc->spriteFrameByName(M_SKILL_PATH("cancel")), pFc->spriteFrameByName(M_SKILL_PATH("cancel")), pFc->spriteFrameByName(M_SKILL_PATH("cancel")),CTalentInfo::singleTalentInfo(),menu_selector(CTalentInfo::onTalentNodeClick));
}
