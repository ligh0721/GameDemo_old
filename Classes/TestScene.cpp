#include "CommInc.h"
#include "testScene.h"
#include "Action.h"

bool CCTestLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	CCPoint zeroPoint=CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize visibleSize=CCDirector::sharedDirector()->getVisibleSize();
	CCSprite* test1=CCSprite::create("CloseNormal.png");
	CCSprite* test2=CCSprite::create("CloseNormal.png");
	this->addChild(test1);
	this->addChild(test2);
	test1->setPosition(ccp(zeroPoint.x+visibleSize.width/4,zeroPoint.y+visibleSize.height/4));
	test2->setPosition(ccp(zeroPoint.x+visibleSize.width/2,zeroPoint.y+visibleSize.height/2));
	CCActionInterval* act1=CCMoveTo::create(10.0,ccp(zeroPoint.x+visibleSize.width*3/4,zeroPoint.y+visibleSize.height*3/4));
	test1->runAction(act1);
	CCActionInterval* act2=CCMoveToNode::create(3.0, test1, true);
	CCActionInterval* act3=CCMoveTo::create(5.0,ccp(zeroPoint.x+visibleSize.width*3/4,zeroPoint.y+visibleSize.height/2));
	CCFiniteTimeAction* actSeq1=CCSequence::create(act2,act3,NULL);
	CCAction* speedAct1=CCSpeed::create(act2,0.1);
	//test2->runAction(actSeq1);
	//test2->runAction(act2);
	test2->runAction(speedAct1);
	return true;
}

CCScene* CCTestLayer::scene()
{
	CCTestLayer* pLayer=CCTestLayer::create();
	CCScene* pScene=CCScene::create();
	pScene->addChild(pLayer);
	return pScene;
}
