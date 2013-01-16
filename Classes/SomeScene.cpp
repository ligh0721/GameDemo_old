#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "TankSprite.h"
#include "Unit.h"
#include "Skill.h"
#include "Box2DEx.h"
#include "BattleGroundScene.h"
#include "SomeScene.h"
#include "WHomeScene.h"
#include "MissionInfo.h"
#include "DotaScene.h"
#include "TechTreeScene.h"

cocos2d::CCScene* CCMainLayer::scene()
{
	CCScene* pScene=CCScene::create();
	CCMainLayer* pLayer=CCMainLayer::create();
	pScene->addChild(pLayer);
	return pScene;
}

bool CCMainLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCLabelTTF* pLabel=CCLabelTTF::create("SOLO","Arial", 24);

	pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - pLabel->getContentSize().height));

	this->addChild(pLabel, 1);

	CCFiniteTimeAction* pAct=CCFadeIn::create(5.0);
	CCFiniteTimeAction* pAct2=CCFadeOut::create(4.0);
	CCFiniteTimeAction* pAct3=CCFadeTo::create(6.0,128);
	CCFiniteTimeAction* pActSeq=CCSequence::create(pAct,pAct2,pAct3,NULL);
	pLabel->runAction(pActSeq);

	CCLabelTTF* labelItem1=CCLabelTTF::create("start","Arial", 24);
	CCLabelTTF* labelItem2=CCLabelTTF::create("settings","Arial",24);
	CCLabelTTF* labelItem3=CCLabelTTF::create("others","Arial",24);

	CCMenuItemLabel* itemLabel1=CCMenuItemLabel::create(labelItem1,this,menu_selector(CCMainLayer::menuItemStartCallback));
	CCMenuItemLabel* itemLabel2=CCMenuItemLabel::create(labelItem2,this,menu_selector(CCMainLayer::menuItemSettingCallback));
	CCMenuItemLabel* itemLabel3=CCMenuItemLabel::create(labelItem3,this,menu_selector(CCMainLayer::menuItemOtherCallback));

	itemLabel1->setPosition(visibleSize.width/4,visibleSize.height/2);
	itemLabel2->setPosition(visibleSize.width/4,visibleSize.height/2-30);
	itemLabel3->setPosition(visibleSize.width/4,visibleSize.height/2-60);

	CCMenu* pMenu=CCMenu::create(itemLabel1,itemLabel2,itemLabel3,NULL);


	pMenu->setPosition(visibleSize.width/4,visibleSize.height/32);
	this->addChild(pMenu,1);
	return true;
}



void CCMainLayer::onExit()
{
	CCLayer::onExit();
	removeAllChildrenWithCleanup(true);
}

void CCMainLayer::menuItemStartCallback( CCObject* pSender )
{
	//CCScene* toScene=CCBattleGroundScene::create();
	CCScene* toScene=CCTechTreeScene::create();
	CCDirector::sharedDirector()->replaceScene(toScene);
}

void CCMainLayer::menuItemSettingCallback( CCObject* pSender )
{
	CCScene* toScene=CCHomeLayer::scene();
	CCDirector::sharedDirector()->replaceScene(toScene);
}

void CCMainLayer::menuItemOtherCallback( CCObject* pSender )
{
	CCScene* toScene=CCChooseTroopScene::create();
	CCDirector::sharedDirector()->replaceScene(toScene);
}

cocos2d::CCScene* CCHomeLayer::scene()
{
	CCScene* pScene=CCScene::create();
	CCHomeLayer* pLayer=CCHomeLayer::create();
	pScene->addChild(pLayer,1);
	return pScene;
}

void CCHomeLayer::menuItemOpenItems( CCObject* pSender )
{
	CCSize s=CCDirector::sharedDirector()->getVisibleSize();

	CCLabelTTF* tmpLabel1=CCLabelTTF::create("temp1","Arial",24);
	CCLabelTTF* tmpLabel2=CCLabelTTF::create("temp2","Arial",24);
	CCMenuItemLabel* tempItem1=CCMenuItemLabel::create(tmpLabel1,this,menu_selector(CCHomeLayer::tempItemOneCallback));
	CCMenuItemLabel* tempItem2=CCMenuItemLabel::create(tmpLabel2,this,menu_selector(CCHomeLayer::tempItemTwoCallback));
	tempItem1->setPosition(s.width/8,s.height/4);
	tempItem2->setPosition(s.width/8,s.height/4-30);

	CCMenu* tmpMenu=CCMenu::create(tempItem1,tempItem2,NULL);
	tmpMenu->setPosition(s.width/8,s.height/4);
	this->addChild(tmpMenu);
}

void CCHomeLayer::menuItemOpenSkills( CCObject* pSender )
{

}

bool CCHomeLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	CCSize s=CCDirector::sharedDirector()->getVisibleSize();

	CCLabelTTF* itemLabel=CCLabelTTF::create("Items","Arial",24);
	CCLabelTTF* skillLabel=CCLabelTTF::create("Skills","Arial",24);

	CCMenuItemLabel* menuItem=CCMenuItemLabel::create(itemLabel,this,menu_selector(CCHomeLayer::menuItemOpenItems));
	CCMenuItemLabel* menuSkill=CCMenuItemLabel::create(skillLabel,this,menu_selector(CCHomeLayer::menuItemOpenSkills));
	menuItem->setPosition(s.width/4,s.height/2-30);
	menuSkill->setPosition(s.width/4,s.height/2-60);

	CCMenu* pMenu=CCMenu::create(menuItem,menuSkill,NULL);
	pMenu->setPosition(s.width/4,s.height/2);

	addChild(pMenu);


	return true;
}

void CCHomeLayer::tempItemOneCallback( CCObject* pSender )
{
	CCMenuItemLabel* temp=(CCMenuItemLabel*)pSender;
	temp->getParent()->removeFromParentAndCleanup(true);
}

void CCHomeLayer::tempItemTwoCallback( CCObject* pSender )
{

}



bool CCChooseTroopLayer::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("skill.plist");
	for(int i=0;i<3;i++)
	{
		heroChosenArr[i]=0;
	}
	visibleSize=CCDirector::sharedDirector()->getVisibleSize();
	zeroPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	heroPanel=CCButtonPanel::create(1, 3, 64, 10, 10, NULL);
	heroChosenPanel=CCButtonPanel::create(1,2,64,10,10,NULL);
	this->addChild(heroPanel,2);
	this->addChild(heroChosenPanel,2);
	heroPanel->setPosition(zeroPoint.x+visibleSize.width/5,zeroPoint.y+visibleSize.height/4);
	heroChosenPanel->setPosition(zeroPoint.x+visibleSize.width/5,zeroPoint.y+visibleSize.height*3/4);
	CCSkillButtonNormal* normalButton=CCSkillButtonNormal::create(M_SKILL_PATH("cancel"),M_SKILL_DOWN_PATH("cancel"),NULL,NULL,NULL,0,this,callfuncN_selector(CCChooseTroopLayer::onHeroClick),NULL);
	CCSkillButtonNormal* normalButton2=CCSkillButtonNormal::create(M_SKILL_PATH("cancel"),M_SKILL_DOWN_PATH("cancel"),NULL,NULL,NULL,0,this,callfuncN_selector(CCChooseTroopLayer::onHeroClick),NULL);
	heroPanel->addButton(normalButton,0,0);
	heroPanel->addButton(normalButton2,1,0);
    normalButton->setTag(0);
    normalButton2->setTag(1);

    CCMenuItemImage* pOverItem=CCMenuItemImage::create("CloseNormal.png","CloseSelected.png",NULL,
        this,menu_selector(CCChooseTroopLayer::menuItemOverCallBack));
    CCMenu* pMenu=CCMenu::create(pOverItem,NULL);
    pOverItem->setPosition(zeroPoint.x+visibleSize.width*9/10,zeroPoint.y+visibleSize.height/10);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu);
	return true;
}

cocos2d::CCScene* CCChooseTroopLayer::scene()
{
	CCScene* pScene=CCScene::create();
	CCChooseTroopLayer* pLayer=CCChooseTroopLayer::create();
	pScene->addChild(pLayer,1);

	return pScene;
}

void CCChooseTroopLayer::onHeroClick( CCNode* pNode )
{
	//CCLog("clicked");
	int i=0;
	while(heroChosenArr[i]!=0)
	{
		i++;
		if(i>2)
			break;
	}
	if(i<=2)
	{
		CCSkillButtonNormal* tempButton=(CCSkillButtonNormal*)pNode;
		//tempButton->retain();
		//heroPanel->delButton(tempButton);
		
		CCSkillButtonNormal* tempButtonChosen=tempButton->copyImg(this,callfuncN_selector(CCChooseTroopLayer::onHeroChosenClick),NULL);
		heroChosenPanel->addButton(tempButtonChosen,i,0);
		tempButtonChosen->setTag(i);
		heroChosenArr[i]=(tempButton->getTag()+1);
	}
}

void CCChooseTroopLayer::onHeroChosenClick( CCNode* pNode )
{
	CCSkillButtonNormal* tempButton=(CCSkillButtonNormal*)pNode;
	int i=tempButton->getTag();
    CCLog("%d\n",heroChosenArr[i]);
	tempButton->retain();

	heroChosenPanel->delButton(tempButton);

	heroChosenArr[i]=0;
}

CCChooseTroopLayer::CCChooseTroopLayer()
{

}


void CCChooseTroopLayer::menuItemOverCallBack( CCObject* pSender )
{
    //chosenHero´«µÝµ½Info
    
    MissionInfo* infoExp=MissionInfo::getComingMissionInfo();
// 
//     CCPoint *point1=new CCPoint(0,400);
//     CCPoint *point2=new CCPoint(300,400);
//     CCPoint *point3=new CCPoint(300,100);
//     CCPoint *point4=new CCPoint(600,200);
//     CCArray* soldierPathPointArray=new CCArray(4);
//     CCArray* soldierPathPointArray2=new CCArray(4);
//     soldierPathPointArray->addObject(point1);
//     soldierPathPointArray->addObject(point2);
//     soldierPathPointArray->addObject(point3);
//     soldierPathPointArray->addObject(point4);
// 
//     CCPoint *point11=new CCPoint(0,100);
//     CCPoint *point12=new CCPoint(300,100);
//     CCPoint *point13=new CCPoint(300,300);
//     CCPoint *point14=new CCPoint(600,200);
//     soldierPathPointArray2->addObject(point11);
//     soldierPathPointArray2->addObject(point12);
//     soldierPathPointArray2->addObject(point13);
//     soldierPathPointArray2->addObject(point14);
// 
//     CCArray* soldierAdvanceInfoArray=new CCArray(2);
//     SoldierAdvanceInfo* advanceInfoExp=SoldierAdvanceInfo::create(ccp(0,400),soldierPathPointArray);
//     SoldierAdvanceInfo* advanceInfoExp2=SoldierAdvanceInfo::create(ccp(0,100),soldierPathPointArray2);
//     advanceInfoExp->m_waveOfSoldier[1][0]=16;
//     advanceInfoExp->m_waveOfSoldier[1][1]=10;
//     advanceInfoExp2->m_waveOfSoldier[1][0]=10;
//     advanceInfoExp2->m_waveOfSoldier[1][1]=16;
//     soldierAdvanceInfoArray->addObject(advanceInfoExp);
//     soldierAdvanceInfoArray->addObject(advanceInfoExp2);
//     infoExp->init(soldierAdvanceInfoArray);
//     infoExp->setHeroChosen(heroChosenArr[0],heroChosenArr[1],heroChosenArr[2]);

    CCPoint *point1=new CCPoint(0,800);
    CCPoint *point2=new CCPoint(1000,0);
    CCPoint *point3=new CCPoint(2000,800);
    CCPoint *point4=new CCPoint(2000,800);
    CCPoint *point5=new CCPoint(1000,800);
    CCPoint *point6=new CCPoint(0,800);

    CCArray* pathPointArray1=new CCArray(2);
    pathPointArray1->addObject(point1);
    pathPointArray1->addObject(point4);
    CCArray* pathPointArray2=new CCArray(2);
    pathPointArray2->addObject(point2);
    pathPointArray2->addObject(point5);
    CCArray* pathPointArray3=new CCArray(2);
    pathPointArray3->addObject(point3);
    pathPointArray3->addObject(point6);

    SoldierAdvanceInfo* advanceInfoLeft=SoldierAdvanceInfo::create(ccp(0,800),pathPointArray1);
    SoldierAdvanceInfo* advanceInfoBottom=SoldierAdvanceInfo::create(ccp(1000,0),pathPointArray2);
    SoldierAdvanceInfo* advanceInfoRight=SoldierAdvanceInfo::create(ccp(2000,800),pathPointArray3);

    advanceInfoLeft->m_waveOfSoldier[1][1]=1;
    advanceInfoBottom->m_waveOfSoldier[1][1]=1;
    advanceInfoRight->m_waveOfSoldier[1][0]=1;

    CCArray* soldierAdvanceInfoArray=new CCArray(3);

    soldierAdvanceInfoArray->addObject(advanceInfoLeft);
    soldierAdvanceInfoArray->addObject(advanceInfoBottom);
    soldierAdvanceInfoArray->addObject(advanceInfoRight);

    infoExp->init(soldierAdvanceInfoArray);

    CCScene* pScene=CCDotaScene::create();
    CCDirector::sharedDirector()->replaceScene(pScene,true);
}



bool CCChooseTroopScene::init()
{
	m_pChooseTroopLayer=CCChooseTroopLayer::create();
	addChild(m_pChooseTroopLayer);
	return true;
}
