#include "CommInc.h"

#include "GameLogic.h"
#include "GameDisplay.h"
#include "TankSprite.h"
#include "Unit.h"
#include "Skill.h"
#include "Box2DEx.h"
#include "BattleGroundScene.h"
#include "SkillUI.h"
#include "PropUI.h"
#include "GameChapterScene.h"
#include "UnitShowScene.h"

bool CCCtrlLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    CCSize oVis = CCDirector::sharedDirector()->getVisibleSize();
    setContentSize(oVis);
    CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pCache->addSpriteFramesWithFile("UI.plist");
    pCache->addSpriteFramesWithFile("skill.plist");
    CCSprite* pSprite = CCSprite::createWithSpriteFrameName("800x480.png");
    addChild(pSprite);
    pSprite->setPosition(ccp(oVis.width / 2, oVis.height / 2));

    return true;
}

void CCCtrlLayer::addUI()
{
    CCSize oVis = CCDirector::sharedDirector()->getVisibleSize();
    //CCSize oVis = CCDirector::sharedDirector()->getWinSize();
    CCPoint oOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCBattleGroundLayer* pLayer = CCGameManager::sharedGameManager()->m_pBattleGroundLayer;

    // Create a menu with the "close" menu item, it's an auto release object.
    //CCNode* pSkillBar = CCNode::create();
    m_pSkillPanel = CCButtonPanel::create(5, 1, 64, 7, 5, "Skill5_78x352_7_5_black.png");

    addChild(m_pSkillPanel);
    CCSize oSz = m_pSkillPanel->getContentSize();
    m_pSkillPanel->setPosition(ccp(oSz.width / 2, oVis.height - oSz.height / 2));

	m_pPackagePanel = CCPackagePanel::create(5, 1, 64, 7, 5, "Skill5_78x352_7_5_black.png");
	addChild(m_pPackagePanel);
	oSz = m_pSkillPanel->getContentSize();
	m_pPackagePanel->setPosition(ccp(oSz.width + oSz.width/2, oVis.height - oSz.height / 2));
	
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        pLayer,
        menu_selector(CCBattleGroundLayer::menuCloseCallback));


    if (CCApplication::sharedApplication()->getTargetPlatform() == kTargetIphone)
    {
        pCloseItem->setPosition(ccp(oVis.width - 20 + oOrigin.x, 20 + oOrigin.y));
    }
    else 
    {
        pCloseItem->setPosition(ccp(oVis.width - 40 + oOrigin.x, 40 + oOrigin.y));
    }

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    CCControlSwitch *pCtrlSwitch = CCControlSwitch::create(
        CCSprite::create("UI/switch-mask.png"),
        CCSprite::create("UI/switch-on.png"),
        CCSprite::create("UI/switch-off.png"),
        CCSprite::create("UI/switch-thumb.png"),
        CCLabelTTF::create("On", "Arial-BoldMT", 16),
        CCLabelTTF::create("Off", "Arial-BoldMT", 16));
    pCtrlSwitch->setPosition(ccp(50 + pCtrlSwitch->getContentSize().width / 2, 50));
    addChild(pCtrlSwitch, 2, 8);
    CCSize o = pCtrlSwitch->getContentSize();

    pCtrlSwitch->addTargetWithActionForControlEvents(pLayer, cccontrol_selector(CCBattleGroundLayer::onValueChanged), CCControlEventValueChanged);
    pLayer->onValueChanged(pCtrlSwitch, CCControlEventValueChanged);

	 CCMenuItemFont* item1 = CCMenuItemFont::create( "chapter", this, menu_selector(CCCtrlLayer::onPushChapterScene) );
	 CCMenu* menu = CCMenu::create( item1, NULL );
	 menu->alignItemsVertically();
	 addChild( menu );
	 menu->setPosition(80,  oVis.height - oSz.height - 10);


	 CCMenuItemFont* item2 = CCMenuItemFont::create( "unit", this, menu_selector(CCCtrlLayer::onPushUnitShowScene) );
	 CCMenu* menu2 = CCMenu::create( item2, NULL );
	 menu2->alignItemsVertically();
	 addChild( menu2 );
	 menu2->setPosition(80,  oVis.height - oSz.height - 40);
    //pPb->setPosition(getAnchorPointInPoints());
    //pPb->setPercentage(100);

}

void CCCtrlLayer::onPushChapterScene(CCObject* pSender)
{
	CCScene* pScene = CCGameChapterScene::create();
	CCDirector::sharedDirector()->pushScene(pScene);

}

void CCCtrlLayer::onPushUnitShowScene(CCObject* pSender)
{
	CCScene* pScene = CCSkillUpdateScene::create();
	CCDirector::sharedDirector()->pushScene(pScene);
}

void CCCtrlLayer::onPushPropScene(CCObject* pSender)
{
	CCScene* pPropScene = CCPropUpdateScene::create();
	CCDirector::sharedDirector()->pushScene(pPropScene);

}

bool CCBackGroundLayer::initWithColor( const ccColor4B& color )
{
    if (!CCLayerColor::initWithColor(color))
    {
        return false;
    }

    return true;
}

const char* g_pTankBodys[] = {
    "body/body1.png",
    "body/body3.png",
    "body/body5.png",
    "body/body6.png",
    "body/body7.png",
    "body/body10.png",
    "body/body11.png",
    "body/body12.png",
    "body/body14.png",
    "body/body16.png",
    "body/body17.png",
    "body/body19.png",
};

const char* g_pTankGuns[] = {
    "gun/gun9.png",
    "gun/gun10.png",
    "gun/gun12.png",
    "gun/gun13.png",
    "gun/gun14.png",
    "gun/gun15.png",
    "gun/gun16.png",
    "gun/gun17.png",
    "gun/gun18.png",
};

// on "init" you need to initialize your instance
bool CCBattleGroundLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!CCLayer::init())
    {
        return false;
    }
    //setContentSize(CCSizeMake(1000, 1000));
    //setColor(ccc3(128, 128, 128));

    CCSize s = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // add a label shows "Hello World1"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Development Tools", "Arial", 24);

    // position the label on the center of the screen
    pLabel->setPosition(ccp(s.width/2 + origin.x, s.height - 50 + origin.y));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    //return true;
    CCSpriteFrameCache* pCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    pCache->addSpriteFramesWithFile("tank.plist");

    //CCSprite* pSprite = CCSprite::createWithSpriteFrameName("bg_1600x1200.jpg");
    //addChild(pSprite, -10);

    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

    this->scheduleUpdate();

    b2Vec2 gravity;
    gravity.Set(0.0f, -0.0f);
    m_pWorld = new b2World(gravity);
    m_pWorld->SetAllowSleeping(false);

    // Do we want to let bodies sleep?
    //m_pWorld->SetAllowSleeping(true);

    m_pWorld->SetContinuousPhysics(true);

    //     m_debugDraw = new GLESDebugDraw(PTM_RATIO);
    //     world->SetDebugDraw(m_debugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);


#if 0
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = m_pWorld->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom

    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
    
#endif
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    pGm->m_pPhyWorld = m_pWorld;

    pGm->preloadEffectSound("sound/fire.wav");
    pGm->preloadEffectSound("sound/explosion.wav");
    pGm->preloadEffectSound("sound/hitrobot.wav");
    pGm->preloadEffectSound("sound/InvisibilityTarget.wav");
    pGm->preloadEffectSound("sound/DispelMagicTarget.wav");
    pGm->preloadEffectSound("sound/GlueScreenMeteorHit1.wav");
    pGm->preloadEffectSound("sound/missile.wav");
    pGm->preloadEffectSound("sound/Fire_Energizer.wav");
    pGm->preloadEffectSound("sound/Fire_Disease.wav");
    pGm->preloadEffectSound("sound/Fire_Napalm.wav");
    pGm->preloadEffectSound("sound/Fire_Artillery.wav");
    pGm->preloadEffectSound("sound/Fire_Missiles.wav");
    pGm->preloadEffectSound("sound/explo0.wav");
    pGm->setVoice(true);

	//add the b2dWorld node
	//this->addChild(CCB2dWorldNode::sharedB2dWorld());

    return true;
}

void CCBattleGroundLayer::initBattleGround()
{

}

void CCBattleGroundLayer::update(float dt)
{
    int velocityIterations = 10;
    int positionIterations = 10;

    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    m_pWorld->Step(dt, velocityIterations, positionIterations);

	updateCamera();

    static CCProgressBar* pPb = NULL;
    static CCLabelTTF* pLb = NULL;
    static float fOldExp = -1;
    
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CCNormalTankSprite* pPlayerTank = dynamic_cast<CCNormalTankSprite*>(pGm->getTankByKey(pGm->m_iPlayerKey));
    if (!pPlayerTank)
    {
        return;
    }
    CTank* pPlayerTankL = dynamic_cast<CTank*>(pPlayerTank->getLogicBody());

    if (!pPb)
    {
        CCSprite* pBorder = CCSprite::createWithSpriteFrameName("bar_320_32.png");
        //CCSprite* pBorder = CCSprite::createWithSpriteFrameName("status/healthbar_fill.png");
        CCSize oSz = pBorder->getContentSize();
        //pPb = CCProgressBar::create(CCSizeMake(oSz.width * 1.0, oSz.height * 1.0), CCSprite::createWithSpriteFrameName("barfill.png"), pBorder, 0, 0, true);
        pPb = CCProgressBar::create(CCSizeMake(512, 64), CCSprite::createWithSpriteFrameName("Loading-BarFill.png"), CCSprite::createWithSpriteFrameName("Loading-BarBorder.png"), 15, 30, false);
        pPb->setScale(0.8);
        pGm->m_pCtrlLayer->addChild(pPb);
        pPb->setPosition(ccpAdd(pGm->m_pCtrlLayer->getAnchorPointInPoints(), ccp(0, 210)));

        pLb = CCLabelTTF::create("", "Courier-Bold", 24);
        pLb->setColor(ccYELLOW);
        pPb->addChild(pLb);
        pLb->setPosition(pPb->getAnchorPointInPoints());
    }
    
    if (pPlayerTankL->getExp() != fOldExp)
    {
        fOldExp = pPlayerTankL->getExp();
        float fPer = pPlayerTankL->getExp() * 100 / pPlayerTankL->getMaxExp();
        pPb->setPercentage(fPer, 0.5);
        char szLvlInfo[64];
        sprintf(szLvlInfo, "Lv%u: %u/%u", pPlayerTankL->getLevel(), pPlayerTankL->getExp(), pPlayerTankL->getMaxExp());
        pLb->setString(szLvlInfo);
    }
    //fSum += dt;

}

void CCBattleGroundLayer::menuCloseCallback(CCObject* pSender)
{
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
    if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    {
        CCDirector::sharedDirector()->end();
    }
    
    CCDirector::sharedDirector()->pushScene(CCSkillUpdateScene::create());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void CCBattleGroundLayer::onValueChanged( CCObject* pSender, CCControlEvent controlEvent )
{
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CCNormalTankSprite* pPlayerTank = dynamic_cast<CCNormalTankSprite*>(pGm->getTankByKey(pGm->m_iPlayerKey));
    if (!pPlayerTank)
    {
        return;
    }

    CCControlSwitch* pSwitch = dynamic_cast<CCControlSwitch*>(pSender);
    CTankDemoByTS* pTankL = dynamic_cast<CTankDemoByTS*>(pPlayerTank->getLogicBody());
    pTankL->m_bUseAI = pSwitch->isOn();
    pPlayerTank->stopMove();
}

bool CCBattleGroundLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CCSize oSz = pGm->getLogicBody()->getStageSize();
    CCNormalTankSprite* pPlayerTank = dynamic_cast<CCNormalTankSprite*>(pGm->getTankByKey(pGm->m_iPlayerKey));
    int iTankCount = pGm->getTankCount();
    CCTankSprite* pTank = NULL;
    if (iTankCount < 2 || !pPlayerTank)
    {
        CTank* pTankLogic = NULL;
        CCSprite* pGun = NULL;
        for (int i = 0; i < 3; i++)
        {
            pTankLogic = CreateLogicObject<CTankDemoByTS>();
            pTankLogic->setMaxHp(500);
            pTankLogic->setHp(500);
            pTankLogic->setBonusExp(pTankLogic->getMaxHp() / 4.7);
            pTankLogic->setForceByIndex(2);
            pTankLogic->setAlly(1 << 2);
            pGun = CCSprite::createWithSpriteFrameName(g_pTankGuns[rand() % (sizeof(g_pTankGuns) / sizeof(g_pTankGuns[0]))]);
            pTank = CCSkillTankSprite::createWithSpriteFrameNameAndLogicGun(g_pTankBodys[rand() % (sizeof(g_pTankBodys) / sizeof(g_pTankBodys[0]))], pTankLogic, pGun);
            pGm->addTank(pTank, ccp(rand() % (int)oSz.width, rand() % (int)oSz.height), rand() % 360);

        }
        pTankLogic = CreateLogicObject<CTankDemoByTS>();
        pTankLogic->setMaxHp(500);
        pTankLogic->setBonusExp(pTankLogic->getMaxHp() / 4.7);
        pTankLogic->setForceByIndex(1);
        pTankLogic->setAlly((1 << 1) | (1 << 0));
        pGun = CCSprite::createWithSpriteFrameName("gun/gun15.png");
        pTank = CCSkillTankSprite::createWithSpriteFrameNameAndLogicGun("body/body17.png", pTankLogic, pGun);
        pGm->addTank(pTank, ccp(rand() % (int)oSz.width, rand() % (int)oSz.height), rand() % 360);

        // Player Tank
        if (!pPlayerTank)
        {
            pTankLogic = CreateLogicObject<CTankDemoByTS>();
            pGm->m_iPlayerKey = pTankLogic->getLogicKey();
            pGun = CCSprite::createWithSpriteFrameName("gun/gun14.png");
            pTank = CCSkillTankSprite::createWithSpriteFrameNameAndLogicGun("body/body16.png", pTankLogic, pGun);
            pGm->addTank(pTank, ccp(rand() % (int)oSz.width, rand() % (int)oSz.height), rand() % 360);
            pPlayerTank = dynamic_cast<CCNormalTankSprite*>(pTank);
            
            pTankLogic->setMaxHp(200);
            pTankLogic->setHp(100);
            pTankLogic->setBonusExp(pTankLogic->getMaxHp() / 4.7);
            pTankLogic->setForceByIndex(0);
            pTankLogic->setAlly((1 << 1) | (1 << 0));
            
        }
        onValueChanged(pGm->m_pCtrlLayer->getChildByTag(8), CCControlEventValueChanged);
    }
#if 0
	CCSprite* pSprite = CCSprite::createWithSpriteFrameName("body/body17.png");
	pSprite->setPosition(CCPointMake(90, 90));
	this->addChild(pSprite);
	CCB2dAction* action = CCB2dAction::create();
	pSprite->runAction(action);
#endif
    if (touch->getLocation().x < 160)
    {
        return false;
    }

    if (!dynamic_cast<CTankDemoByTS*>(pPlayerTank->getLogicBody())->m_bUseAI)
    {
        pPlayerTank->stopMove();
        pPlayerTank->moveTo(ccpSub(touch->getLocation(), getPosition()), 200);
    }
    
    return true;
}

void CCBattleGroundLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    setPosition(ccpAdd(getPosition(), touch->getDelta()));
}

void CCBattleGroundLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
}

void CCBattleGroundLayer::touchDelegateRetain()
{
    this->retain();
}

void CCBattleGroundLayer::touchDelegateRelease()
{
    this->release();
}

void CCBattleGroundLayer::updateCamera()
{
    CCGameManager* pGm = CCGameManager::sharedGameManager();
    CCTankSprite* pTank = pGm->getTankByKey(pGm->m_iPlayerKey);
    if (!pTank)
    {
        return;
    }

	CCSize oSz = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint oLayerLoc = getPosition();
	CCPoint oCenter(oSz.width / 2 + 80, oSz.height / 2);
    CCPoint oLoc = pTank->getPosition();

	float fVal = 0.05f; //camera与Tank之间的弹簧 系数
	
	CCPoint oDelta;
	oDelta.x = fVal * (oCenter.x - oLayerLoc.x - oLoc.x);
	oDelta.y = fVal * (oCenter.y - oLayerLoc.y - oLoc.y);

	setPosition(ccpAdd(oDelta, oLayerLoc));
}


bool CCBattleGroundScene::init()
{
    CCScene::init();
    CCGameManager* pGm = CCGameManager::sharedGameManager();

    pGm->addBattleGroundLayer(this);
    pGm->addCtrlLayer(this);
    pGm->addBackGroundLayer(this);

    pGm->m_pCtrlLayer->addUI();

    return true;
}

CCBattleGroundScene::~CCBattleGroundScene()
{
    CCLOG("DTOR");
}
