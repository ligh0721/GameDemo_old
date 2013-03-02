#pragma once

#include "GameCtrl.h"
#include "GameDisplay.h"

class CSkillInfo : public CCObject
{
    typedef struct
    {
        bool isFloat;
        union
        {
            float f;
            int i;
        };
    } S_VALUE;

public:
    CSkillInfo();
    virtual ~CSkillInfo();

    virtual bool init(int iMaxLevel, int iMaxValueCount, const char* pName, const char* pDescribe, const char* pBtnImgBaseName);
    M_CREATE_FUNC_PARAM(CSkillInfo, (int iMaxLevel, int iMaxValueCount, const char* pName, const char* pDescribe, const char* pBtnImgBaseName), iMaxLevel, iMaxValueCount, pName, pDescribe, pBtnImgBaseName);

    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual int getKey() const;

    M_SYNTHESIZE(int, m_iMaxLevel, MaxLevel);
    M_SYNTHESIZE(int, m_iMaxValueCount, MaxValueCount);
    
    void setName(const char* pName);
    const char* getName() const;
    void setDescribe(const char* pDescribe);
    const char* getDescribe() const;
    void setButtonImage(const char* pBtnImgBaseName);
    const char* getButtonImage() const;
    
    void setValueAsFloat(int iLevel, int iIndex, float fValue);
    void setValueAsInteger(int iLevel, int iIndex, int iValue);
    float getValueAsFloat(int iLevel, int iIndex) const;
    int getValueAsInteger(int iLevel, int iIndex) const;

public:
    const int m_iKey;
    S_VALUE** m_ppValue;
    string m_sName;
    string m_sDescribe;
    string m_sBtnImg;
};

// ººƒ‹£¨∞¸∫¨¡À“ª–©ººƒ‹–≠“Èµƒ‘º∂®
class CSkill : public CCObject, public CLevelExp
{
    friend class CUnit;
	friend class CConsumeProp;

public:
    CSkill();
    virtual ~CSkill();
    //CREATE_FUNC(CSkill);
    virtual bool init();
    
    // ººƒ‹…ÓøΩ±¥£¨√ø∏ˆ≈……˙µƒ–¬ººƒ‹∂º±ÿ–Î∏≤∏«∏√∫Ø ˝£¨“‘±„”⁄ººƒ‹∞¥’’ƒ£∞Âººƒ‹∏¥÷∆–¬∂‘œÛ
    // have to @override
    virtual CCObject* copyWithZone(CCZone* pZone) = 0;

    // ººƒ‹±ªÃÌº”/…æ≥˝ ±¥•∑¢
    // ±ÿ–Î∏≤∏«£¨«“∏≤∏«∫Û±ÿ–Î‘Ÿµ˜”√∏∏¿‡∫Ø ˝£¨‘⁄∆‰÷–Ω¯––◊¢≤·/◊¢œ˙¥•∑¢∆˜µƒ≤Ÿ◊˜
    virtual void onSkillAdd(); // have to @override
    virtual void onSkillDel(); // have to @override

    // ººƒ‹◊º±∏æÕ–˜(CD)∫Û¥•∑¢
    virtual void onSkillReady(); // @override

protected:
    // ººƒ‹≥÷”–’ﬂ ¬º˛œÏ”¶£¨÷ª∏≤±ª◊¢≤·µƒ¥•∑¢∆˜œ‡”¶µƒ ¬º˛∫Ø ˝º¥ø…
    // @override
    virtual void onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    virtual CAttackData* onUnitAttacked(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitDamageTarget(float fDamage, CUnit* pTarget);
    virtual void onUnitHpChange(float fChanged);
    virtual void onUnitRevive();
    virtual void onUnitDie();
    virtual void onUnitTick(float fDt);
    virtual void onUnitDestroyProjectile(CCProjectileWithAttackData* pProjectile);

    // ◊¢≤·¥•∑¢∆˜£¨Õ®≥£‘⁄ onSkillAdd ÷–±ªµ˜”√
    void registerOnAttackTargetTrigger();
    void registerOnAttackedTrigger();
    void registerOnDamagedSurfaceTrigger();
    void registerOnDamagedInnerTrigger();
    void registerOnDamageTargetTrigger();
    void registerOnHpChangeTrigger();
    void registerOnReviveTrigger();
    void registerOnDieTrigger();
    void registerOnTickTrigger();
    void registerOnDestroyProjectileTrigger();

    // ◊¢œ˙¥•∑¢∆˜£¨Õ®≥£‘⁄ onSkillDel ÷–±ªµ˜”√
    void unregisterOnAttackTargetTrigger();
    void unregisterOnAttackedTrigger();
    void unregisterOnDamagedSurfaceTrigger();
    void unregisterOnDamagedInnerTrigger();
    void unregisterOnDamageTargetTrigger();
    void unregisterOnHpChangeTrigger();
    void unregisterOnReviveTrigger();
    void unregisterOnDieTrigger();
    void unregisterOnTickTrigger();
    void unregisterOnDestroyProjectileTrigger();

    // —”≥Ÿ◊¢œ˙¥•∑¢∆˜£¨Õ®≥£‘⁄æﬂ±∏“ª∂®≥÷–¯ ±º‰µƒBUFF¿‡ººƒ‹µƒ onSkillDel ÷–±ªµ˜”√
    void unregisterOnTickTriggerLater();


public:
    //virtual void setName(const char* pName);
    //virtual const char* getName();
    virtual int getKey() const;

    // ªÒ»°ººƒ‹≥÷”–’ﬂ
    virtual CUnit* getOwner() const;

    
    M_SYNTHESIZE(float, m_fCoolDown, CoolDown);
    M_SYNTHESIZE(float, m_fCDLeft, CoolDownLeft);
    M_SYNTHESIZE(CCNode*, m_pDisplayBody, DisplayBody);
    M_SYNTHESIZE(int, m_iInfoKey, InfoKey);

protected:
    virtual void setOwner(CUnit* pOwner);

protected:
    const int m_iKey;
    CUnit* m_pOwner;
    std::string m_sName;
};

// æﬂ±∏µ»º∂µƒººƒ‹£¨Œ™ººƒ‹µ»º∂…˝º∂‘ˆº”œﬁ÷∆≤Œ ˝
class CLevelLimitSkill : public CSkill
{
public:
    CLevelLimitSkill();
    virtual ~CLevelLimitSkill();
    //CREATE_FUNC(CLevelLimitSkill);
    virtual bool init();

    bool canBeLearning(uint32_t dwLvl) const;

    M_SYNTHESIZE_READONLY(uint32_t, m_dwLvlLmt, LevelLimit);
    M_SYNTHESIZE_READONLY(uint32_t, m_dwLvlStp, LevelStep);
};

// ÷˜∂Øººƒ‹£¨¥•∑¢ ±ª˙”…≥÷”–’ﬂ◊‘…Ìøÿ÷∆£¨≤¢¥¯”–CoolDown–‘÷ 
class CActiveSkill : public CLevelLimitSkill
{
public:
    enum CAST_TARGET_TYPE
    {
        kNoTarget,
        kPointTarget,
        kUnitTarget
    };

//     enum WEAPON_TYPE
//     {
//         kWTClosely = 0,
//         kWTInstant = 1,
//         kWTDelayed = 2
//     };

public:
    CActiveSkill();
    virtual ~CActiveSkill();

    virtual bool init(float fCoolDown);
    //M_CREATE_FUNC_PARAM(CActiveSkill, (float fCoolDown), fCoolDown);

    virtual void cast();
    virtual bool canCast() const;
    virtual bool isCoolingDown() const;
    virtual void resetCD();

    // µ±ººƒ‹ø™ º Õ∑≈ ±¥•∑¢
    // @override
    virtual void onSkillCast();

    M_SYNTHESIZE_PASS_BY_REF(CCPoint, m_oTargetPoint, TargetPoint);
    CC_PROPERTY(int, m_iTargetUnit, TargetUnit);
    M_SYNTHESIZE(float, m_fCastRange, CastRange);
    M_SYNTHESIZE(CAST_TARGET_TYPE, m_eCastTargetType, CastTargetType);
    M_SYNTHESIZE(float, m_fTargetUnitHalfOfWidth, TargetUnitHalfOfWidth); // cache property
    M_SYNTHESIZE(CGameUnit::WEAPON_TYPE, m_eWeaponType, WeaponType);
    const CCPoint& updateTargetUnitPoint();
    virtual CCSkillButtonAdvance* getSkillButton();
    virtual void setCastAniInfo(CGameUnit::ANIMATION_INDEX eAniIndex, float fCastEffectDelay);
    virtual CGameUnit::ANIMATION_INDEX getCastAniIndex() const;
    virtual float getCastEffectDelay() const;

public:
    CGameUnit::ANIMATION_INDEX m_eAniIndex;
    float m_fCastEffectDelay;
};

// ±ª∂Øººƒ‹£¨¥•∑¢ ±ª˙”…±ª∂Øººƒ‹À˘◊¢≤·µƒ¥•∑¢∆˜À˘æˆ∂®
class CPassiveSkill : public CLevelLimitSkill
{
public:
    CPassiveSkill();
    virtual ~CPassiveSkill();
    //CREATE_FUNC(CPassiveSkill);
    virtual bool init();
};

// BUFF£¨¿‡À∆”⁄±ª∂Øººƒ‹£¨Õ®≥£”…Õ‚≤ø∏Ωº”∏¯µ•Œª
class CBuffSkill : public CPassiveSkill
{
    friend class CUnit;

public:
    CBuffSkill();
    virtual ~CBuffSkill();
    virtual bool init(float fDuration, bool bCanBePlural = false, int iSrcKey = 0);
    //M_CREATE_FUNC_PARAM(CBuffSkill, (float fDuration, bool bCanBePlural), fDuration, bCanBePlural);
    M_GET_TYPE_KEY;

    void timeStep(float dt); //  ±º‰≤Ω‘ˆ£¨‘⁄ onUnitTick ÷– ◊≤øµ˜”√
    void delBuffIfTimeout(); // BUFF≈–∂œ…æ≥˝£¨‘⁄ onUnitTick ÷–Œ≤≤øµ˜”√

    M_SYNTHESIZE(float, m_fDuration, Duration);
    M_SYNTHESIZE(float, m_fPass, Pass);
    M_SYNTHESIZE(int, m_iSrcKey, SrcKey);

    // BUFF «∑Òø…÷ÿµ˛∏Ωº”
    virtual void setPlural(bool bPlural);
    virtual bool canBePlural() const;

    // ÷˜∂Ø¥”≥÷”–’ﬂ…Ì…œ…æ≥˝BUFF◊‘…Ì£¨≤¢Ω´ Owner  Ù–‘÷√Œ™ NULL
    virtual void delBuffFromOwner(bool bAfterTriggerLoop = true);
    
protected:
    // ¥”BUFF≈……˙µƒ◊”¿‡£¨≤ª–Ë“™∏≤∏«œ¬¡Ω∏ˆ∫Ø ˝
    // ƒ⁄≤ø◊¢≤·/—”≥Ÿ◊¢œ˙¡À“ª∏ˆ onUnitTick ¥•∑¢∆˜£¨”√”⁄øÿ÷∆BUFF≥÷–¯ ±º‰
    virtual void onSkillAdd();
    virtual void onSkillDel();

    // ¥”BUFF≈……˙µƒ◊”¿‡£¨±ÿ–Î∏≤∏«œ¬¡Ω∏ˆ∫Ø ˝£¨≤¢«“∏≤∏«∫Ø ˝÷–±ÿ–Îµ˜”√∏∏¿‡∫Ø ˝
    // BUFFµƒ◊¢≤·/◊¢œ˙◊¢≤·¥•∑¢∆˜£¨–Ë“™ÃÌº”÷¡œ¬¡–∫Ø ˝÷–
    // have to @override
    virtual void onBuffAdd();
    virtual void onBuffDel();

    // —°‘Ò–‘∏≤∏«£¨»Áπ˚‘⁄BUFF≥÷–¯µƒ ±º‰ƒ⁄£¨–Ë“™÷‹∆⁄–‘µƒ∂‘µ•ŒªΩ¯––¥¶¿Ì£¨‘Ú–Ë“™∏≤∏«
    // @override
    virtual void onUnitTick(float fDt);

public:
    bool m_bCanBePlural;
};

class CAuraPas : public CPassiveSkill
{
public:
    enum TARGET_FLAG
    {
        kSelf = 1,
        kOwn = 1 << 1,
        kAlly = 1 << 2,
        kEnemy = 1 << 3,
    };

public:
    virtual bool init(float fRange, uint32_t dwTargetFlag, float fInterval, int iBuffTemplateKey, int iBuffLevel);
    M_CREATE_FUNC_PARAM(CAuraPas, (float fRange, uint32 dwTargetFlag, float fInterval, int iBuffTemplateKey, int iBuffLevel), fRange, dwTargetFlag, fInterval, iBuffTemplateKey, iBuffLevel);
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    virtual void onSkillAdd();
    virtual void onSkillDel();

protected:
    virtual void onUnitTick(float fDt);

public:
    float m_fPass;
    static const int CONST_MIN_DURATION = 1;
    float m_fRange;
    uint32_t m_dwTargetFlag;
    float m_fInterval;
    int m_iBuffTemplateKey;
    int m_iBuffLevel;
     
};

class CSelfBuffMakerAct : public CActiveSkill
{
public:
    virtual bool init(float fCoolDown, int iBuffTemplateKey, int iBuffLevel);
    M_CREATE_FUNC_PARAM(CSelfBuffMakerAct, (float fCoolDown, int iBuffTemplateKey, int iBuffLevel), fCoolDown, iBuffTemplateKey, iBuffLevel);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillCast();

public:
    int m_iBuffTemplateKey;
    int m_iBuffLevel;
};

class CAttackBuffMakerPas : public CPassiveSkill
{
public:
    virtual bool init(int iProbability, int iBuffTemplateKey, int iBuffLevel, const CExtraCoeff& roCoeff);
    M_CREATE_FUNC_PARAM(CAttackBuffMakerPas, (int iProbability, int iBuffTemplateKey, int iBuffLevel, const CExtraCoeff& roCoeff), iProbability, iBuffTemplateKey, iBuffLevel, roCoeff);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);

public:
    int m_iProbability;
    int m_iBuffLevel;
    int m_iBuffTemplateKey;
    CExtraCoeff m_oCoeff;
};

class CHpChangeBuff : public CBuffSkill
{
public:
    virtual bool init(float fDuration, bool bCanBePlural, float fInterval, float fHpChange, bool bPercent, float fMinHp);
    M_CREATE_FUNC_PARAM(CHpChangeBuff, (float fDuration, bool bCanBePlural, float fInterval, float fHpChange, bool bPercent, float fMinHp), fDuration, bCanBePlural, fInterval, fHpChange, bPercent, fMinHp);
    virtual CCObject* copyWithZone(CCZone* pZone);
    M_GET_TYPE_KEY;

    M_SYNTHESIZE(float, m_fInterval, Interval);
    M_SYNTHESIZE(float, m_fIntervalPass, IntervalPass);
    M_SYNTHESIZE(float, m_fHpChange, HpChange);
    M_SYNTHESIZE(bool, m_bPercent, Percent);
    M_SYNTHESIZE(float, m_fMinHp, MinHp);


protected:
    virtual void onUnitTick(float fDt);
    virtual void onUnitInterval();

};

class CVampirePas : public CPassiveSkill
{
public:
    virtual bool init(float fRatio);
    M_CREATE_FUNC_PARAM(CVampirePas, (float fRatio), fRatio);
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitDamageTarget(float fDamage, CUnit* pTarget);

public:
    float m_fRatio;
};

class CThumpPas : public CPassiveSkill
{
public:
    virtual bool init(int iProbability, const CExtraCoeff& roCoeff, float fDuration);
    M_CREATE_FUNC_PARAM(CThumpPas, (int iProbability, const CExtraCoeff& roCoeff, float fDuration), iProbability, roCoeff, fDuration);
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);

public:
    int m_iProbability;
    CExtraCoeff m_oCoeff;
    float m_fDuration;

};

class CStunBuff : public CBuffSkill
{
public:
    virtual bool init(float fDuration, bool bCanBePlural);
    M_CREATE_FUNC_PARAM(CStunBuff, (float fDuration, bool bCanBePlural), fDuration, bCanBePlural);
    virtual CCObject* copyWithZone(CCZone* pZone);
    M_GET_TYPE_KEY;

protected:
    virtual void onBuffAdd();
    virtual void onBuffDel();
};

class CDoubleAttackPas : public CPassiveSkill
{
public:
    virtual bool init(int iProbability);
    M_CREATE_FUNC_PARAM(CDoubleAttackPas, (int iProbability), iProbability);
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitAttackTarget(CAttackData* pAttack, CUnit* pTarget);

public:
    int m_iProbability;
};

class CHarmShieldBuff : public CBuffSkill
{
public:
    virtual bool init(float fDuration, bool bCanBePlural, const CExtraCoeff& roCoeff, bool bDelBuff);
    M_CREATE_FUNC_PARAM(CHarmShieldBuff, (float fDuration, bool bCanBePlural, const CExtraCoeff& roCoeff, bool bDelBuff), fDuration, bCanBePlural, roCoeff, bDelBuff);
    virtual CCObject* copyWithZone(CCZone* pZone);
    M_GET_TYPE_KEY;

protected:
    virtual void onBuffAdd();
    virtual void onBuffDel();

    virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource);

public:
    CExtraCoeff m_oCoeff;
    bool m_bDelBuff;
};

class CCommboPas : public CPassiveSkill
{
public:
    virtual bool init(float fMaxInterval, const char* pFmt);
    M_CREATE_FUNC_PARAM(CCommboPas, (float fMaxInterval, const char* pFmt), fMaxInterval, pFmt);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitDamageTarget(float fDamage, CUnit* pTarget);
    virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitTick(float fDt);

public:
    float m_fDt;
    float m_fMaxInterval;
    int m_iCombo;
    int m_iComboOld;
    string m_sFmt;
    CCLabelTTF* m_pLabel;
};

class CAttackTextPas : public CPassiveSkill
{
public:
    virtual bool init(float fMaxInterval, const char* pFmt);
    M_CREATE_FUNC_PARAM(CAttackTextPas, (float fMaxInterval, const char* pFmt), fMaxInterval, pFmt);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitDamageTarget(float fDamage, CUnit* pTarget);
    virtual void onUnitDamaged(CAttackData* pAttack, CUnit* pSource);
    virtual void onUnitTick(float fDt);

public:
    float m_fDt;
    float m_fMaxInterval;
    float m_fValue;
    string m_sFmt;
    CCLabelTTF* m_pLabel;
};

class CScatterFireAct : public CActiveSkill
{
public:
    virtual bool init(float fCoolDown, float fPower, int iCount, float fDelta, float fRange, float fVel, float fScaleX, float fScaleY);
    M_CREATE_FUNC_PARAM(CScatterFireAct, (float fCoolDown, float fPower, int iCount, float fDelta, float fRange, float fVel, float fScaleX, float fScaleY), fCoolDown, fPower, iCount, fDelta, fRange, fVel, fScaleX, fScaleY);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillCast();

public:
    float m_fPower;
    int m_iCount;
    float m_fDelta;
    float m_fRange;
    float m_fVel;
    float m_fScaleX;
    float m_fScaleY;
};

class CShrapnelPas : public CPassiveSkill
{
public:
    virtual bool init(float fPower, int iCount, float fDelta, float fRange, float fVel, float fScaleX, float fScaleY, int iProbability, int iMaxGeneration);
    M_CREATE_FUNC_PARAM(CShrapnelPas, (float fPower, int iCount, float fDelta, float fRange, float fVel, float fScaleX, float fScaleY, int iProbability, int iMaxGeneration), fPower, iCount, fDelta, fRange, fVel, fScaleX, fScaleY, iProbability, iMaxGeneration);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitDestroyProjectile(CCProjectileWithAttackData* pProjectile);

public:
    float m_fPower;
    int m_iCount;
    float m_fDelta;
    float m_fRange;
    float m_fVel;
    float m_fScaleX;
    float m_fScaleY;
    int m_iProbability;
    int m_iMaxGeneration;
};

class CHomingMissileAct : public CActiveSkill
{
public:
    virtual bool init(float fCoolDown, float fPower, float fInitDuration, int iBuffTemplateKey, int iBuffLevel);
    M_CREATE_FUNC_PARAM(CHomingMissileAct, (float fCoolDown, float fPower, float fInitDuration, int iBuffTemplateKey, int iBuffLevel), fCoolDown, fPower, fInitDuration, iBuffTemplateKey, iBuffLevel);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillCast();

public:
    float m_fPower;
    float m_fInitDuration;
    int m_iBuffLevel;
    int m_iBuffTemplateKey;
};

class CStatusShowPas : public CPassiveSkill
{
public:
    virtual bool init();
    M_CREATE_FUNC_PARAM(CStatusShowPas, ());
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitHpChange(float fChanged);

public:
    CCProgressBar m_oProgressBar;
};

class CHpChangePas : public CPassiveSkill
{
public:
    virtual bool init(float fInterval, float fHpChange, bool bPercent, float fMinHp);
    M_CREATE_FUNC_PARAM(CHpChangePas, (float fInterval, float fHpChange, bool bPercent, float fMinHp), fInterval, fHpChange, bPercent, fMinHp);
    virtual CCObject* copyWithZone(CCZone* pZone);
    M_GET_TYPE_KEY;

    M_SYNTHESIZE(float, m_fInterval, Interval);
    M_SYNTHESIZE(float, m_fIntervalPass, IntervalPass);
    M_SYNTHESIZE(float, m_fHpChange, HpChange);
    M_SYNTHESIZE(bool, m_bPercent, Percent);
    M_SYNTHESIZE(float, m_fMinHp, MinHp);

    virtual void onSkillAdd();
    virtual void onSkillDel();

protected:
    virtual void onUnitTick(float fDt);
    virtual void onUnitInterval();

};

class CSpeedBuff : public CBuffSkill
{
public:
    virtual bool init(float fDuration, bool bCanBePlural, const CExtraCoeff& roExMoveSpeed, const CExtraCoeff& roExAttackSpeed);
    M_CREATE_FUNC_PARAM(CSpeedBuff, (float fDuration, bool bCanBePlural, const CExtraCoeff& roExMoveSpeed, const CExtraCoeff& roExAttackSpeed), fDuration, bCanBePlural, roExMoveSpeed, roExAttackSpeed);
    virtual CCObject* copyWithZone(CCZone* pZone);
    M_GET_TYPE_KEY;

protected:
    virtual void onBuffAdd();
    virtual void onBuffDel();

public:
    CExtraCoeff m_oExMoveSpeed;
    CExtraCoeff m_oExAttackSpeed;
};

class CSplashPas : public CPassiveSkill
{
public:
    virtual bool init(float fNearRange, const CExtraCoeff& roExNearDamage, float fMidRange, const CExtraCoeff& roExMidDamage, float fFarRange, const CExtraCoeff& roExFarDamage);
    M_CREATE_FUNC_PARAM(CSplashPas, (float fNearRange, const CExtraCoeff& roExNearDamage, float fMidRange, const CExtraCoeff& roExMidDamage, float fFarRange, const CExtraCoeff& roExFarDamage), fNearRange, roExNearDamage, fMidRange, roExMidDamage, fFarRange, roExFarDamage);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onUnitDamageTarget(float fDamage, CUnit* pTarget);

public:
    float m_fNearRange;
    CExtraCoeff m_oExNearDamage;
    float m_fMidRange;
    CExtraCoeff m_oExMidDamage;
    float m_fFarRange;
    CExtraCoeff m_oExFarDamage;
};

class CSplashAct : public CActiveSkill
{
public:
    virtual bool init(float fCoolDown, float fNearRange, const CAttackValue& roNearDamage, float fMidRange, const CAttackValue& roMidDamage, float fFarRange, const CAttackValue& roFarDamage, int iBuffTemplateKey, int iBuffLevel);
    M_CREATE_FUNC_PARAM(CSplashAct, (float fCoolDown, float fNearRange, const CAttackValue& roNearDamage, float fMidRange, const CAttackValue& roMidDamage, float fFarRange, const CAttackValue& roFarDamage, int iBuffTemplateKey, int iBuffLevel), fCoolDown, fNearRange, roNearDamage, fMidRange, roMidDamage, fFarRange, roFarDamage, iBuffTemplateKey, iBuffLevel);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onSkillCast();

public:
    float m_fNearRange;
    CAttackValue m_oNearDamage;
    float m_fMidRange;
    CAttackValue m_oMidDamage;
    float m_fFarRange;
    CAttackValue m_oFarDamage;
    int m_iBuffTemplateKey;
    int m_iBuffLevel;
};

class CThrowBuff : public CBuffSkill
{
public:
    CThrowBuff();

    virtual bool init(float fDuration, bool bCanBePlural, int iSrcKey, float fThrowRange, float fThrowDuration, float fMaxHeight, const CAttackValue& roDamage, float fDamageRange, int iBuffTemplateKey, int iBuffLevel);
    M_CREATE_FUNC_PARAM(CThrowBuff, (float fDuration, bool bCanBePlural, int iSrcKey, float fThrowRange, float fThrowDuration, float fMaxHeight, const CAttackValue& roDamage, float fDamageRange, int iBuffTemplateKey, int iBuffLevel), fDuration, bCanBePlural, iSrcKey, fThrowRange, fThrowDuration, fMaxHeight, roDamage, fDamageRange, iBuffTemplateKey, iBuffLevel);
    virtual CCObject* copyWithZone(CCZone* pZone);

    M_SYNTHESIZE_PASS_BY_REF(CCPoint, m_oEndPos, EndPos);
    M_SYNTHESIZE(float, m_fThrowDuration, ThrowDuration);
    M_SYNTHESIZE(float, m_fMaxHeight, MaxHeight);
    M_SYNTHESIZE(float, m_fThrowRange, ThrowRange);

    virtual void onBuffAdd();
    virtual void onBuffDel();

    virtual void onThrowEnd(CCNode* pNode);

public:
    const int m_iActRotateKey;
    const int m_iActThrowKey;
    CAttackValue m_oDamage;
    int m_iBuffTemplateKey;
    int m_iBuffLevel;
    float m_fDamageRange;
};

class CTransmitBuff : public CBuffSkill
{
public:
	CTransmitBuff();

	virtual bool init(float fDuration, bool bCanBePlural, int iSrcKey, vector<CCPoint>& vecRandomPoint, float fFadeInDuration,  float fFadeOutDuration, int iBuffTemplateKey, int iBuffLevel);
	M_CREATE_FUNC_PARAM(CTransmitBuff, (float fDuration, bool bCanBePlural, int iSrcKey, vector<CCPoint>& vecRandomPoint, float fFadeInDuration,  float fFadeOutDuration, int iBuffTemplateKey, int iBuffLevel), fDuration, bCanBePlural, iSrcKey, vecRandomPoint, fFadeInDuration, fFadeOutDuration, iBuffTemplateKey, iBuffLevel);
	virtual CCObject* copyWithZone(CCZone* pZone);

	M_SYNTHESIZE(float, m_fFadeInDuration, FadeInDuration);
	M_SYNTHESIZE(float, m_fFadeOutDuration, FadeOutDuration);
	virtual void onBuffAdd();
	virtual void onBuffDel();

	virtual void onTransmitBegin(CCNode* pNode);
	virtual void onTransmitEnd(CCNode* pNode);

public:
	vector<CCPoint> m_vecRandomPoint;
	int m_iLastAttackedUnit;
	int m_iBuffTemplateKey;
	int m_iBuffLevel;
};

class CChainLightingBuff : public CBuffSkill
{
public:
    
    virtual bool init(float fDuration, bool bCanBePlural, int iSrcKey, float fMaxCastRange, float fMaxJumpDistance, int iMaxJumpCount, const CAttackValue& roDamage, const vector<int>& vecEffectedUnitKey);
    M_CREATE_FUNC_PARAM(CChainLightingBuff, (float fDuration, bool bCanBePlural, int iSrcKey, float fMaxCastRange, float fMaxJumpDistance, int iMaxJumpCount, const CAttackValue& roDamage, const vector<int>& vecEffectedUnitKey), fDuration, bCanBePlural, iSrcKey, fMaxCastRange, fMaxJumpDistance, iMaxJumpCount, roDamage, vecEffectedUnitKey);
    virtual CCObject* copyWithZone(CCZone* pZone);
    
    virtual void onBuffAdd();
	virtual void onBuffDel();
    virtual void turnNext(CCObject* pObj);
public:
    float m_fMaxCastRange;
    float m_fMaxJumpDistance;
    int m_iMaxJumpCount;
    CAttackValue m_oDamage;
    vector<int>  m_vecEffectedUnitKey;

private:
    CGameUnit* m_pNextUnit;
    CChainLightingBuff* m_pBuffCP;
};

class CSwordStormSkill : public CPassiveSkill
{
public:
	CSwordStormSkill();
    
	virtual bool init(int iProbability, float fDuration, float fMaxDamageRange, const CAttackValue& roMaxDamage, const CExtraCoeff& roDamageCoef,  char* pActName);
	M_CREATE_FUNC_PARAM(CSwordStormSkill, (int iProbability, float fDuration, float fMaxDamageRange, const CAttackValue& roMaxDamage, const CExtraCoeff& roDamageCoef,  char* pActName), iProbability, fDuration, fMaxDamageRange, roMaxDamage, roDamageCoef, pActName);
	virtual CCObject* copyWithZone(CCZone* pZone);
    
	virtual void onSkillAdd();
	virtual void onSkillDel();
    virtual void onUnitDamageTarget(float fDamage, CUnit* pTarget);
    
    M_SYNTHESIZE(float, m_fDelayPerUnit, DelayPerUnit);
    M_SYNTHESIZE(int, m_iCountAnimLoop, CountAnimLoop);
    
    virtual void onActEndPerAnim(CCObject* pObj);
private:
    int m_iProbability;
    float m_fDuration;
    float m_fMaxDamageRange;
    CAttackValue m_oMaxDamage;
    CExtraCoeff m_oDamageCoef;
    char* m_pActName;
};

class CProjectileAct : public CActiveSkill
{
public:
    virtual bool init(float fCoolDown, float fCastRange, const CAttackValue& roDamage, CProjectile* pProj, int iBuffTemplateKey, int iBuffLevel);
    M_CREATE_FUNC_PARAM(CProjectileAct, (float fCoolDown, float fCastRange, const CAttackValue& roDamage, CProjectile* pProj, int iBuffTemplateKey, int iBuffLevel), fCoolDown, fCastRange, roDamage, pProj, iBuffTemplateKey, iBuffLevel);
    virtual CCObject* copyWithZone(CCZone* pZone);

    virtual void onSkillAdd();
    virtual void onSkillDel();

    virtual void onSkillCast();

    CC_PROPERTY(CProjectile*, m_pTemplateProjectile, TemplateProjectile);
    M_SYNTHESIZE(float, m_fProjectileMoveSpeed, ProjectileMoveSpeed);
    M_SYNTHESIZE(float, m_fProjectileScale, ProjectileScale);
    M_SYNTHESIZE(float, m_fProjectileMaxOffsetY, ProjectileMaxOffsetY);
    M_SYNTHESIZE(float, m_fProjectileBirthOffsetX, ProjectileBirthOffsetX);
    M_SYNTHESIZE(float, m_fProjectileBirthOffsetY, ProjectileBirthOffsetY);

public:
    CAttackValue m_oDamage;
    int m_iBuffTemplateKey;
    int m_iBuffLevel;

};

class CChainBuff : public CBuffSkill
{
public:
    typedef vector<int> VEC_DAMAGED;

public:
    CChainBuff();

    virtual bool init(float fDuration, bool bCanBePlural, int iSrcKey, float fRange, int iMaxTimes, const CAttackValue& roDamage, CProjectile* pProj);
    M_CREATE_FUNC_PARAM(CChainBuff, (float fDuration, bool bCanBePlural, int iSrcKey, float fRange, int iMaxTimes, const CAttackValue& roDamage, CProjectile* pProj), fDuration, bCanBePlural, iSrcKey, fRange, iMaxTimes, roDamage, pProj);
    virtual CCObject* copyWithZone(CCZone* pZone);
    M_GET_TYPE_KEY;

    virtual void onBuffAdd();
    virtual void onBuffDel();

    M_SYNTHESIZE(int, m_iMaxTimes, MaxTimes)
    M_SYNTHESIZE(int, m_iStartUnit, StartUnit);
    M_SYNTHESIZE(int, m_iEndUnit, EndUnit);

    CC_PROPERTY(CProjectile*, m_pTemplateProjectile, TemplateProjectile);
    M_SYNTHESIZE(float, m_fProjectileMoveSpeed, ProjectileMoveSpeed);
    M_SYNTHESIZE(float, m_fProjectileScale, ProjectileScale);
    M_SYNTHESIZE(float, m_fProjectileMaxOffsetY, ProjectileMaxOffsetY);
    M_SYNTHESIZE(float, m_fProjectileBirthOffsetX, ProjectileBirthOffsetX);
    M_SYNTHESIZE(float, m_fProjectileBirthOffsetY, ProjectileBirthOffsetY);
    M_SYNTHESIZE(CGameUnit::WEAPON_TYPE, m_eWeaponType, WeaponType);

    static bool checkConditions(CGameUnit* pUnit, CChainBuff* pBuff);

public:
    float m_fRange;
    CAttackValue m_oDamage;
    VEC_DAMAGED m_vecDamaged;
};

