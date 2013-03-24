//
//  StartScene.h
//  GameDemo
//
//  Created by qiuhelin on 13-3-21.
//
//

#ifndef GameDemo_StartScene_h
#define GameDemo_StartScene_h

class CStartLayer : public CCLayer,
                    public cocos2d::extension::CCBSelectorResolver
{
public:
    static CCScene* scene();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CStartLayer, create);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
    void onStart(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onOption(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onHelp(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onQuit(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
};
class CStartLayerLoader : public extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CStartLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CStartLayer);
};
#endif
