//
//  StartScene.cpp
//  GameDemo
//
//  Created by qiuhelin on 13-3-21.
//
//
#include "CommInc.h"
#include "StartScene.h"
#include "WHomeScene.h"


CCScene* CCStartLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    CCNodeLoaderLibrary *lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); //生成一个默认的Node Loader
    
    lib->registerCCNodeLoader("CStartLayer", CCStartLayerLoader::loader());

    CCBReader *reader = new CCBReader(lib); //用node load lib 初始化一个ccb reader
    
    CCNode *node = reader->readNodeGraphFromFile("HelloCocosBuilder.ccbi", scene); //从ccbi文件中加载node
    
    reader->release(); //注意手动释放内存
    
    if (node!=NULL)
    {
        scene->addChild(node); //将node 添加到scene中
        node->setScale(1.0);
    }
    
    return scene;
}

extension::SEL_CCControlHandler CCStartLayer::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStart", CCStartLayer::onStart)
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOption", CCStartLayer::onOption)
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onQuit", CCStartLayer::onQuit)
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelp", CCStartLayer::onHelp)
    return NULL;
}

SEL_MenuHandler CCStartLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName)
{
    return NULL;
}
void CCStartLayer::onStart(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCLOG("onStart");
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, CCWHomeScene::create()));
    
}
void CCStartLayer::onOption(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCLOG("onOption");
}
void CCStartLayer::onQuit(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCLOG("onQuit");
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void CCStartLayer::onHelp(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCLOG("onHelp");

}
