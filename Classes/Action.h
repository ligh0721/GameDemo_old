#pragma once


class CCCoverAct : public CCAction
{
public:
    virtual ~CCCoverAct();

    virtual bool init(float fDuration);
    CREATE_FUNC_PARAM(CCCoverAct, (float fDuration), fDuration);

    virtual void startWithTarget(CCNode *pTarget);
    virtual void endEffect(CCNode* pNode);

public:
    float m_fDuration;
    CCSprite* m_pSprite;
};

class CCShakeAct : public CCActionInterval
{
public:
    virtual bool init(float fDuration, int iTimes, float fRange);
    CREATE_FUNC_PARAM(CCShakeAct, (float fDuration, int iTimes, float fRange), fDuration, iTimes, fRange);

    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);

    virtual void effectEnd(CCNode* pNode);


public:
    int m_iTimes;
    float m_fRange;
    CCPoint m_oLoc;
};

class CCJumpByEx : public CCAction
{
public:
    virtual bool init(const CCPoint& roPosition, const CCPoint& roDelta, const ccColor3B& roColor);
    CREATE_FUNC_PARAM(CCJumpByEx, (const CCPoint& roPosition, const CCPoint& roDelta, const ccColor3B& roColor), roPosition, roDelta, roColor);

    virtual void startWithTarget(CCNode *pTarget);

public:
    CCPoint m_oPosition;
    CCPoint m_oDelta;
    ccColor3B m_oColor;
};

class CCFlash : public CCAction
{
public:
    virtual ~CCFlash();

    virtual bool init(float fDuration, CCSprite* pSprite, float fRotation);
    CREATE_FUNC_PARAM(CCFlash, (float fDuration, CCSprite* pSprite, float fRotation), fDuration, pSprite, fRotation);

    virtual void startWithTarget(CCNode *pTarget);
    virtual void endEffect(CCNode* pNode);

public:
    float m_fDuration;
    CCSprite* m_pSprite;
    float m_fRotation;
};

class CCSelected : public CCAction
{
public:
    virtual bool init(CCNode* pNode);
    CREATE_FUNC_PARAM(CCSelected, (CCNode* pNode), pNode);

    virtual void startWithTarget(CCNode *pTarget);
    virtual void endEffect(CCNode* pNode);

public:
    CCNode* m_pNode;
};

class CCMoveToNode : public CCActionInterval
{
public:
    virtual ~CCMoveToNode();
    virtual bool init(float fDuration, CCNode* pNode, bool bFixRotation = false, float fMaxOffsetY = 0.0, float fScaleY = 1.0, float fYOffsetOfAnchor = 0.0);
    CREATE_FUNC_PARAM(CCMoveToNode, (float fDuration, CCNode* pNode, bool bFixRotation = false, float fMaxOffsetY = 0.0, float fScaleY = 1.0, float fOffsetYOfAnchor = 0.0), fDuration, pNode, bFixRotation, fMaxOffsetY, fScaleY, fOffsetYOfAnchor);
    virtual void startWithTarget(CCNode *pTarget);

    virtual void update(float time);
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    CCPoint m_oEndPos;
    CCPoint m_oDeltaPos;
    CCNode* m_pEndNode;
    CCPoint m_oStartPos;
    float m_fMinSpeed;
    float m_fOffsetYOfAnchor;
    float m_fScaleY;
    float m_fMaxOffsetY;
    float m_fA;
    bool m_bFixRotation;
};

class CCLightning : public CCAnimate
{
public:
    virtual ~CCLightning();
    virtual bool init(CCAnimation* pAnimation, CCNode* pStartNode, CCNode* pEndNode, const CCPoint& roStartNodeOffsetOfAnchor = CCPointZero, float fEndNodeYOffsetOfAnchor = 0.0);
    CREATE_FUNC_PARAM(CCLightning, (CCAnimation *pAnimation, CCNode* pStartNode, CCNode* pEndNode, const CCPoint& roStartNodeOffsetOfAnchor = CCPointZero, float fEndNodeYOffsetOfAnchor = 0.0), pAnimation, pStartNode, pEndNode, roStartNodeOffsetOfAnchor, fEndNodeYOffsetOfAnchor);

    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float t);

    virtual void fixTargetPosition(CCNode* pTarget);

protected:
    CCNode* m_pStartNode;
    CCNode* m_pEndNode;
    CCPoint m_oStartNodeOffsetOfAnchor;
    float m_fEndNodeOffsetOfAnchor;
};

class CCSequenceEx : public CCSequence
{
public:
    static CCSequenceEx* createWithTwoActions(CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo);
    virtual CCFiniteTimeAction* getActionOne();
    virtual CCFiniteTimeAction* getActionTwo();
};

class CCMoveToEx : public CCMoveTo
{
public:
    virtual bool initWithDuration(float fDuration, const CCPoint& roPos, float fMaxOffsetY = 0.0, bool bFixRotation = true, float fScaleY = 1.0, float fYOffsetOfAnchor = 0.0);
    CREATE_INITWITH_FUNC_PARAM(Duration, CCMoveToEx, (float fDuration, const CCPoint& roPos, float fMaxOffsetY = 0.0, bool bFixRotation = true, float fScaleY = 1.0, float fYOffsetOfAnchor = 0.0), fDuration, roPos, fMaxOffsetY, bFixRotation, fScaleY, fYOffsetOfAnchor);

    virtual void update(float time);
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    float m_fYOffsetOfAnchor;
    float m_fScaleY;
    float m_fMaxOffsetY;
    bool m_bFixRotation;
};

class CCDelayRelease : public CCSequence
{
public:
    static const float CONST_EX_DURATION;

public:
    virtual bool initWithDuration(float fDelay);
    CREATE_INITWITH_FUNC_PARAM(Duration, CCDelayRelease, (float fDelay), fDelay);

    void onActEnd(CCNode* pNode);

};

class CCReleaseAfter : public CCSequence
{
public:
    virtual bool initWithAction(CCFiniteTimeAction* fAct);
    CREATE_INITWITH_FUNC_PARAM(Action, CCReleaseAfter, (CCFiniteTimeAction* fAct), fAct);

    void onActEnd(CCNode* pNode);
    
};

class CCFadeInOutScale4 : public CCSequence
{
public:
    virtual bool init(float fScaleStart, float fScaleMid, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd);
    CREATE_FUNC_PARAM(CCFadeInOutScale4, (float fScaleStart, float fScaleMid, float fScaleEnd, float fDurToMid, float fDurToNormal, float fDurKeep, float fDurToEnd), fScaleStart, fScaleMid, fScaleEnd, fDurToMid, fDurToNormal, fDurKeep, fDurToEnd);

    virtual void startWithTarget(CCNode *pTarget);

protected:
    float m_fScaleStart;
};

