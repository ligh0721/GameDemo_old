
class CCMainLayer:cocos2d::CCLayer
{
public:
	bool init();
	CREATE_FUNC(CCMainLayer);
	static cocos2d::CCScene* scene();
	void onExit();

	void menuItemStartCallback(CCObject* pSender);
	void menuItemSettingCallback(CCObject* pSender);
	void menuItemOtherCallback(CCObject* pSender);
};


class CCHomeLayer:public cocos2d::CCLayer
{
public:
	bool init();

	void menuItemOpenItems(CCObject* pSender);
	void menuItemOpenSkills(CCObject* pSender);
	void tempItemOneCallback(CCObject* pSender);
	void tempItemTwoCallback(CCObject* pSender);
	CREATE_FUNC(CCHomeLayer);
	static cocos2d::CCScene* scene();
	// 	void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	// 	void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	// 	void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
};







class CCChooseTroopLayer:public cocos2d::CCLayer
{
public:
	CCChooseTroopLayer();
	CREATE_FUNC(CCChooseTroopLayer);
	virtual bool init();
	void onHeroClick(CCNode* pNode);
	void onHeroChosenClick(CCNode* pNode);
	static cocos2d::CCScene* scene();
    void menuItemOverCallBack(CCObject* pSender);

protected:
	CCButtonPanel* heroPanel;
//	CCSkillPanel* soldierPanel;
	CCButtonPanel* heroChosenPanel;
//	CCSkillPanel* soldierChosenPanel;
	CCSize visibleSize;
	CCPoint zeroPoint;
	int heroChosenArr[3];
};




class CCChooseTroopScene:public cocos2d::CCScene
{
public:
	CREATE_FUNC(CCChooseTroopScene);
	virtual bool init();
public:
	CCChooseTroopLayer* m_pChooseTroopLayer;
};