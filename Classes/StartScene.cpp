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

CStartLayer::CStartLayer():m_pLabelText(NULL)
{}

CStartLayer::~CStartLayer()
{
    CC_SAFE_DELETE(m_pLabelText);
}
CCScene* CStartLayer::scene()
{
    CCScene *scene = CCScene::create();
    
    CCNodeLoaderLibrary *lib = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary(); //生成一个默认的Node Loader
    
    lib->registerCCNodeLoader("CStartLayer", CStartLayerLoader::loader());

    CCBReader *reader = new CCBReader(lib); //用node load lib 初始化一个ccb reader
    
    CCNode *node = reader->readNodeGraphFromFile("HelloCocosBuilder.ccbi", scene); //从ccbi文件中加载node
    
    reader->release(); //注意手动释放内存
    
    if (node!=NULL)
    {
        scene->addChild(node); //将node 添加到scene中
    }
    
    return scene;
}

extension::SEL_CCControlHandler CStartLayer::onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onStart", CStartLayer::onStart)
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOption", CStartLayer::onOption)
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onQuit", CStartLayer::onQuit)
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHelp", CStartLayer::onHelp)
    return NULL;
}

SEL_MenuHandler CStartLayer::onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName)
{
    return NULL;
}
bool CStartLayer::onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "LabelText", CCLabelBMFont*, this->m_pLabelText);
    return true;

}
void CStartLayer::onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
{
    m_pLabelText->setString("hello world");
}

void CStartLayer::onStart(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCLOG("onStart");
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5, CCWHomeScene::create()));
    
}
void CStartLayer::onOption(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCLOG("onOption");
    m_pLabelText->setString("click option");
}
void CStartLayer::onQuit(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCLOG("onQuit");
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void CStartLayer::onHelp(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent)
{
    CCLOG("onHelp");

}
