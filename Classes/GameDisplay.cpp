#include "CommInc.h"

#include "GameDisplay.h"
#include "Skill.h"
#include "SkillUI.h"
#include "GameCtrl.h"


CCProjectileWithAttackData::CCProjectileWithAttackData()
    : m_pAttackData(NULL)
    , m_iGeneration(1)
{
}

CCProjectileWithAttackData::~CCProjectileWithAttackData()
{
    CC_SAFE_RELEASE(m_pAttackData);
}

CAttackData* CCProjectileWithAttackData::getAttackData() const
{
    return m_pAttackData;
}

void CCProjectileWithAttackData::setAttackData( CAttackData* pAttackData )
{
    if (pAttackData != m_pAttackData)
    {
        CC_SAFE_RELEASE(m_pAttackData);
    }

    m_pAttackData = pAttackData;
    CC_SAFE_RETAIN(m_pAttackData);
}

CCSkillManager::CCSkillManager()
{
}

CCSkillManager::~CCSkillManager()
{
}

bool CCSkillManager::init()
{
    m_oArrSkill.init();
    return true;
}

CCSkillManager* CCSkillManager::m_pInst = NULL;

CCSkillManager* CCSkillManager::sharedSkillManager()
{
    if (m_pInst)
    {
        return m_pInst;
    }

    m_pInst = CCSkillManager::create();
    CC_SAFE_RETAIN(m_pInst);
    return m_pInst;
}

void CCSkillManager::cleanUp()
{
    m_oArrSkill.removeAllObjects();
}

int CCSkillManager::addSkill( CSkill* pSkill )
{
    m_oArrSkill.addObject(pSkill);
    return pSkill->getKey();
}

CSkill* CCSkillManager::getSkill( int iKey )
{
    CSkill* pSkill = NULL;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrSkill, pObj)
    {
        pSkill = dynamic_cast<CSkill*>(pObj);
        if (pSkill->getKey() == iKey)
        {
            return pSkill;
        }
    }
    return NULL;
}

CSkill* CCSkillManager::copySkill( int iKey )
{
    CSkill* pSkill = getSkill(iKey);
    if (!pSkill)
    {
        return NULL;
    }
    return dynamic_cast<CSkill*>(pSkill->copy());
}

CSkill* CCSkillManager::copySkill( CSkill* pSkill )
{
    return dynamic_cast<CSkill*>(pSkill->copy());
}

CCSkillInfoManager* CCSkillInfoManager::m_pInst = NULL;

CCSkillInfoManager::CCSkillInfoManager()
{
}

CCSkillInfoManager::~CCSkillInfoManager()
{
}

bool CCSkillInfoManager::init()
{
    m_oArrSkillInfo.init();
    return true;
}

CCSkillInfoManager* CCSkillInfoManager::sharedSkillInfoManager()
{
    if (m_pInst)
    {
        return m_pInst;
    }

    m_pInst = CCSkillInfoManager::create();
    CC_SAFE_RETAIN(m_pInst);
    return m_pInst;
}

void CCSkillInfoManager::cleanUp()
{
    m_oArrSkillInfo.removeAllObjects();
}

void CCSkillInfoManager::addSkillInfo( CSkillInfo* pSkillInfo )
{
    m_oArrSkillInfo.addObject(pSkillInfo);
}

CSkillInfo* CCSkillInfoManager::getSkillInfo( int iKey )
{
    CSkillInfo* pSkillInfo = NULL;
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(&m_oArrSkillInfo, pObj)
    {
        pSkillInfo = dynamic_cast<CSkillInfo*>(pObj);
        if (pSkillInfo->getKey() == iKey)
        {
            return pSkillInfo;
        }
    }
    return NULL;
}

CCSkillButtonBase::CCSkillButtonBase()
    : m_pTarget(NULL)
    , m_iClickRetCode(0)
{
}

CCSkillButtonBase::~CCSkillButtonBase()
{

}

void CCSkillButtonBase::onCDBlickDone(CCNode* pNode)
{
    if (m_pBlink)
    {
        m_pBlink->setVisible(false);
    }
}

bool CCSkillButtonBase::init(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished)
{
	CCSprite* pNormalSprite = NULL;
	CCSprite* pSelectedSprite = NULL;
	CCSprite* pDisabledSprite = NULL;
	m_pDisableImageFrameName = pDisabledImage;
	m_pNormalImageFrameName = pNormalImage;
	m_pSelectImageFrameName = pSelectedImage;
	
	pNormalImage && (pNormalSprite = CCSprite::createWithSpriteFrameName(pNormalImage));
	pSelectedImage && (pSelectedSprite = CCSprite::createWithSpriteFrameName(pSelectedImage));
	pDisabledImage && (pDisabledSprite = CCSprite::createWithSpriteFrameName(pDisabledImage));
	if (!initWithNormalSprite(pNormalSprite, pSelectedSprite, pDisabledSprite, this, menu_selector(CCSkillButtonBase::onClick)))
	{
		return false;
	}
    m_pTarget = pTarget;
    m_pOnClick = pOnClick;
    m_pOnFinished = pOnFinished;
    setCoolDown(fCoolDown);
    CCSize sz = getContentSize();
    CCPoint oAp = getAnchorPointInPoints();

    // Create ProgressTimer
    m_pPt = CCProgressTimer::create((CCSprite*)getNormalImage());
    addChild(m_pPt);
    m_pPt->setPosition(oAp);
    m_pPt->setVisible(false);
    m_pPt->setType(kCCProgressTimerTypeRadial);

    // Create Blink Image
    if (pBlinkImage)
    {
        m_pBlink = CCSprite::createWithSpriteFrameName(pBlinkImage);
        addChild(m_pBlink, 10);
        m_pBlink->setPosition(oAp);
        m_pBlink->setVisible(false);
    }
    else
    {
        m_pBlink = NULL;
    }

    if (pMaskImage)
    {
        m_pMask = CCSprite::createWithSpriteFrameName(pMaskImage);
        addChild(m_pMask);
        m_pMask->setPosition(oAp);
        m_pMask->setVisible(false);
    }
    else
    {
        m_pMask = NULL;
    }

    // Create Label
    //m_pLabel = CCLabelTTF::create("5", "", 24, CCSizeMake(sz.width - 8, 32), kCCTextAlignmentRight, kCCVerticalTextAlignmentBottom); //CCLabelBMFont::create("88", "fonts/futura-48.fnt", sz.width - 6, kCCTextAlignmentRight);//, CCSizeMake(getContentSize().width - 6, 16.0), kCCTextAlignmentRight, "", 16.0
    //addChild(m_pLabel);
    //CCSize szLbl = m_pLabel->getContentSize();
    //m_pLabel->setPosition(ccp(sz.width - szLbl.width / 2 - 4, szLbl.height / 2));
    //m_pLabel->setColor(ccRED);

    return true;
}

void CCSkillButtonBase::onClick(CCObject* pObject)
{
    setClickRetCode(0);
    if (m_pTarget && m_pOnClick)
    {
        (m_pTarget->*m_pOnClick)(this);
    }
    if (getClickRetCode() < 0)
    {
        return;
    }

}

void CCSkillButtonBase::onCoolDownDone(CCNode* pNode)
{
    if (getCoolDown())
    {
        m_pMask->setVisible(false);
        m_pPt->setVisible(false);
        this->setEnabled(true);
        if (m_pBlink)
        {
            m_pBlink->setVisible(true);
            m_pBlink->runAction(CCSequence::create(CCFadeIn::create(0.25f), CCFadeOut::create(0.25f), CCCallFuncN::create(this, callfuncN_selector(CCSkillButtonBase::onCDBlickDone)), NULL));
        }
    }

    if (m_pTarget && m_pOnFinished)
    {
        (m_pTarget->*m_pOnFinished)(this);
    }
}

void CCSkillButtonBase::coolDown(float fFromPercent)
{
    if (getCoolDown())
    {
        float fCoolDownReal = getCoolDown() * (100.0 - fFromPercent) / 100.0;
        m_pMask->setVisible(true);
        m_pPt->setVisible(true);
        this->setEnabled(false);
        dynamic_cast<CCSprite*>(m_pDisabledImage)->setOpacity(0x50 + (0xFF - 0x50) * fFromPercent / 100.0);
        m_pDisabledImage->runAction(CCFadeTo::create(fCoolDownReal, 0xFF));
        
        m_pPt->setOpacity(0x7F + (0xFF - 0x7F) * fFromPercent / 100.0);
        CCProgressFromTo* pPro = CCProgressFromTo::create(fCoolDownReal, fFromPercent, 100.0f);
        
        //m_pPt->runAction();

        
        m_pPt->runAction(CCSpawn::createWithTwoActions(
            CCFadeTo::create(fCoolDownReal, 0xFF),
            CCSequence::create(pPro, CCCallFuncN::create(this, callfuncN_selector(CCSkillButtonBase::onCoolDownDone)), NULL)
            ));
    }
    else
    {
        onCoolDownDone(this);
    }
}

float CCSkillButtonBase::getPercentage() const
{
    return m_pPt->getPercentage();
}

bool CCSkillButtonNormalWithInfo::init( const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iInfoKey )
{
    setInfoKey(iInfoKey);
    CCSkillInfoManager* pSim = CCSkillInfoManager::sharedSkillInfoManager();
    const char* pBtnImg = pSim->getSkillInfo(m_iInfoKey)->getButtonImage();
    char szNormal[256];
    char szSelected[256];
    char szDisabled[256];
    sprintf(szNormal, "%s/BTN%s.png", pBtnImg, pBtnImg);
    sprintf(szSelected, "%s/BTN%sDOWN.png", pBtnImg, pBtnImg);
    sprintf(szDisabled, "%s/DISBTN%s.png", pBtnImg, pBtnImg);
    
    pSim->getSkillInfo(m_iInfoKey);
    CCSkillButtonNormal::init(szNormal, szSelected, szDisabled, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
    
    return true;
}

CCSkillButtonAdvance::CCSkillButtonAdvance()
    : m_iSkillKey(0)
{
}

CCSkillButtonAdvance::~CCSkillButtonAdvance()
{
}

bool CCSkillButtonAdvance::init(const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, int iUnitKey, int iSkillKey, CCUnitLayer* pUnitLayer )
{
    m_bPressed = false;
    setUnitLayer(pUnitLayer);
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(iUnitKey);
    if (!pUnit)
    {
        return false;
    }
    CSkill* pSkill = pUnit->getSkill(iSkillKey);
    if (!pSkill)
    {
        return false;
    }
    m_iUnitKey = iUnitKey;
    CCSkillButtonBase::init(pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, pSkill->getCoolDown(), this, callfuncN_selector(CCSkillButtonAdvance::onSkillClick), callfuncN_selector(CCSkillButtonAdvance::onSkillFinished));

    m_iSkillKey = iSkillKey;
    pSkill->setDisplayBody(this);
    return true;
}

void CCSkillButtonAdvance::setCoolDown( float fDuration )
{
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(m_iUnitKey);
    if (!pUnit)
    {
        return;
    }
    CSkill* pSkill = pUnit->getSkill(m_iSkillKey);
    if (!pSkill)
    {
        return;
    }
    pSkill->setCoolDown(fDuration);
}

float CCSkillButtonAdvance::getCoolDown() const
{
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(m_iUnitKey);
    if (!pUnit)
    {
        return -1;
    }
    CSkill* pSkill = pUnit->getSkill(m_iSkillKey);
    if (!pSkill)
    {
        return -1;
    }
    return pSkill->getCoolDown();
}

void CCSkillButtonAdvance::onSkillClick( CCNode* pNode )
{
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(m_iUnitKey);
    if (!pUnit || pUnit->isDead() || pUnit->isDoingOr(CGameUnit::kSuspended))
    {
        M_SKILL_BUTTON_CANCEL(pNode);
        return;
    }
    CSkill* pSkill = pUnit->getSkill(m_iSkillKey);
    if (!pSkill)
    {
        M_SKILL_BUTTON_CANCEL(pNode);
        return;
    }
    CActiveSkill* pAct = dynamic_cast<CActiveSkill*>(pSkill);
    if (!pAct)
    {
        M_SKILL_BUTTON_CANCEL(pNode);
        return;
    }
    M_DEF_FC(pFc);

    // 检查其他按钮，如果有按下状态，需进行复位
    CCArray* pBtns = getParent()->getChildren();
    CCObject* pObj = NULL;
    CCSkillButtonAdvance* pBtnAdv = NULL;
    CCARRAY_FOREACH(pBtns, pObj)
    {
        pBtnAdv = dynamic_cast<CCSkillButtonAdvance*>(pObj);
        if (pBtnAdv && pBtnAdv->isPressed() && pBtnAdv != this)
        {
            pBtnAdv->setPressed(NULL);
        }
    }

    switch (pAct->getCastTargetType())
    {
    case CActiveSkill::kPointTarget:
    case CActiveSkill::kUnitTarget:
        if (isPressed())
        {
            // 此次为取消操作
            getUnitLayer()->endOrderUnitToCast();
        }
        else
        {
            getUnitLayer()->preOrderUnitToCast(m_iUnitKey, m_iSkillKey);
            setPressed();
        }
        
        return;
    }

    pUnit->getUnitLayer()->preOrderUnitToCast(m_iUnitKey, m_iSkillKey);
    if (!pUnit->cast())
    {
        M_SKILL_BUTTON_CANCEL(pNode);
        getUnitLayer()->endOrderUnitToCast();
        return;
    }
    getUnitLayer()->endOrderUnitToCast();
}

void CCSkillButtonAdvance::onSkillFinished( CCNode* pNode )
{
    CGameUnit* pUnit = getUnitLayer()->getUnitByKey(m_iUnitKey);
    if (!pUnit)
    {
        return;
    }
    CSkill* pSkill = pUnit->getSkill(m_iSkillKey);
    if (!pSkill)
    {
        return;
    }
    if (pSkill->getOwner())
    {
        pSkill->setCoolDownLeft(0);
        pSkill->onSkillReady();
    }
}

void CCSkillButtonAdvance::setPressed( CCSpriteFrame* pFrame )
{
    if (pFrame)
    {
        setNormalSpriteFrame(pFrame);
    }
    else
    {
        M_DEF_FC(pFc);
        setNormalSpriteFrame(pFc->spriteFrameByName(m_pNormalImageFrameName));
    }
    m_bPressed = (pFrame != NULL);
}

bool CCSkillButtonAdvance::isPressed() const
{
    return m_bPressed;
}

void CCSkillButtonAdvance::onExit()
{
    if (isPressed())
    {
        getUnitLayer()->endOrderUnitToCast();
    }
    CCSkillButtonBase::onExit();
}

CCButtonPanel::CCButtonPanel()
    : m_ppBtnPos(NULL)
    , m_pRetain(NULL)
{
}

CCButtonPanel::~CCButtonPanel()
{
    if (m_ppBtnPos)
    {
        delete[] m_ppBtnPos;
    }
    CC_SAFE_RELEASE(m_pRetain);
}

const float CCButtonPanel::CONST_ACTION_DURATION = 0.25;

bool CCButtonPanel::init( int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName )
{
    m_iRow = iRow;
    m_iLine = iLine;
    m_fButtonWidth = fButtonWidth;
    m_fBorderWidth = fBorderWidth;
    m_fInnerBorderWidth = fInnerBorderWidth;
    if (pBackgroundFrameName)
    {
        m_pBackground = CCSprite::createWithSpriteFrameName(pBackgroundFrameName);
        addChild(m_pBackground);
        m_pBackground->setPosition(getAnchorPointInPoints());
    }
    else
    {
        m_pBackground = NULL;
    }
    
    CCSize oSz = CCSizeMake(m_fBorderWidth * 2 + m_fInnerBorderWidth * (m_iLine - 1) + m_fButtonWidth * m_iLine, m_fBorderWidth * 2 + m_fInnerBorderWidth * (m_iRow - 1) + m_fButtonWidth * m_iRow);
    setContentSize(oSz);
    
    m_iOwnerKey = 0;

    m_pSkillMenu = CCMenu::create();
    addChild(m_pSkillMenu);
    m_pSkillMenu->setContentSize(getContentSize());
    m_pSkillMenu->setPosition(CCPointZero);

    if (m_ppBtnPos)
    {
        delete[] m_ppBtnPos;
    }
    CC_SAFE_RELEASE(m_pRetain);

    size_t uCount = iRow * iLine;
    m_ppBtnPos = new CCSkillButtonBase*[uCount];
    memset(m_ppBtnPos, 0, sizeof(CCSkillButtonBase*) * uCount);

    m_iCount = 0;

    return true;
}

void CCButtonPanel::addButton( CCSkillButtonBase* pButton, int iIndex )
{
    CCAssert(iIndex < m_iRow * m_iLine, "Break Bounds");
    CCAssert(m_iCount <= m_iRow * m_iLine, "already full");
    CCSkillButtonBase* pBtn = getButton(iIndex);
    if (pBtn)
    {
        delButton(iIndex);
    }

    m_pSkillMenu->addChild(pButton);
    pButton->setPosition(index2Point(iIndex));

    m_ppBtnPos[iIndex] = pButton;

    ++m_iCount;
}

void CCButtonPanel::addButton( CCSkillButtonBase* pButton, int iX, int iY )
{
    CCAssert(iY < m_iRow && iX < m_iLine, "Break Bounds");
    addButton(pButton, toIndex(iX, iY));
}

void CCButtonPanel::delButton( int iIndex )
{
    --m_iCount;
    m_pSkillMenu->removeChild(m_ppBtnPos[iIndex], true);
    m_ppBtnPos[iIndex] = NULL;
}
void CCButtonPanel::delButton( CCSkillButtonBase* pButton )
{
    int iIndex = getButtonIndex(pButton);
    CCAssert(iIndex >= 0, "button not found");
    delButton(iIndex);
}

void CCButtonPanel::delButton(int iX, int iY)
{
    CCAssert(iY < m_iRow && iX < m_iLine, "Break Bounds");
    delButton(toIndex(iX, iY));
}


int CCButtonPanel::allotSlot( ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iStartY = bY ? 0 : (m_iRow - 1);
    int iStartX = bX ? 0 : (m_iLine - 1);

    int iEndY = bY ? (m_iRow - 1) : 0;
    int iEndX = bX ? (m_iLine - 1) : 0;

    return allotSlot(iStartX, iStartY, iEndX, iEndY, eVer, eHor);
}

int CCButtonPanel::allotSlot( int iStartX, int iStartY, int iEndX, int iEndY, ADD_VERTICAL eVer, ADD_HORIZONTAL eHor )
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iY = bY ? 1 : -1;
    int iX = bX ? 1 : -1;
    
    int iIndex;
    for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
    {
        int iStartX0 = (y == iStartY ? iStartX : (bX ? 0 : (m_iLine - 1)));
        int iEndX0 = (y == iEndY ? iEndX : (bX ? (m_iLine - 1) : 0));
        for (int x = iStartX0; bX ? (x <= iEndX0) : (x >= iEndX0); x += iX)
        {
            iIndex = toIndex(x, y);
            if (!getButton(iIndex))
            {
                return iIndex;
            }
        }
    }

    return -1;
}

void CCButtonPanel::clearUpSlot( ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    bool bY = (eVer == kBottomToTop);
    bool bX = (eHor == kLeftToRight);

    int iY = bY ? 1 : -1;
    int iX = bX ? 1 : -1;

    int iStartY = bY ? 0 : (m_iRow - 1);
    int iEndY = bY ? (m_iRow - 1) : 0;

    int iStartX = bX ? 0 : (m_iLine - 1);
    int iEndX = bX ? (m_iLine - 1) : 0;
    
    int iEmpty = -1;
    int iIndex;
    for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
    {
        for (int x = iStartX; bX ? (x <= iEndX) : (x >= iEndX); x += iX)
        {
            iIndex = toIndex(x, y);
            if (getButton(iIndex))
            {
                if (iEmpty >= 0)
                {
                    // 遇到按钮后且前面有空槽，移动按钮
                    moveButton(iIndex, iEmpty);
                    // 在前面找一块新空槽
                    iEmpty = allotSlot(index2X(iEmpty), index2Y(iEmpty), x, y, eVer, eHor);
                }
            }
            else
            {
                if (iEmpty < 0)
                {
                    // 首次遇到空槽
                    iEmpty = iIndex;
                }
            }
        }
    }
}

CCSkillButtonBase* CCButtonPanel::getButton(int iX, int iY) const
{
    return getButton(toIndex(iX, iY));
}

CCSkillButtonBase* CCButtonPanel::getButton( int iIndex ) const
{
    CCAssert(iIndex < m_iRow * m_iLine, "Break Bounds");
    return m_ppBtnPos[iIndex];
}

int CCButtonPanel::getButtonIndex(CCSkillButtonBase* pButton) const
{
    int n = m_iRow * m_iLine;
    for (int i = 0; i < n; ++i)
    {
        if (m_ppBtnPos[i] == pButton)
        {
            return i;
        }
    }
    
    return -1;
}

bool CCButtonPanel::isFull()
{
    return m_iCount == m_iRow * m_iLine;
}

int CCButtonPanel::index2Y( int iIndex ) const
{
    return iIndex / m_iLine;
}

int CCButtonPanel::index2X( int iIndex ) const
{
    return iIndex % m_iLine;
}

void CCButtonPanel::retainButton( CCSkillButtonBase* pButton )
{
    CC_SAFE_RETAIN(pButton);
    CC_SAFE_RELEASE(m_pRetain);
    m_pRetain = pButton;
}

CCSkillButtonBase* CCButtonPanel::getRetainButton() const
{
    return m_pRetain;
}

int CCButtonPanel::toIndex( int iX, int iY ) const
{
    return iY * m_iLine + iX;
}

void CCButtonPanel::addButtonEx( CCSkillButtonBase* pButton, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    addButton(pButton, allotSlot(eVer, eHor));
}

void CCButtonPanel::moveButton( int iIndexSrc, int iIndexDst )
{
    CCSkillButtonBase* pSrc = getButton(iIndexSrc);
    m_ppBtnPos[iIndexDst] = pSrc;
    m_ppBtnPos[iIndexSrc] = NULL;
    pSrc->setPosition(index2Point(iIndexDst));
}

CCPoint CCButtonPanel::index2Point( int iIndex )
{
    float fTmp = m_fButtonWidth + m_fInnerBorderWidth;
    float fTmp2 = m_fBorderWidth + m_fButtonWidth / 2;
    const CCSize& roSz = getContentSize();
    int iX = index2X(iIndex);
    int iY = index2Y(iIndex);
    return ccp(iX * fTmp - roSz.width / 2 + fTmp2, iY * fTmp - roSz.height / 2 + fTmp2);
}

void CCButtonPanel::pushAction( const ACTION_NODE& roAct )
{
    m_lstActs.push_back(roAct);
    if (m_lstActs.size() == 1)
    {
        onPrevActEnd(NULL);
    }
}

void CCButtonPanel::onPrevActEnd( CCNode* pNode )
{
    // 动作前半段，上一个动作即将结束，进行一些收尾工作
    CCAssert(!m_lstActs.empty(), "cannot be empty");
    
    if (pNode)
    {
        // 不是启动执行
        ACTION_NODE rNode = m_lstActs.front();
        switch (rNode.eAct)
        {
        case kAdd:
            break;

        case kDel:
            //m_pSkillMenu->removeChild(m_ppBtnPos[rNode.stDel.iIndex], true);
            delButton(rNode.stDel.pBtn);
            if (rNode.stDel.bClearUp)
            {
                // 队列前面插入clearUp动作节点
                m_lstActs.pop_front();
                m_lstActs.push_front(ACTION_NODE(rNode.stDel.eVer, rNode.stDel.eHor));
                m_lstActs.push_front(rNode);
            }
            
            break;

        case kMove:
            moveButton(getButtonIndex(rNode.stMove.pBtn), rNode.stMove.iIndexDst);
            break;

        case kAddEx:
            break;

        case kClearUp:
            break;

        default:
            ;
        }

        m_lstActs.pop_front();
    }
    
    if (m_lstActs.empty())
    {
        return;
    }

    // 动作后半段，上一个动作彻底执行结束，并且已弹出，新的动作开始运行
    ACTION_NODE rNode = m_lstActs.front();
    switch (rNode.eAct)
    {
    case kAdd:
        addButton(rNode.stAdd.pBtn, rNode.stAdd.iIndex);
        CC_SAFE_RELEASE(rNode.stAdd.pBtn);
        rNode.stAdd.pBtn->setOpacity(0);
        rNode.stAdd.pBtn->runAction(CCSequence::create(CCFadeIn::create(CONST_ACTION_DURATION), NULL));
        onPrevActEnd(rNode.stAdd.pBtn);
        break;

    case kDel:
        rNode.stDel.pBtn->stopAllActions();
        rNode.stDel.pBtn->runAction(CCSequence::create(CCFadeOut::create(CONST_ACTION_DURATION), CCCallFuncN::create(this, callfuncN_selector(CCButtonPanel::onPrevActEnd)), NULL));
        break;

    case kMove:
        rNode.stMove.pBtn->runAction(CCSequence::create(CCMoveTo::create(CONST_ACTION_DURATION, index2Point(rNode.stMove.iIndexDst)), CCCallFuncN::create(this, callfuncN_selector(CCButtonPanel::onPrevActEnd)), NULL));
        break;

    case kAddEx:
        addButton(rNode.stAddEx.pBtn, allotSlot(rNode.stAddEx.eVer, rNode.stAddEx.eHor));
        CC_SAFE_RELEASE(rNode.stAddEx.pBtn);
        rNode.stAddEx.pBtn->setOpacity(0);
        rNode.stAddEx.pBtn->runAction(CCSequence::create(CCFadeIn::create(CONST_ACTION_DURATION), NULL));
        onPrevActEnd(rNode.stAddEx.pBtn);
        break;

    case kClearUp:
        {
            bool bY = (rNode.stClearUp.eVer == kBottomToTop);
            bool bX = (rNode.stClearUp.eHor == kLeftToRight);

            int iY = bY ? 1 : -1;
            int iX = bX ? 1 : -1;

            int iStartY = bY ? 0 : (m_iRow - 1);
            int iEndY = bY ? (m_iRow - 1) : 0;

            int iStartX = bX ? 0 : (m_iLine - 1);
            int iEndX = bX ? (m_iLine - 1) : 0;

            int iMove = 0;
            m_lstActs.pop_front();
            CCSkillButtonBase* pBtn;
            int iEmpty = -1;
            int iIndex;
            for (int y = iStartY; bY ? (y <= iEndY) : (y >= iEndY); y += iY)
            {
                int iStartX0 = (y == iStartY ? iStartX : (bX ? 0 : (m_iLine - 1)));
                int iEndX0 = (y == iEndY ? iEndX : (bX ? (m_iLine - 1) : 0));
                for (int x = iStartX0; bX ? (x <= iEndX0) : (x >= iEndX0); x += iX)
                {
                    iIndex = toIndex(x, y);
                    pBtn = getButton(iIndex);
                    if (pBtn)
                    {
                        if (iEmpty >= 0)
                        {
                            // 遇到按钮后且前面有空槽，移动按钮
                            m_ppBtnPos[iEmpty] = pBtn;
                            m_ppBtnPos[iIndex] = NULL;
                            
                            m_lstActs.push_front(ACTION_NODE()); // empty action
                            ++iMove;
                            
                            pBtn->runAction(CCSequence::create(CCMoveTo::create(CONST_ACTION_DURATION, index2Point(iEmpty)), CCCallFuncN::create(this, callfuncN_selector(CCButtonPanel::onPrevActEnd)), NULL));
                            // 在前面找一块新空槽
                            iEmpty = allotSlot(index2X(iEmpty), index2Y(iEmpty), x, y, rNode.stClearUp.eVer, rNode.stClearUp.eHor);
                        }
                    }
                    else
                    {
                        if (iEmpty < 0)
                        {
                            // 首次遇到空槽
                            iEmpty = iIndex;
                        }
                    }
                }
            }

            if (!iMove && !m_lstActs.empty())
            {
                onPrevActEnd((CCNode*)(1));
            }
        }
        
        break;

    default:
        ;
    }
}

void CCButtonPanel::pushAddButtonAction( CCSkillButtonBase* pButton, int iIndex )
{
    CC_SAFE_RETAIN(pButton);
    pushAction(ACTION_NODE(pButton, iIndex));
}

void CCButtonPanel::pushDelButtonAction( int iIndex, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/, bool bClearUp /*= true*/ )
{
    ACTION_NODE stNode(iIndex, eVer, eHor, bClearUp);
    stNode.stDel.pBtn = getButton(iIndex);
    pushAction(stNode);
}

void CCButtonPanel::pushMoveButtonAction( int iIndexSrc, int iIndexDst )
{
    ACTION_NODE stNode(iIndexSrc, iIndexDst);
    stNode.stMove.pBtn = getButton(iIndexSrc);
    pushAction(stNode);
}

void CCButtonPanel::pushAddButtonExAction( CCSkillButtonBase* pButton, ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    CC_SAFE_RETAIN(pButton);
    pushAction(ACTION_NODE(pButton, eVer, eHor));
}

void CCButtonPanel::pushClearUpSlotAction( ADD_VERTICAL eVer /*= kBottomToTop*/, ADD_HORIZONTAL eHor /*= kLeftToRight*/ )
{
    pushAction(ACTION_NODE(eVer, eHor));
}

int CCButtonPanel::getMaxCount() const
{
    return m_iRow * m_iLine;
}

int CCButtonPanel::getCount() const
{
    return m_iCount;
}

CCProgressBar::CCProgressBar()
{
}

CCProgressBar::~CCProgressBar()
{
}

bool CCProgressBar::init( const CCSize& roSize, CCSprite* pFill, CCSprite* pBorder, float fHorizBorderWidth, float fVertBorderWidth, bool bFillOnTop )
{
    setAnchorPoint(ccp(0.5, 0.5));
    setContentSize(roSize);

    CCSize oFillSz(roSize.width - fVertBorderWidth * 2, roSize.height - fHorizBorderWidth * 2);

    m_pPt = CCProgressTimer::create(pFill);
    if (m_pPt->getParent())
    {
        m_pPt->removeFromParentAndCleanup(true);
    }
    addChild(m_pPt, bFillOnTop);
    CCSize oSz = m_pPt->getContentSize();
    m_pPt->setPosition(getAnchorPointInPoints());
    m_pPt->setScaleX(oFillSz.width / oSz.width);
    m_pPt->setScaleY(oFillSz.height / oSz.height);
    m_pPt->setType(kCCProgressTimerTypeBar);
    m_pPt->setPercentage(0);
    m_pPt->setBarChangeRate(ccp(1, 0));
    m_pPt->setMidpoint(ccp(0, 0));

    if (pBorder)
    {
        addChild(pBorder, !bFillOnTop);
        oSz = pBorder->getContentSize();
        pBorder->setPosition(getAnchorPointInPoints());
        pBorder->setScaleX(roSize.width / oSz.width);
        pBorder->setScaleY(roSize.height / oSz.height);
    }
    
    return true;
}

void CCProgressBar::setPercentage( float fPercent )
{
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    //m_oHd.setPosition()
    m_pPt->setPercentage(fPercent);
}

void CCProgressBar::setPercentage( float fPercent, float fDuration, CCFiniteTimeAction* pEndAction /*= NULL*/ )
{
    m_pPt->stopAllActions();
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    float fWidth = m_pPt->getScaleX() * m_pPt->getContentSize().width;
    if (pEndAction)
    {
        m_pPt->runAction(CCSequence::create(CCProgressTo::create(fDuration, fPercent), pEndAction, NULL));
    }
    else
    {
        m_pPt->runAction(CCSequence::create(CCProgressTo::create(fDuration, fPercent), NULL));
    }
    
}

CCFiniteTimeAction* CCProgressBar::setPercentageAction( float fPercent, float fDuration, CCFiniteTimeAction* pEndAction /*= NULL*/ )
{
    m_pPt->stopAllActions();
    if (fPercent > CONST_MAX_PROCESS_BAR_PERCENT)
    {
        fPercent = CONST_MAX_PROCESS_BAR_PERCENT;
    }
    float fWidth = m_pPt->getScaleX() * m_pPt->getContentSize().width;
    
    if (pEndAction)
    {
        return CCSequence::create(CCProgressTo::create(fDuration, fPercent), pEndAction, NULL);
    }
    return CCSequence::create(CCProgressTo::create(fDuration, fPercent), NULL);
}

void CCProgressBar::setFillColor( const ccColor3B& roColor )
{
    m_pPt->setColor(roColor);
}

const float CCProgressBar::CONST_MAX_PROCESS_BAR_PERCENT = 99.99999;

bool CCPackagePanel::init( int iRow, int iLine, float fButtonWidth, float fBorderWidth, float fInnerBorderWidth, const char* pBackgroundFrameName )
{
	m_iRow = iRow;
	m_iLine = iLine;
	m_fButtonWidth = fButtonWidth;
	m_fBorderWidth = fBorderWidth;
	m_fInnerBorderWidth = fInnerBorderWidth;
	m_pBackground = CCSprite::createWithSpriteFrameName(pBackgroundFrameName);
	addChild(m_pBackground);
	CCSize oSz = CCSizeMake(m_fBorderWidth * 2 + m_fInnerBorderWidth * (iRow - 1) + m_fButtonWidth * iRow, m_fBorderWidth * 2 + m_fInnerBorderWidth * (iLine - 1) + m_fButtonWidth * iLine);
	setContentSize(oSz);
	m_pBackground->setPosition(getAnchorPointInPoints());;

	m_pPropMenu = CCMenu::create();
	addChild(m_pPropMenu);
	m_pPropMenu->setContentSize(getContentSize());
	m_pPropMenu->setPosition(CCPointZero);

	return true;
}

void CCPackagePanel::addProp(CCMenuItem* pItem, int iX, int iY)
{
	m_pPropMenu->addChild(pItem);
	float fTmp = m_fButtonWidth + m_fInnerBorderWidth;
	float fTmp2 = m_fBorderWidth + m_fButtonWidth / 2;
	CCSize oSz = getContentSize();
	pItem->setPosition(ccp(iX * fTmp - oSz.width / 2 + fTmp2, iY * fTmp - oSz.height / 2 + fTmp2));
}

void CCPackagePanel::delProp(CCMenuItem* pItem)
{
	m_pPropMenu->removeChild(pItem, true);
}

CCPropItemImg::CCPropItemImg()
	:m_iPropKey(0)
{

}

CCPropItemImg::~CCPropItemImg()
{

}

bool CCPropItemImg::init( const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, int iPropKey )
{
	CCSkillButtonBase::init(pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, 
		pMaskImage, 0, this, callfuncN_selector(CCPropItemImg::onPropItemClick), callfuncN_selector(CCPropItemImg::onPropItemFinished));
	m_iPropKey = iPropKey;
	return true;
	
}

void CCPropItemImg::onPropItemClick( CCNode* pNode )
{
	
}

void CCPropItemImg::onPropItemFinished( CCNode* pNode )
{

}

void CCPropItemImg::setCoolDown( float fDuration )
{

}

float CCPropItemImg::getCoolDown() const
{
	return 0.0;
}

CCSkillButtonNormal* CCSkillButtonNormal::copyImg(CCObject* target, SEL_CallFuncN callOnClick, SEL_CallFuncN callOnFinished )
{
	CCSkillButtonNormal* pButtonRet = CCSkillButtonNormal::create(this->m_pNormalImageFrameName,this->m_pSelectImageFrameName,this->m_pDisableImageFrameName,NULL,NULL,this->getCoolDown(),target,callOnClick,callOnFinished);
// 	pButtonRet->setNormalImage(this->getNormalImage());
// 	pButtonRet->setSelectedImage(this->getSelectedImage());
// 	pButtonRet->setDisabledImage(this->getDisabledImage());
	return pButtonRet;
}

bool CCGameChapterButton::init( const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iLevel )
{
	CCSkillButtonNormal::init(pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
	m_iLevel = iLevel;
	return true;
}


bool CCCommmButton::init( const char* pNormalImage, const char* pSelectedImage, const char* pDisabledImage, const char* pBlinkImage, const char* pMaskImage, float fCoolDown, CCObject* pTarget, SEL_CallFuncN pOnClick, SEL_CallFuncN pOnFinished, int iKey )
{
	CCSkillButtonNormal::init(pNormalImage, pSelectedImage, pDisabledImage, pBlinkImage, pMaskImage, fCoolDown, pTarget, pOnClick, pOnFinished);
	m_iKey = iKey;
	return true;
}

bool CCLoadingLayer::init( CAsyncLoadingInterface* pLoader, int iStageCount, CCNode* pLoadingBackground, const CCSize& oTransf )
{
    //CCLayerColor::init();
    //m_pLoadingBackground = pLoadingBackground;

    setAnchorPoint(ccp(0.5, 0.5));
    CCSize oBgSz = pLoadingBackground->getContentSize();
    if (!oTransf.equals(CCSizeZero))
    {
        // 强制设定尺寸
        setContentSize(oTransf);
        addChild(pLoadingBackground);
        pLoadingBackground->setPosition(getAnchorPointInPoints());
        pLoadingBackground->setScaleX(oTransf.width / oBgSz.width);
        pLoadingBackground->setScaleY(oTransf.height / oBgSz.height);
        //pLoadingBackground->setContentSize(oTransf);
    }
    else
    {
        setContentSize(oBgSz);
        addChild(pLoadingBackground);
        pLoadingBackground->setPosition(getAnchorPointInPoints());
    }

    m_pAsyncLoading = pLoader;
    m_iLoadingStageCount = iStageCount;
    m_iLoadingStage = 0;
    m_pLoadingProgressNode = NULL;
    m_pLoadingProgressAction = NULL;
    
    
    return true;
}

void CCLoadingLayer::startLoading()
{
    m_pAsyncLoading->onLoading(m_iLoadingStage);
    if (m_pLoadingProgressNode && m_pLoadingProgressAction)
    {
        m_pLoadingProgressNode->runAction(CCSequence::createWithTwoActions(m_pLoadingProgressAction, CCCallFunc::create(this, callfunc_selector(CCLoadingLayer::onLoadingOnceEnd))));
    }
    else
    {
        runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.0), CCCallFunc::create(this, callfunc_selector(CCLoadingLayer::onLoadingOnceEnd))));
    }
}

void CCLoadingLayer::setProgressNodeAndAction( CCNode* pNode, CCFiniteTimeAction* pAction )
{
    m_pLoadingProgressNode = pNode;
    m_pLoadingProgressAction = pAction;
}

void CCLoadingLayer::onLoadingOnceEnd()
{
    ++m_iLoadingStage;
    if (m_iLoadingStage >= m_iLoadingStageCount)
    {
        // 加载完毕
        m_pAsyncLoading->onLoadingEnd();
    }
    else
    {
        startLoading();
    }
}

float CCLoadingLayer::getPercentage() const
{
    return (m_iLoadingStage + 1) * 100.0 / m_iLoadingStageCount;
}

