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
                    public extension::CCBSelectorResolver,
                    public extension::CCBMemberVariableAssigner,
                    public extension::CCNodeLoaderListener
{
public:
    CStartLayer();
    virtual ~CStartLayer();
    static CCScene* scene();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CStartLayer, create);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, CCString * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, CCString * pMemberVariableName, CCNode * pNode);
    virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
    void onStart(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onOption(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onHelp(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
    void onQuit(cocos2d::CCObject *pSender, cocos2d::extension::CCControlEvent pCCControlEvent);
private:
    CCLabelBMFont* m_pLabelText;
};
class CStartLayerLoader : public extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CStartLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CStartLayer);
};
#endif
