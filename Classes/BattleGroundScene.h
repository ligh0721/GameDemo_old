#ifndef __BATTLEGROUNDSCENE_H__
#define __BATTLEGROUNDSCENE_H__


class CCBattleGroundLayer;

class CCCtrlLayer : public CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(CCCtrlLayer);

    void addUI();
    void onValueChanged(CCObject* pSender, CCControlEvent controlEvent){};

	void onPushPropScene(CCObject* pSender);
	void onPushChapterScene(CCObject* pSender);
	void onPushUnitShowScene(CCObject* pSender);
	
public:
    CCButtonPanel* m_pSkillPanel;
	CCPackagePanel* m_pPackagePanel;
};

class CCBackGroundLayer : public CCLayerColor
{
public:
    virtual bool initWithColor(const ccColor4B& color);
    CREATEWITH_FUNC_PARAM(Color, CCBackGroundLayer, (const ccColor4B& color), color);
};

class CCBattleGroundLayer : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    void initBattleGround();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    //static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    void onValueChanged(CCObject* pSender, CCControlEvent controlEvent);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();

    virtual void update(float dt);
    virtual void updateCamera();

    // implement the "static node()" method manually
    CREATE_FUNC_PARAM(CCBattleGroundLayer, ());

public:
    bool m_bIsTouching;

    b2World* m_pWorld;
};

class CCBattleGroundScene : public CCScene
{
public:
    virtual ~CCBattleGroundScene();
    virtual bool init();
    CREATE_FUNC(CCBattleGroundScene);

public:
};

#endif // __BATTLEGROUNDSCENE_H__
