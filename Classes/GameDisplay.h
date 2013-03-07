#pragma once

#include "GameLogic.h"

class CEventInterface;
class CGameLogicObject;
class CGameControl;
class CBullet;

class CSkillInfo;
class CActiveSkill;
class CAttackData;

class CCGameManager;
class CCBulletSprite;
class CCTankSprite;
class CCNormalTankSprite;

class CCBattleGroundLayer;
class CCCtrlLayer;
class CCBackGroundLayer;

class CCBattleGroundScene;
class CCSkillUpdateScene;

class CCShadowNode;

class CProp;
class CCEventTriggerInterface
{
public:
    virtual ~CCEventTriggerInterface() = 0;

    virtual CEventInterface* getEventInterface() const = 0;
    virtual bool isTriggerOn() const = 0;
};

class CCLogicInfoInterface
{
public:
    CCLogicInfoInterface();
    virtual ~CCLogicInfoInterface() = 0;

    virtual CGameLogicObject* getLogicBody() const;
    virtual void setLogicBody(CGameLogicObject* pLogicBody);

public:
    CGameLogicObject* m_pLogicBody;
};

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

class CCGameSprite : public CCSprite
{
public:
    CCGameSprite();
    virtual ~CCGameSprite();

    virtual void setPosition(const CCPoint& roPos);

    static float reviseRotation(float fDegree);

    CCShadowNode* getShadowNode();
    void setShadowNode(CCShadowNode* pShadowNode);
    void delShadowNode();

    CC_SYNTHESIZE(float, m_fLineVelocity, LineVelocity);
    CC_SYNTHESIZE(float, m_fAngularVelocity, AngularVelocity);

    virtual void startDoing(uint32_t dwMask);
    virtual void endDoing(uint32_t dwMask);
    virtual bool isDoingOr(uint32_t dwMask) const;
    virtual bool isDoingAnd(uint32_t dwMask) const;
    virtual bool isDoingNothing() const;

public:
    uint32_t m_dwDoingFlags;
private:
    CCShadowNode* m_pShadowNode;
};

class CCShadowNode : public CCNode
{
public:
    virtual ~CCShadowNode();

    virtual bool init(CCNode* pParent);
    CREATE_FUNC_PARAM(CCShadowNode, (CCNode* pParent), pParent);

    virtual CCGameSprite* getOwner();
    virtual void setOwner(CCGameSprite* pOwner);

public:
    CCGameSprite* m_pOwner;
};

class CCRadarStreak : public CCMotionStreak
{
public:
    CCRadarStreak();

    static CCRadarStreak* create(float fade, float minSeg, float stroke, ccColor3B color, const char* path, CCSprite* pSprite);
    virtual void updateStreak(float dt);

private:
    CCSprite* m_pSprite;
    float m_fRadius;
    float m_fAngle;
};

class CCGameManager : public CCObject
{
protected:
    CCGameManager();

public:
    virtual ~CCGameManager();

    virtual bool init();
    CREATE_FUNC_PARAM(CCGameManager, ());
    static CCGameManager* sharedGameManager();

    void addBattleGroundLayer(CCScene* pTargetSence);
    void addCtrlLayer(CCScene* pTargetSence);
    void addBackGroundLayer(CCScene* pTargetSence);

    void addTank(CCTankSprite* pTank, const CCPoint& oPos, float fRotate);
    void addBullet(CCBulletSprite* pBullet, CCTankSprite* pTank);
    int getBulletCount() const;
    int getTankCount() const;
    //int getAllyCount() const;
    //int getEnemyCount() const;
    CCBulletSprite* getBulletByIndex(int iIndex);
    CCTankSprite* getTankByIndex(int iIndex);
    CCTankSprite* getTankByKey(int iKey);
    CSkill* getSkillByKey(int iOwner, int iKey);
    CBuffSkill* getBuffByKey(int iOwner, int iKey);

    virtual CGameControl* getLogicBody() const;

    void setVoice(bool bTurnOn);
    bool isVoiceEnabled() const;
    void preloadEffectSound(const char* pEffect);
    void preloadBackgroundSound(const char* pBackground);
    void playEffectSound(const char* pEffect, bool bLoop = false);
    void playBackgroundSound(const char* pBackground, bool bLoop = true);

	void setPackage(CUnitPackage* pPackage);

	void pushScene(CCScene* pScene);
	void popSceneWithTrans(float duration);
public:
    static CCGameManager* m_pInst;
    CGameControl* m_pLogicBody;
    CCBattleGroundLayer* m_pBattleGroundLayer;
    CCCtrlLayer* m_pCtrlLayer;
    CCBackGroundLayer* m_pBackGroundLayer;
    CCSpriteBatchNode* m_pTankBatch;
    CCSpriteBatchNode* m_pBulletBatch;
    CCParticleBatchNode* m_pParticleBatch;
    b2World* m_pPhyWorld;
    SimpleAudioEngine* m_pAudio;
    int m_iPlayerKey;
    bool m_bTurnOnVoice;
    float m_fMaxBonusDist;
	CUnitPackage* m_pPackage;
	CCArray m_oArrScene;
	int m_iTotalChapter;
	int m_iCompletedChapter;
};

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

template <typename BULLETSPRITE>
inline BULLETSPRITE* CreateBulletSprite(const char* pFrameName, CBullet* pBulletLogic, CCTankSprite* pOwnerSprite)
{
    BULLETSPRITE* pBulletSprite = BULLETSPRITE::createWithSpriteFrameNameAndLogic(pFrameName, pBulletLogic);
    CCGameManager::sharedGameManager()->addBullet(pBulletSprite, pOwnerSprite);
    return pBulletSprite;
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

    virtual void coolDown();

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

public:
    int m_iUnitKey;
    int m_iSkillKey;
    bool m_bPressed;
};

class CCButtonPanel : public CCNode
{
public:
    virtual bool init(int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName);
    CREATE_FUNC_PARAM(CCButtonPanel, (int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName), iRow, iLine, fButtonWidth, fBorderWidth, fInnerBorderWidth, pBackgroundFrameName);

    virtual void addButton(CCSkillButtonBase* pButton, int iX, int iY);
    virtual void delButton(CCSkillButtonBase* pButton);

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
