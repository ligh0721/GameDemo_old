class CCTechTreeLayer;
class CTechTreeLayer;
class CTalentInfo;

class CCTechTreeScene:public cocos2d::CCScene
{
public:
	CREATE_FUNC(CCTechTreeScene);
	virtual bool init();
public:
	CTechTreeLayer* m_pTechTreeLayer;
    CTalentInfo* m_pTalentInfo;
};


class CCTechTreeLayer:public CCLayer
{
public:
	CCTechTreeLayer();
	virtual bool init();
	static cocos2d::CCScene* scene();
	CREATE_FUNC(CCTechTreeLayer);
	void techDevelopCallback(CCObject* pSender);
	void showDetail();
	void learnCallback(CCObject* pSender);
	void cancelCallback(CCObject* pSender);
	void onBtnBackClick(CCObject* pSender);

	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
public:
	CCMenuItemFont techOneItem;
	CCMenuItemFont techTwoItem;
	int techArray[10];
	CCMenu pMenu;
	CCMenu pMenu2;
	CCMenuItemFont learnItem;
	CCMenuItemFont cancelItem;
	CCMenuItemImage m_oBackToGame;
	int flagStore;
	CCPoint zeroPoint;
	CCSize visibleSize;
};


class CTechTreeLayer:public CCLayer
{
private:
    CTechTreeLayer();
public:
    static CTechTreeLayer* singleTechTree();
    virtual bool init();

    CREATE_FUNC(CTechTreeLayer);
public:
 //   void onBtnChooseTechTreeClick(CCObject* pObject);
    void onBtnFirstTechNodeClick(CCObject* pObject);
    void onBtnLearnClick(CCObject *pObject);
    void onBtnCancelClick(CCObject *pObject);
    void onBtnBackClick(CCObject *pObject);
protected:
    //CCMenuItemFont m_oFirstTechTree;
    CCMenu m_oMenuMainTechTree;
    int m_nodeLine[7];
    CCMenuItemImage m_oFirstTechNode1;
    CCMenuItemImage m_oFirstTechNode2;
    CCMenuItemImage m_oFirstTechNode3;
    CCMenuItemImage m_oFirstTechNode4;
    CCMenuItemImage m_oFirstTechNode5;
    CCMenuItemImage m_oFirstTechNode6;
   // CCMenu m_oMenuFirstTechTree;
    CCMenuItemImage m_oBtnBack;
    CCMenuItemFont m_oButtonLearn;
    CCMenuItemFont m_oButtonCancel;

   // CCButtonPanel
};

// class CTalentLine
// {
// public:
//     struct TALENT_NODE
//     {
//         TALENT_NODE(int iCost_) : iCost(iCost_), bLearned(false) {}
//         int iCost;
//         bool bLearned;
//     };
//     typedef vector<TALENT_NODE> VEC_TALENT_NODES;
// 
// public:
//     int addNode(int iCost);
//     int getNodeCount() const;
//     bool canStudy(int iGold) const; //  
//     bool study();
//     void reset();
// 
// public:
//     VEC_TALENT_NODES m_vecNodes;
// };

// class CTalentManager
// {
// public:
//     
//    
//     typedef vector<CTalentLine> VEC_TALENTS;
// 
//     int getCurTalentNode(int iTalentType) const;
//     int getNextTalentNode(int iTalentType) const;
//     int getCost(int iTalentType, int iLevel);
//     bool canLearn(int iTalentType, int iLevel);
//     bool learn(int iTalentType, int iLevel);
// 
//     int addNewTalentType();
//     
// };

class CTalentInfo : public CCLayer
{
public:
    struct TALENT_NODE
    {
        int iUnitIndex;
        int iEffectIndex;
        int iCost;
        int iStatus;
        char* sInfo;
        char* pNodeImage;
        TALENT_NODE();
    };
    typedef vector<TALENT_NODE> TALENT_TREE;
    typedef vector<TALENT_TREE> TALENT_FOREST;
public:
    CREATE_FUNC(CTalentInfo);
    static CTalentInfo* singleTalentInfo();
    virtual bool init();

    void addTalentTree(TALENT_TREE newTalent);
    void addTalentNode(int treeNumber,TALENT_NODE newNode);

    void onTalentNodeClick(CCObject* pObject);

    void onItemOkClick(CCObject* pObject);

    void onItemResetClick(CCObject* pObject);

    void onItemLearnClick(CCObject* pObject);

    void setSkillPoint(int skillPoint);

    void setSkillPointShow();

    void TalentDemo();
public:
    TALENT_FOREST m_talentForest;
    CCMenu m_menu;
    CCMenuItemImage m_itemOk;
    CCMenuItemImage m_itemReset;
    CCMenuItemImage m_itemLearn;
    CCLabelTTF m_talentInfo;
    CCLabelTTF m_skillPointShow;
    int m_iSkillPoint;
    int m_iFocusLine;
    int m_iFocusNode;
    float m_spaceBottom;
    float m_spaceLeft;
    float m_spaceLine;
    map<int,int> m_retPair;
    //
    CCArray m_oArrTalentNode;
};