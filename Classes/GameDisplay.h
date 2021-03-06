#pragma once


class CGameControl;

class CSkillInfo;
class CActiveSkill;
class CAttackData;


class CCCtrlLayer;

class CCSkillUpdateScene;

class CProp;

class CCProjectileWithAttackData
{
public:
    CCProjectileWithAttackData();
    virtual ~CCProjectileWithAttackData() = 0;

    virtual CAttackData* getAttackData() const;
    virtual void setAttackData(CAttackData* pAttackData);

    M_SYNTHESIZE(int, m_iGeneration, Generation);

public:
    CAttackData* m_pAttackData;
};

class CSkill;

class CCSkillManager : public CCObject
{
public:
    CCSkillManager();
    virtual ~CCSkillManager();

    virtual bool init();
    CREATE_FUNC_PARAM(CCSkillManager, ());

    static CCSkillManager* sharedSkillManager();
    virtual void cleanUp();

    int addSkill(CSkill* pSkill);
    CSkill* getSkill(int iKey);
    CSkill* copySkill(int iKey);
    CSkill* copySkill(CSkill* pSkill); // unsafe

public:
    static CCSkillManager* m_pInst;
    CCArray m_oArrSkill;
};
  
class CCSkillInfoManager : public CCObject
{
public:
    CCSkillInfoManager();
    virtual ~CCSkillInfoManager();

    virtual bool init();
    CREATE_FUNC_PARAM(CCSkillInfoManager, ());

    static CCSkillInfoManager* sharedSkillInfoManager();
    virtual void cleanUp();

    void addSkillInfo(CSkillInfo* pSkillInfo);
    CSkillInfo* getSkillInfo(int iKey);

public:
    static CCSkillInfoManager* m_pInst;
    CCArray m_oArrSkillInfo;
};

template <typename LOGIC>
inline LOGIC* CreateLogicObject()
{
    return new LOGIC();
}

class CCSkillButtonBase : public CCMenuItemImage
{
protected:
    CCSkillButtonBase();
    virtual ~CCSkillButtonBase();

public:
    virtual bool init(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished);
    //CREATE_FUNC_PARAM(CCSkillButtonBase, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    virtual void setCoolDown(float fDuration) = 0;
    virtual float getCoolDown() const = 0;
    void setLabel(const char* pLabel);
    CC_SYNTHESIZE(int, m_iClickRetCode, ClickRetCode);

    virtual void coolDown(float fFromPercent = 0);
    float getPercentage() const;

private:
    void onClick(CCObject* pObject);
    void onCoolDownDone(CCNode* pNode);
    void onCDBlickDone(CCNode* pNode);

protected:
    CCObject* m_pTarget;
    SEL_CallFuncN m_pOnClick;
    SEL_CallFuncN m_pOnFinished;
    CCProgressTimer* m_pPt;
    CCFiniteTimeAction* m_pAct;
    CCSprite* m_pBlink;
    CCSprite* m_pMask;
    //CCLabelTTF* m_pLabel;
public:
    const char* m_pNormalImageFrameName;
    const char* m_pSelectImageFrameName;
    const char* m_pDisableImageFrameName;
	
};

class CCSkillButtonNormal : public CCSkillButtonBase
{
public:
    CREATE_FUNC_PARAM(CCSkillButtonNormal, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    CC_SYNTHESIZE(float, m_fCoolDown, CoolDown);
	CCSkillButtonNormal* copyImg(CCObject* target, SEL_CallFuncN callOnClick, SEL_CallFuncN callOnFinished);
};

class CCCommmButton : public CCSkillButtonNormal
{
public:
	virtual bool init (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iKey);
	CREATE_FUNC_PARAM(CCCommmButton, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iKey), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished, iKey);

public:
	int m_iKey;

};
class CCGameChapterButton : public CCSkillButtonNormal
{
public:
	 virtual bool init (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iLevel);
	 CREATE_FUNC_PARAM(CCGameChapterButton, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iLevel), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished, iLevel);
public:
	int m_iLevel;

};
class CCSkillButtonNormalWithInfo : public CCSkillButtonNormal
{
public:
    virtual bool init(const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iInfoKey);
    CREATE_FUNC_PARAM(CCSkillButtonNormalWithInfo, (const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iInfoKey), pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished, iInfoKey);
    CC_SYNTHESIZE(int, m_iInfoKey, InfoKey);
};

class CCUnitLayer;

class CCSkillButtonAdvance : public CCSkillButtonBase
{
public:
    CCSkillButtonAdvance();
    virtual ~CCSkillButtonAdvance();

    virtual bool init(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, int iUnitKey, int iSkillKey, CCUnitLayer* pUnitLayer);
    CREATE_FUNC_PARAM(CCSkillButtonAdvance, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, int iUnitKey, int iSkillKey, CCUnitLayer* pUnitLayer), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, iUnitKey, iSkillKey, pUnitLayer);
    virtual void setCoolDown(float fDuration);
    virtual float getCoolDown() const;

    virtual void onSkillClick(CCNode* pNode);
    virtual void onSkillFinished(CCNode* pNode);
    M_SYNTHESIZE(CCUnitLayer*, m_pUnitLayer, UnitLayer);
    virtual void setPressed(CCSpriteFrame* pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(M_SKILL_PATH("cancel")));
    virtual bool isPressed() const;

    virtual void onExit();

public:
    int m_iUnitKey;
    int m_iSkillKey;
    bool m_bPressed;
};

class CCButtonPanel : public CCNode
{
public:
    enum ADD_HORIZONTAL
    {
        kLeftToRight,
        kRightToLeft
    };

    enum ADD_VERTICAL
    {
        kBottomToTop,
        kTopToBottom
    };

    enum ACTION_CMD
    {
        kUnknown,
        kAdd,
        kDel,
        kMove,
        kAddEx,
        kClearUp
    };

    static const float CONST_ACTION_DURATION;

    struct ACTION_NODE
    {
        ACTION_NODE() { memset(this, 0, sizeof(ACTION_NODE)); }
        ACTION_NODE(CCSkillButtonBase* pBtn, int iIndex): eAct(kAdd) { stAdd.pBtn = pBtn; stAdd.iIndex = iIndex; }
        ACTION_NODE(int iIndex, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor, bool bClearUp): eAct(kDel) { stDel.pBtn = NULL; stDel.iIndex = iIndex; stDel.eVer = eVer; stDel.eHor = eHor; stDel.bClearUp = bClearUp; }
        ACTION_NODE(int iIndexSrc, int iIndexDst): eAct(kMove) { stMove.pBtn = NULL; stMove.iIndexSrc = iIndexSrc; stMove.iIndexDst = iIndexDst; }
        ACTION_NODE(CCSkillButtonBase* pBtn, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor): eAct(kAddEx) { stAddEx.pBtn = pBtn; stAddEx.eVer = eVer; stAddEx.eHor = eHor; }
        ACTION_NODE(ADD_VERTICAL eVer, ADD_HORIZONTAL eHor): eAct(kClearUp) { stClearUp.eVer = eVer; stClearUp.eHor = eHor; }
        //~ACTION_NODE() { if (eAct == kAdd) {CC_SAFE_RELEASE(stAdd.pBtn); } else if (eAct == kAddEx) {CCLOG("release");CC_SAFE_RELEASE(stAddEx.pBtn);} }

        ACTION_CMD eAct;
        union
        {
            struct // ADD
            {
                CCSkillButtonBase* pBtn;
                int iIndex;
            } stAdd;

            struct // DEL
            {
                CCSkillButtonBase* pBtn;
                int iIndex;
                ADD_VERTICAL eVer;
                ADD_HORIZONTAL eHor;
                bool bClearUp;
            } stDel;

            struct // MOVE
            {
                CCSkillButtonBase* pBtn;
                int iIndexSrc;
                int iIndexDst;
            } stMove;

            struct // ADDEX
            {
                CCSkillButtonBase* pBtn;
                ADD_VERTICAL eVer;
                ADD_HORIZONTAL eHor;
            } stAddEx;

            struct // CLEARUP
            {
                ADD_VERTICAL eVer;
                ADD_HORIZONTAL eHor;
            } stClearUp;
        };
    };

    typedef list<ACTION_NODE> LIST_ACTION;

public:
    CCButtonPanel();
    virtual ~CCButtonPanel();

    virtual bool init(int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName);
    CREATE_FUNC_PARAM(CCButtonPanel, (int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName), iRow, iLine, fButtonWidth, fBorderWidth, fInnerBorderWidth, pBackgroundFrameName);

    void addButton(CCSkillButtonBase* pButton, int iIndex); // org
    void addButton(CCSkillButtonBase* pButton, int iX, int iY);
    
    void delButton(int iIndex); // org
    void delButton(int iX, int iY);
    void delButton(CCSkillButtonBase* pButton);

    void moveButton(int iIndexSrc, int iIndexDst); // org

    void addButtonEx(CCSkillButtonBase* pButton, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    int allotSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);
    int allotSlot(int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor);
    void clearUpSlot(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    void retainButton(CCSkillButtonBase* pButton);
    CCSkillButtonBase* getRetainButton() const;

    CCSkillButtonBase* getButton(int iX, int iY) const;
    CCSkillButtonBase* getButton(int iIndex) const;

    int getButtonIndex(CCSkillButtonBase* pButton) const;

    int index2Y(int iIndex) const;
    int index2X(int iIndex) const;
    int toIndex(int iX, int iY) const;
    CCPoint index2Point(int iIndex);

    bool isFull();

    void pushAction(const ACTION_NODE& roAct);
    void onPrevActEnd(CCNode* pNode);

    void pushAddButtonAction(CCSkillButtonBase* pButton, int iIndex);
    void pushDelButtonAction(int iIndex, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight, bool bClearUp = true);
    void pushMoveButtonAction(int iIndexSrc, int iIndexDst);
    void pushAddButtonExAction(CCSkillButtonBase* pButton, ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);
    void pushClearUpSlotAction(ADD_VERTICAL eVer = kBottomToTop, ADD_HORIZONTAL eHor = kLeftToRight);

    int getMaxCount() const;
    int getCount() const;

public:
    CC_SYNTHESIZE(int, m_iRow, Row);
    CC_SYNTHESIZE(int, m_iLine, Line);
    CC_SYNTHESIZE(float, m_fButtonWidth, ButtonWidth);
    CC_SYNTHESIZE(float, m_fInnerBorderWidth, InnerBorderWidth);
    CC_SYNTHESIZE(float, m_fBorderWidth, BorderWidth);
    CC_SYNTHESIZE(CCMenu*, m_pSkillMenu, SkillMenu);
    CC_SYNTHESIZE(CCSprite*, m_pBackground, Background);

public:
    int m_iOwnerKey;
    CCSkillButtonBase** m_ppBtnPos;
    CCSkillButtonBase* m_pRetain;
    LIST_ACTION m_lstActs;
    int m_iCount;
};

class CCProgressBar : public CCNode
{
public:
    static const float CONST_MAX_PROCESS_BAR_PERCENT;

public:
    CCProgressBar();
    virtual ~CCProgressBar();

    virtual bool init(const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop);
    CREATE_FUNC_PARAM(CCProgressBar, (const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop), roSize, pFill, pBorder, fHorizBorderWidth, fVertBorderWidth, bFillOnTop);

    virtual void setPercentage(float fPercent);
    virtual void setPercentage(float fPercent, float fDuration, CCFiniteTimeAction* pEndAction = NULL);
    virtual void setFillColor(const ccColor3B& roColor);

public:
    CCProgressTimer m_oPt;
};

class CCPackagePanel : public CCNode
{

public:
	virtual bool init(int iRow, int iLine, float fButtonWidth, float BorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName);
	CREATE_FUNC_PARAM(CCPackagePanel, (int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName), iRow, iLine, fButtonWidth, fBorderWidth, fInnerBorderWidth, pBackgroundFrameName);

	virtual void addProp(CCMenuItem* pItem, int iX, int iY);
	virtual void delProp(CCMenuItem* pItem);

public:
	CC_SYNTHESIZE(int, m_iRow, Row);
	CC_SYNTHESIZE(int, m_iLine, Line);
	CC_SYNTHESIZE(float, m_fButtonWidth, ButtonWidth);
	CC_SYNTHESIZE(float, m_fInnerBorderWidth, InnerBorderWidth);
	CC_SYNTHESIZE(float, m_fBorderWidth, BorderWidth);
	CC_SYNTHESIZE(CCMenu*, m_pPropMenu, PropMenu);
	CC_SYNTHESIZE(CCSprite*, m_pBackground, Background);

public:
	int m_iOwnerKey;

};

class CPropManage : public CCObject
{
public:
	CPropManage();
	virtual ~CPropManage();
	CREATE_FUNC_PARAM(CPropManage,());

	virtual bool init();
	CPropManage* sharePropManage();

	void addProp(CProp* pProp);
	void delProp(CProp* pProp);

private :
	CPropManage* m_pInst;
	CCArray m_oarrProp;
};

class CCPropItemImg : public CCSkillButtonBase
{
public:
	CCPropItemImg();
	virtual ~CCPropItemImg();

	virtual bool init(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, int iPropKey);
	CREATE_FUNC_PARAM(CCPropItemImg, (const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, int iPropKey), pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, iPropKey);

	virtual void setCoolDown(float fDuration);
	virtual float getCoolDown() const;

	virtual void onPropItemClick(CCNode* pNode);
	virtual void onPropItemFinished(CCNode* pNode);

public:
	int m_iPropKey;
};
