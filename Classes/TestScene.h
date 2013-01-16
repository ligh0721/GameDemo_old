class CCTestLayer:public cocos2d::CCLayer
{
public:
	CREATE_FUNC(CCTestLayer);
	virtual bool init();
	static CCScene* scene();
};