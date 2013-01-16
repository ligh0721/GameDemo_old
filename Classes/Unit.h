#pragma once

class CUnit;
class CAttackData;
class CSkill;
class CBuffSkill;
class CUnitPackage;
class CCProjectileWithAttackData;
class CGameFile;

class CLevelExp;

class CLevelUpdate
{
public:
    virtual void updateMaxExp(CLevelExp* pLevel) = 0; // @override
    virtual void onLevelChange(CLevelExp* pLevel, int32_t iChanged) = 0; // @override
};

// 等级经验值，赋予对象等级经验值特性
// 需要覆盖 updateMaxExp，提供等级变化时的最大经验值变更公式
// 等级变化后触发 onLevelChange
class CLevelExp
{
public:
    CLevelExp();
    virtual ~CLevelExp();
    //CREATE_FUNC(CLevelExp);
    //virtual bool init();

    virtual void updateMaxExp(); // @override
    virtual void onLevelChange(int32_t iChanged); // @override
    void addExp(uint32_t dwExp);
    void addLevel(uint32_t dwLvl);
    virtual void setLevel(uint32_t dwLvl);
    virtual void setMaxLevel(uint32_t dwMaxLvl);

    uint32_t getLevel() const;
    uint32_t getMaxLevel() const;
    uint32_t getExp() const;
    uint32_t getMaxExp() const;

    void setLevelUpdate(CLevelUpdate* pUpdate);

public:
    uint32_t m_dwLvl;
    uint32_t m_dwMaxLvl;
    uint32_t m_dwExp;
    uint32_t m_dwMaxExp;
    CLevelUpdate* m_pUpdate;

};


class CLife : public CCObject
{
public:
    CLife();
    virtual ~CLife();
    CREATE_FUNC(CLife);
    virtual bool init();

    bool revive(float fHp);
    bool setHp(float fHp);
    void setMaxHp(float fMaxHp);
    bool isDead() const;
    virtual void onRevive();
    virtual void onDie();
    virtual void onHpChange(float fChanged);


    M_SYNTHESIZE_READONLY(float, m_fHp, Hp);
    M_SYNTHESIZE_READONLY(float, m_fMaxHp, MaxHp);


};

// 攻击数值，由多种类型的攻击组合而成
class CAttackValue
{
public:
    static const int CONST_MAX_ATTACK_TYPE = 4;
    enum ATTACK_TYPE
    {
        kPhysical = 0,
        kMagical = 1,
        kSiege = 2,
        kHoly = 3
    };
    static const int CONST_MAX_NAME_INDEX = 2;
    enum NAME_INDEX
    {
        kEnName = 0,
        kCnName = 1
    };
    static const char* CONST_ARR_NAME[CONST_MAX_ATTACK_TYPE][CONST_MAX_NAME_INDEX];
    typedef float ARR_ATTACK_VALUES[CONST_MAX_ATTACK_TYPE];

public:
    CAttackValue();
    CAttackValue(int iCount, ATTACK_TYPE eType1, float fValue1, ...);
    virtual ~CAttackValue();

    float getAttack(ATTACK_TYPE eAttackType) const;
    void setAttack(ATTACK_TYPE eAttackType, float fAttack);
    void setAttack(const CAttackValue& roAv);
    void setZero();

    static const char* getName(ATTACK_TYPE eAttackType, int iIndex = 0);

    const CAttackValue& operator=(const CAttackValue& roAv);

    ARR_ATTACK_VALUES m_afAttack;
};

// 护甲数值，由多种类型的护甲组合而成
class CArmorValue
{
public:
    static const int CONST_MAX_ARMOR_TYPE = 6;
    enum ARMOR_TYPE
    {
        kNormal = 0,
        kHeavy = 1,
        kCrystal = 2,
        kWall = 3,
        kHero = 4,
        kHoly = 5
    };
    static const int CONST_MAX_NAME_INDEX = 2;
    enum NAME_INDEX
    {
        kEnName = 0,
        kCnName = 1
    };

    static const char* CONST_ARR_NAME[CONST_MAX_ARMOR_TYPE][CONST_MAX_NAME_INDEX];
    typedef float ARR_ARMOR_VALUES[CONST_MAX_ARMOR_TYPE];

public:
    CArmorValue();
    CArmorValue(int iCount, ARMOR_TYPE eType1, float fValue1, ...);
    virtual ~CArmorValue();

    float getArmor(ARMOR_TYPE eArmorType) const;
    void setArmor(ARMOR_TYPE eArmorType, float fArmor);
    void setArmor(const CArmorValue& roAv);
    void setZero();

    static const char* getName(ARMOR_TYPE eArmorType, int iIndex = 0);

    const CArmorValue& operator=(const CArmorValue& roAv);

    ARR_ARMOR_VALUES m_afArmor;
};

// 攻击-护甲计算系数
extern float g_afAttackArmorTable[CArmorValue::CONST_MAX_ARMOR_TYPE][CAttackValue::CONST_MAX_ATTACK_TYPE];

// 攻击附带的BUFF，包含攻击命中后的附加概率
class CAttackBuff : public CCObject
{
public:
    CAttackBuff();
    virtual ~CAttackBuff();

    virtual bool init(CBuffSkill* pBuff, int iProbability);
    M_CREATE_FUNC_PARAM(CAttackBuff, (CBuffSkill* pBuff, int iProbability), pBuff, iProbability);

public:
    CBuffSkill* m_pBuff;
    int m_iProbability;
};

// 攻击数据，由攻击数值和攻击BUFF链组成，攻击动作中主要传递的数据体
class CAttackData : public CCObject, public CAttackValue
{
public:
    CAttackData();
    virtual ~CAttackData();
    CREATE_FUNC(CAttackData);
    virtual bool init();

    void addBuff(CBuffSkill* pBuff, int iProbability);

    M_SYNTHESIZE(float, m_fAngle, Angle);

public:
    CCArray m_oArrBuff;
};

// f = ax + b，x为基准值，a和b分为计算乘数和加数，应用情形：触发重击将造成自身攻击力的2倍并额外附加30点的伤害；提升自身20%的基础力量值
// 应用该接结构，可轻易解决装备武器/新增BUFF，卸载武器/删除BUFF后的属性一致
class CExtraCoeff
{
public:
    CExtraCoeff();
    CExtraCoeff(float fMulriple, float fAddend);
    virtual ~CExtraCoeff();

    float getValue(float fBase) const;

    M_SYNTHESIZE(float, m_fMulriple, Mulriple);
    M_SYNTHESIZE(float, m_fAddend, Addend);
};

class CUnitForce
{
public:
    CUnitForce();

    bool isAllyOf(const CUnitForce* pForce) const;
    bool isEnemyOf(const CUnitForce* pForce) const;

    void setForceByIndex(int iForceIndex);

    M_SYNTHESIZE(uint32_t, m_dwForceFlag, Force);
    M_SYNTHESIZE(uint32_t, m_dwAllyMaskFlag, Ally);
};

class CForceResouce : public CCObject, public CUnitForce
{
public:
    virtual bool init();
    CREATE_FUNC(CForceResouce);

    virtual bool initWithChangeCallback(CCObject* pSender, SEL_CallFuncO pFunc);
    CREATEWITH_FUNC_PARAM(ChangeCallback, CForceResouce, (CCObject* pSender, SEL_CallFuncO pFunc), pSender, pFunc);

    M_SYNTHESIZE(int, m_iGold, Gold);
    void changeGold(int iChange);
    virtual void onGoldChange(int iChange);

protected:
    CCObject* m_pSender;
    SEL_CallFuncO m_pCallback;
};

// 游戏单位，是攻击数据的发起源，或是攻击数据的受体
class CUnit : public CLife
{
    friend class CSkill;

public:
    enum TRIGGER_INDEX
    {
        kAttackTargetTrigger = 0,
        kAttackedTrigger,
        kDamagedSurfaceTrigger,
        kDamagedInnerTrigger,
        kDamageTargetTrigger,
        kHpChangeTrigger,
        kReviveTrigger,
        kDieTrigger,
        kTickTrigger,
        kDestroyProjectileTrigger
    };
    // 返回Trigger屏蔽标志位
#define UNIT_TRIGGER_MASK(eTriggerIndex) (1 << eTriggerIndex)
    enum TRIGGER_MASK
    {
        kNoMasked = 0,
        kMaskAll = 0xFFFFFFFF,
        kMaskActiveTrigger = UNIT_TRIGGER_MASK(kAttackTargetTrigger) | UNIT_TRIGGER_MASK(kDamageTargetTrigger)
    };

protected:
    inline static bool isMasked(TRIGGER_INDEX eTriggerIndex, uint32_t dwTriggerMask) { return (UNIT_TRIGGER_MASK(eTriggerIndex) & dwTriggerMask) != 0; }

public:
    CUnit();
    virtual ~CUnit();
    //CREATE_FUNC(CUnit);
    virtual bool init();

    // 高层攻击函数，用于最初生成攻击数据，一个攻击动作生成的攻击数据，一般调用该函数
    // 攻击动作，可对目标造成动作，如普通攻击、技能等
    // 攻击数据，描述这次攻击的数据体，详见 CAttackData 定义
    // 内部会自行调用中层、底层攻击函数，对攻击数据进行传递并处理，通常返回处理后的攻击数据，也可以返回 NULL
    // 内部会根据人物属性对攻击数据进行一次变换，如力量加成等
    CAttackData* attackAdv(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);

    // 中层攻击函数
    // 触发 onAttackTarget，
    CAttackData* attackMid(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);

    // 底层攻击函数，目前无逻辑，只是将传递过来的攻击数据返回给上层
    CAttackData* attackBot(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask = kNoMasked);

    // 高层伤害函数，攻击者生成的攻击到达目标后，目标将调用该函数，计算自身伤害
    // 内部会对攻击数据进行向下传递
    // 触发 onAttacked，如果onAttacked返回 NULL，伤害将不会继续向下层函数传递。比如说，闪避成功，伤害无需继续计算
    void damagedAdv(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);

    // 中层伤害函数，攻击数据已经不可消除，但可以改变伤害数据，如一次全额伤害的抵挡，虽然结果上看HP没有受损，但仍然会进行一次0伤害判定
    // 触发 onDamaged
    // 遍历攻击数据携带的BUFF链，根据附着概率对单位自身进行BUFF附加
    // 根据单位属性，进行攻击数据变换，如抗性对攻击数据的影响
    // 根据单位护甲，进行攻击数据中的攻击数值变换
    void damagedMid(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);

    // 底层伤害函数，直接扣除指定量的HP值
    // 触发伤害源的 onDamaeTarget
    // 调用 setHp，从而会触发 onHpChange，可能会触发onDie
    void damagedBot(float fDamage, CUnit* pSource, uint32_t dwTriggerMask = kNoMasked);

    // 为单位添加/删除技能
    void addSkill(CSkill* pSkill);
    void delSkill(CSkill* pSkill);

	//为单位添加包
	void addPackage(CUnitPackage* pPackage);

    // 为单位添加/删除BUFF
    void addBuff(CBuffSkill* pBuff, bool bForce = false);
    void delBuff(CBuffSkill* pBuff, bool bAfterTriggerLoop = true);

    CSkill* getSkill(int iKey);
    CBuffSkill* getBuff(int iKey);
    CBuffSkill* getBuffByType(int iTypeKey);

    virtual void cleanUpTriggers();

    virtual void suspend() = 0;
    virtual void resume() = 0;

    // @override
    virtual void onAttackTarget(CAttackData* pAttack, CUnit* pTarget, uint32_t dwTriggerMask);          // 攻击发出时，攻击者触发
    virtual CAttackData* onAttacked(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);      // 攻击抵达时，受害者触发
    virtual void onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);               // 攻击命中时，受害者触发
    virtual void onDamageTarget(float fDamage, CUnit* pTarget, uint32_t dwTriggerMask);                 // 攻击命中时，攻击者触发
    virtual void onRevive();
    virtual void onDie();
    virtual void onHpChange(float fChanged);
    virtual void onTick(float fDt);
    virtual void onDestroyProjectile(CCProjectileWithAttackData* pProjectile); // 攻击数据消除时会触发，通常由投射物携带攻击数据，二者生存期一致

    int getKey() const;

protected:
    // 根据属性对攻击数据进行变换
    // @override
    virtual CAttackData* transformAttackByAttribute(CAttackData* pAttack);
    virtual CAttackData* transformDamageByAttribute(CAttackData* pAttack);

    // 添加触发器，内部调用
    void addToOnAttackTargetTrigger(CSkill* pSkill);
    void addToOnAttackedTrigger(CSkill* pSkill);
    void addToOnDamagedSurfaceTrigger(CSkill* pSkill);
    void addToOnDamagedInnerTrigger(CSkill* pSkill);
    void addToOnDamageTargetTrigger(CSkill* pSkill);
    void addToOnHpChangeTrigger(CSkill* pSkill);
    void addToOnReviveTrigger(CSkill* pSkill);
    void addToOnDieTrigger(CSkill* pSkill);
    void addToOnTickTrigger(CSkill* pSkill);
    void addToOnDestroyProjectileTrigger(CSkill* pSkill);

    // 删除触发器，内部调用
    void delFromOnAttackTargetTrigger(CSkill* pSkill);
    void delFromOnAttackedTrigger(CSkill* pSkill);
    void delFromOnDamagedSurfaceTrigger(CSkill* pSkill);
    void delFromOnDamagedInnerTrigger(CSkill* pSkill);
    void delFromOnDamageTargetTrigger(CSkill* pSkill);
    void delFromOnHpChangeTrigger(CSkill* pSkill);
    void delFromOnReviveTrigger(CSkill* pSkill);
    void delFromOnDieTrigger(CSkill* pSkill);
    void delFromOnTickTrigger(CSkill* pSkill);
    void delFromOnDestroyProjectileTrigger(CSkill* pSkill);

    // 延迟删除触发器，延迟到整个触发器遍历结束后删除，内部调用
    void delFromOnTickTriggerLater(CSkill* pSkill);

    // 触发器链的触发，内部调用
    void triggerOnAttackTarget(CAttackData* pAttack, CUnit* pTarget);
    CAttackData* triggerOnAttacked(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedSurface(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamagedInnerTrigger(CAttackData* pAttack, CUnit* pSource);
    void triggerOnDamageTargetTrigger(float fDamage, CUnit* pTarget);
    void triggerOnHpChange(float fChanged);
    void triggerOnRevive();
    void triggerOnDie();
    void triggerOnTick(float fDt);
    void triggerOnDestroyProjectileTrigger(CCProjectileWithAttackData* pProjectile);

    static void addToTrigger(CCArray& roTrigger, CSkill* pSkill);
    static void delFromTrigger(CCArray& roTrigger, CSkill* pSkill);

    void beginTrigger();
    void endTrigger();
    bool isTriggerFree() const;


public:
    const int m_iKey;
    CCArray m_oArrOnAttackTargetChain;
    CCArray m_oArrOnAttackedChain;
    CCArray m_oArrOnDamagedSurfaceChain;
    CCArray m_oArrOnDamagedInnerChain;
    CCArray m_oArrOnDamageTargetChain;
    CCArray m_oArrOnHpChangeChain;
    CCArray m_oArrOnReviveChain;
    CCArray m_oArrOnDieChain;
    CCArray m_oArrOnTickChain;
    CCArray m_oArrOnTickDelChain;
    CCArray m_oArrOnDestroyProjectileChain;

    
    //CArmorValue m_oArmorValue;
    M_SYNTHESIZE(CArmorValue::ARMOR_TYPE, m_eArmorType, ArmorType);
    M_SYNTHESIZE(float, m_fBaseArmorValue, BaseArmorValue);
    
    //CExtraValue m_aoExtraArmorValue[M_MAX_ARMOR_TYPE_COUNT];
    CExtraCoeff m_oExtraArmorValue;
    CExtraCoeff m_oExtraStrength;
    CExtraCoeff m_oExtraIntellect;
    CExtraCoeff m_oExtraAgility;
    CExtraCoeff m_oExtraSpirit;
    CCArray m_oArrBuff;
    CCArray m_oArrSkill;
    M_SYNTHESIZE(float, m_fStrength, Strength);
    M_SYNTHESIZE(float, m_fIntellect, Intellect);
    M_SYNTHESIZE(float, m_fAgility, Agility);
    M_SYNTHESIZE(float, m_fSpirit, Spirit);
    int m_iTriggering;

	CUnitPackage* m_pUnitPackage;
};

class CGameUnit;

class CCGameUnitSprite : public CCSprite
{
public:
    CCGameUnitSprite();
    virtual ~CCGameUnitSprite();
    virtual bool init();
    virtual bool initWithSpriteFrame(CCSpriteFrame *pSpriteFrame, const CCPoint& roAnchor = ccp(0.5f, 0.5f));
    virtual void setDisplayFrame(CCSpriteFrame *pNewFrame);
    virtual void draw();

    virtual void setPosition(const CCPoint& roPos);
    virtual CCNode* getShadowNode();
    M_SYNTHESIZE(CGameUnit*, m_pControler, Controler);

protected:
    virtual void updateShadowNode();

protected:
    CCNode m_oShadowNode;
};

class CProjectile;
class CUnitInfo;

class CGameUnit : public CUnit, public CUnitForce, public CLevelExp
{
public:
    enum DOING_FLAG
    {
        kMoving = 1,
        kWithHostility = 1 << 1,
        kAttacking = 1 << 2,
        kSuspended = 1 << 3

    };

    enum ACTION_TAG
    {
        kActMoveTo,
        kActMove,
        kActAttack,
        kActAttackEffect,
        kActDie,
        kAct1,
        kAct2,
        kAct3,
        kAct4,
        kAct5,
        kAct6
    };

    static const int CONST_MAX_ANIMATION = 8;
    enum ANIMATION_INDEX
    {
        kAnimationMove = 0,
        kAnimationDie = 1,
        kAnimationAct1 = 2,
        kAnimationAct2 = 3,
        kAnimationAct3 = 4,
        kAnimationAct4 = 5,
        kAnimationAct5 = 6,
        kAnimationAct6 = 7,
    };
    typedef vector<ANIMATION_INDEX> VEC_ANI;

    struct ARR_ANIMATION_INFO
    {
        ARR_ANIMATION_INFO() : fDelay(0.0) {}
        ARR_ANIMATION_INFO(const char* pAnimation_, float fDelay_) : sAnimation(pAnimation_), fDelay(fDelay_) {}
        string sAnimation;
        float fDelay;
    };
    typedef struct ARR_ANIMATION_INFO UNIT_ANI_INFOS[CONST_MAX_ANIMATION];

    enum WEAPON_TYPE
    {
        kWTClose = 0,
        kWTInstant = 1,
        kWTDelayed = 2
    };

public:
    CGameUnit();
    virtual bool init();
    CREATE_FUNC_PARAM(CGameUnit, ());

    virtual bool initWithName(const char* pUnit, const CCPoint& roAnchor = ccp(0.5f, 0.5f));
    CREATEWITH_FUNC_PARAM(Name, CGameUnit, (const char* pUnit, const CCPoint& roAnchor = ccp(0.5f, 0.5f)), pUnit, roAnchor);

    virtual bool initWithInfo(const CUnitInfo& roUnitInfo);
    CREATEWITH_FUNC_PARAM(Info, CGameUnit, (const CUnitInfo& roUnitInfo), roUnitInfo);

public: // Move
    static const float CONST_MIN_MOVE_SPEED;
    static const float CONST_MAX_MOVE_SPEED;
    static const float CONST_MIN_MOVE_SPEED_MULRIPLE;
    virtual void setBaseMoveSpeed(float fMoveSpeed);
    virtual float getBaseMoveSpeed() const;
    virtual float getRealMoveSpeed() const;
    virtual void setExMoveSpeed(const CExtraCoeff& roExMoveSpeed);
    virtual CExtraCoeff getExMoveSpeed() const;
    virtual void updateMoveToAnimationSpeed(float fRealMoveSpeed);
    virtual void moveTo(const CCPoint& roPos, bool bWithHostility = false, bool bCancelAttack = true, bool bAutoFlipX = true);
    virtual void followTo(int iTargetKey, bool bWithHostility = false, bool bCancelAttack = true, bool bAutoFlipX = true, float fMaxOffsetY = 0.0);
    virtual void stopMove();
    virtual const CCPoint& getLastMoveToTarget() const;

protected:
    virtual void onActMoveEnd(CCNode* pNode);

public: // Attack
    static const float CONST_MIN_ATTACK_SPEED_INTERVAL;
    static const float CONST_MIN_ATTACK_SPEED_MULRIPLE;
    static const float CONST_MAX_ATTACK_SPEED_MULRIPLE;
    static const float CONST_MAX_ATTACK_BUFFER_RANGE;
    static const float CONST_MAX_CLOSE_ATTACK_Y_RANGE;
    M_SYNTHESIZE(float, m_fAttactRange, AttackRange);
    M_SYNTHESIZE(float, m_fAttactMinRange, AttackMinRange);
    virtual void setBaseAttackInterval(float fAttackInterval);
    virtual float getBaseAttackInterval() const;
    virtual float getBaseAttackSpeed() const;
    virtual float getRealAttackSpeed() const;
    virtual float getRealAttackInterval() const;
    virtual void setExAttackSpeed(const CExtraCoeff& roExAttackSpeed);
    virtual CExtraCoeff getExAttackSpeed() const;
    virtual void updateAttackAnimationSpeed(float fRealAttackInterval);
    virtual void attack(int iTargetKey);
    virtual void stopAttack();
    virtual void setBaseAttackValue(const CAttackValue& roAttackValue);
    virtual float getBaseAttackValue(CAttackValue::ATTACK_TYPE eAttackType) const;
    virtual float getRealAttackValue(CAttackValue::ATTACK_TYPE eAttackType) const;
    virtual void setExAttackValue(CAttackValue::ATTACK_TYPE eAttackType, const CExtraCoeff& roExAttackValue);
    virtual CExtraCoeff getExAttackValue(CAttackValue::ATTACK_TYPE eAttackType) const;
    virtual int getLastAttackTarget() const;
    M_SYNTHESIZE(float, m_fAttackEffectDelay, AttackEffectDelay);
    M_SYNTHESIZE(WEAPON_TYPE, m_eWeaponType, WeaponType);
    M_SYNTHESIZE(float, m_fHalfOfWidth, HalfOfWidth);
    M_SYNTHESIZE(float, m_fHalfOfHeight, HalfOfHeight);
    M_SYNTHESIZE(float, m_fAttackCD, AttackCD);
    virtual void setTemplateProjectile(CProjectile* pProjectile);
    virtual CProjectile* getTemplateProjectile();
    M_SYNTHESIZE(float, m_fProjectileMoveSpeed, ProjectileMoveSpeed);
    M_SYNTHESIZE(float, m_fProjectileScale, ProjectileScale);
    M_SYNTHESIZE(float, m_fProjectileMaxOffsetY, ProjectileMaxOffsetY);
    M_SYNTHESIZE(float, m_fHostilityRange, HostilityRange);
    M_SYNTHESIZE(float, m_fProjectileBirthOffsetX, ProjectileBirthOffsetX);
    M_SYNTHESIZE(float, m_fProjectileBirthOffsetY, ProjectileBirthOffsetY);

protected:
    virtual void onActAttackEffect(CCNode* pNode);
    virtual void onActAttackEnd(CCNode* pNode);
    bool checkAttackDistance(const CCPoint& roPos, CGameUnit* pTarget);
    void moveToAttackPosition(CGameUnit* pTarget);
    
public:
    static const float CONST_MAX_REWARD_RANGE;
    virtual void setName(const char* pUnit, const CCPoint& roAnchor = ccp(0.5f, 0.5f));
    virtual const char* getName() const;
    virtual void setFrame(const char* pFrame);
    virtual void setDefaultFrame();
    virtual void prepareMoveAnimation(const char* pAnimation, float fDelay);
    virtual void prepareAttackAnimation(int iAttackAniCount, ANIMATION_INDEX eAnimation1, const char* pAnimation1, float fDelay1, ...);
    virtual void prepareDieAnimation(const char* pAnimation, float fDelay);
    virtual void prepareAnimation(ANIMATION_INDEX eAnimation, const char* pAnimation, float fDelay);
    virtual void setAnimation(const char* pAnimation, float fDelay, int iRepeat, float fSpeed, ACTION_TAG eTag, CCFiniteTimeAction* pEndAction = NULL);
    virtual void setAnimation(ANIMATION_INDEX eAnimation, int iRepeat, float fSpeed, ACTION_TAG eTag, CCFiniteTimeAction* pEndAction = NULL);
    
    virtual void suspend();
    virtual void resume();

    virtual void startDoing(uint32_t dwMask);
    virtual void endDoing(uint32_t dwMask);
    virtual bool isDoingOr(uint32_t dwMask) const;
    virtual bool isDoingAnd(uint32_t dwMask) const;
    virtual bool isDoingNothing() const;

    virtual void onTick(float fDt);
    virtual void onDie();
    virtual void onDamaged(CAttackData* pAttack, CUnit* pSource, uint32_t dwTriggerMask);

    virtual CCGameUnitSprite* getSprite();
    virtual void setPosition(const CCPoint& roPos);
    virtual const CCPoint& getPosition();
    virtual CCNode* getShadowNode();
    //M_SYNTHESIZE(float, m_fScale, Scale);
    virtual void removeAllSpriteFromTheirParents();
    virtual float getDistance(const CCPoint& roPos);
    virtual float getDistance(CGameUnit* pTarget); // may be unsafe
    virtual void turnTo(bool bLeft);
    M_SYNTHESIZE(int, m_iOffsetZ, OffsetZ);
    virtual void setFixed(bool bFixed);
    virtual bool isFixed() const;
    M_SYNTHESIZE(int, m_iRewardGold, RewardGold);
    M_SYNTHESIZE(int, m_iRewardExp, RewardExp);
    virtual void setForceResource(CForceResouce* pRes);
    virtual CForceResouce* getForceResource();

protected:
    virtual void turnTo(CGameUnit* pTarget);
    virtual void onActDieEnd(CCNode* pNode);

protected:
    CCGameUnitSprite m_oSprite;
    string m_sUnit;
    uint32_t m_dwDoingFlags;
    UNIT_ANI_INFOS m_astAniInfo;

    float m_fBaseMoveSpeed;
    CExtraCoeff m_oExMoveSpeed;
    CCPoint m_oLastMoveToTarget;

    float m_fBaseAttackInterval;
    CExtraCoeff m_oExAttackSpeed;
    
    VEC_ANI m_vecAttackAniIndex;
    CAttackValue m_oBaseAttackValue;
    CExtraCoeff m_aoExAttackValue[CAttackValue::CONST_MAX_ATTACK_TYPE];
    int m_iLastAttackTarget;
    CProjectile* m_pTemplateProjectile;
    bool m_bIsFixed;
    CForceResouce* m_pRes;
};

class CProjectile : public CGameUnit
{
public:
    enum PROJECTILE_TYPE
    {
        kFollow,
        kLightning,
        kRange
    };

public:
    CProjectile();
    virtual ~CProjectile();
    virtual bool init();
    CREATE_FUNC_PARAM(CProjectile, ());
    virtual bool initWithName(const char* pProjectile, const CCPoint& roAnchor = ccp(0.5f, 0.5f));
    CREATEWITH_FUNC_PARAM(Name, CProjectile, (const char* pProjectile, const CCPoint& roAnchor = ccp(0.5f, 0.5f)), pProjectile, roAnchor);

    virtual CAttackData* getAttackData() const;
    virtual void setAttackData(CAttackData* pAttackData);

    M_SYNTHESIZE(int, m_iGeneration, Generation);
    virtual void setTarget(int iTarget);
    virtual void setTarget(CGameUnit* pTarget);
    virtual int getTarget() const;
    M_SYNTHESIZE(int, m_iOwner, Owner);
    M_SYNTHESIZE(PROJECTILE_TYPE, m_eProjectileType, ProjectileType);
    

    virtual void onDie();
    virtual CCObject* copyWithZone(CCZone* pZone);

protected:
    virtual void onActMoveEnd(CCNode* pNode);
    virtual void onActDieEnd(CCNode* pNode);

protected:
    CAttackData* m_pAttackData;
    int m_iTarget;
    
};

class CCUnitLayer : public CCLayerColor
{
public:
    CCUnitLayer();
    virtual bool init();
    CREATE_FUNC(CCUnitLayer);
    virtual bool initWithColor(const ccColor4B& color);
    CREATEWITH_FUNC_PARAM(Color, CCUnitLayer, (const ccColor4B& color), color);
    virtual void onEnter();
    virtual void onExit();

    virtual void setUnitTickInterval(float fInterval);
    virtual float getUnitTickInterval() const;

    virtual void onTickEvent(float fDt);
    virtual void addUnit(CGameUnit* pUnit);
    virtual void addProjectile(CProjectile* pProjectile);

protected:
    float m_fUnitTickInterval;
};

class CCWinUnitLayer : public CCUnitLayer
{
public:
    static const float CONST_MIN_MOVE_DELTA;
    static const float CONST_MAX_CAN_MOVE_DURATION;

public:
    CCWinUnitLayer();
    virtual bool init();
    CREATE_FUNC(CCWinUnitLayer);
    virtual bool initWithColor(const ccColor4B& color);
    CREATEWITH_FUNC_PARAM(Color, CCWinUnitLayer, (const ccColor4B& color), color);

    // default implements are used to call script callback if exist
    virtual void setBackGroundSprite(CCSprite* pSprite);
    virtual void setBackGroundSprite(CCSprite* pSprite, int zOrder, int tag);
    virtual void setBufferEffectParam(float fMoveK, float fBuffRange, float fEdgeK);
    virtual float getTouchMovedDuration() const;
    virtual float getTouchMovedDistance() const;
    virtual float getTouchMovedRadian() const;
    virtual bool isSlideAction() const;
    virtual bool isClickAction() const;

    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void touchDelegateRetain();
    virtual void touchDelegateRelease();
    virtual void bufferWindowEffect(float fDt);

    void adjustWinPos(CCPoint& roPos);

protected:
    bool m_bIsTouching;
    float m_fMoveK;
    float m_fBuffRange;
    float m_fEdgeK;
    float m_fMoveDelta;
    CCPoint m_oMoveStart;
    float m_fTouchMovedDuration;
    float m_fMoveR;
    bool m_bCanMove;
};

class CUnitPath;

class CUnitGroup : public CCObject
{
public:
    typedef bool (*CONDITIONFUNC)(CGameUnit* pUnit, void* pParam);

public:
    CUnitGroup();

    virtual bool init();
    CREATE_FUNC_PARAM(CUnitGroup, ());
    virtual bool initWithUnitsInRange(CUnitGroup* pSource, const CCPoint& roPos, float fRadius, int iMaxCount = INFINITE, CONDITIONFUNC pBoolFunc = NULL, void* pParam = NULL);
    CREATEWITH_FUNC_PARAM(UnitsInRange, CUnitGroup, (CUnitGroup* pSource, const CCPoint& roPos, float fRadius, int iMaxCount = INFINITE, CONDITIONFUNC pBoolFunc = NULL, void* pParam = NULL), pSource, roPos, fRadius, iMaxCount, pBoolFunc, pParam);
    virtual bool initWithCondition(CUnitGroup* pSource, int iMaxCount = INFINITE, CONDITIONFUNC pBoolFunc = NULL, void* pParam = NULL);
    CREATEWITH_FUNC_PARAM(Condition, CUnitGroup, (CUnitGroup* pSource, int iMaxCount = INFINITE, CONDITIONFUNC pBoolFunc = NULL, void* pParam = NULL), pSource, iMaxCount, pBoolFunc, pParam);

    CGameUnit* getUnitByIndex(int iIndex);
    CGameUnit* getUnitByKey(int iKey);
    CGameUnit* getRandomUnit();
    CGameUnit* getNearestUnitInRange(const CCPoint& roPos, float fRadius, CONDITIONFUNC pBoolFunc = NULL, void* pParam = NULL);
    void addUnit(CGameUnit* pUnit);
    void delUnit(CGameUnit* pUnit);
    CGameUnit* copyUnit(int iKey);

    CCArray* getUnitsArray();
    CUnitGroup* getUnitsInRange(const CCPoint& roPos, float fRadius, int iMaxCount = INFINITE, CONDITIONFUNC pBoolFunc = NULL, void* pParam = NULL);
    CUnitGroup* getUnits(int iMaxCount = INFINITE, CONDITIONFUNC pBoolFunc = NULL, void* pParam = NULL);
    void cleanUnits();
    void addUnits(CUnitGroup* pUnits);
    void addUnits(CCArray* pUnits);
    void delUnits(CUnitGroup* pUnits);
    void delUnits(CCArray* pUnits);
    int getUnitsCount();

    virtual void setRangePosition(const CCPoint& roPos, float fRadius);
    virtual void turnTo(bool bLeft);
    virtual void moveTo(const CCPoint& roPos, bool bWithHostility = false, bool bCancelAttack = true, bool bAutoFlipX = true);
    virtual void followTo(int iTargetKey, bool bWithHostility = false, bool bCancelAttack = true, bool bAutoFlipX = true, float fMaxOffsetY = 0.0);
    virtual void stopMove();
    virtual void attack(int iTargetKey);
    virtual void stopAttack();
    virtual void moveAlongPath(CUnitPath* pPath, bool bRestart = false, bool bWithHostility = true, float fBuffArrive = FLT_EPSILON);

    void damagedAdv(CAttackData* pAttack, CUnit* pSource);
    void damagedMid(CAttackData* pAttack, CUnit* pSource);
    void damagedBot(float fDamage, CUnit* pSource);
    void addSkill(CSkill* pSkill);
    void addPackage(CUnitPackage* pPackage);
    void addBuff(CBuffSkill* pBuff, bool bForce = false);

    static bool isLivingAllyOf(CGameUnit* pUnit, CUnitForce* pParam);
    static bool isLivingEnemyOf(CGameUnit* pUnit, CUnitForce* pParam);


protected:
    CCArray m_oArrUnits;
};

class CUnitPath : public CCObject
{
public:
    typedef vector<CCPoint> VEC_POINTS;

public:
    virtual bool init();
    CREATE_FUNC_PARAM(CUnitPath, ());
    virtual bool initWithPoints(const VEC_POINTS& roVecPoints);
    CREATEWITH_FUNC_PARAM(Points, CUnitPath, (const VEC_POINTS& roVecPoints), roVecPoints);
    virtual bool initWithFile(const char* pFileName);
    CREATEWITH_FUNC_PARAM(File, CUnitPath, (const char* pFileName), pFileName);

    void addPoint(const CCPoint& roPos);
    void addPoints(const char* pFileName);
    void saveAsFile(const char* pFileName);
    virtual const CCPoint* getCurTargetPoint(uint32_t dwCurPos);
    virtual void arriveCurTargetPoint(uint32_t& rCurPos);

public:
    VEC_POINTS m_vecPoints;
};

class CPathGameUnit : public CGameUnit
{
public:
    virtual ~CPathGameUnit();
    virtual bool initWithName(const char* pUnit, const CCPoint& roAnchor = ccp(0.5f, 0.5f));
    CREATEWITH_FUNC_PARAM(Name, CPathGameUnit, (const char* pUnit, const CCPoint& roAnchor = ccp(0.5f, 0.5f)), pUnit, roAnchor);
    virtual bool initWithInfo(const CUnitInfo& roUnitInfo);
    CREATEWITH_FUNC_PARAM(Info, CPathGameUnit, (const CUnitInfo& roUnitInfo), roUnitInfo);

    M_SYNTHESIZE(uint32_t, m_dwCurPos, CurPos);

    virtual void onTick(float fDt);
    virtual void moveAlongPath(CUnitPath* pPath, bool bRestart = false, bool bWithHostility = true, float fBuffArrive = 5.0);

protected:
    CUnitPath* m_pCurPath;
    bool m_bWithHostility;
    float m_fBuffArrive;
};

class CUnitInfo
{
public:
    typedef CInitArray<CGameUnit::ANIMATION_INDEX> ARR_ATTACK_ANI;

public:
    CUnitInfo(
        const char* pName,
        const CCPoint& roAnchor,
        float fHalfOfWidth,
        float fHalfOfHeight,
        float fScale,
        float fActMoveDelay,
        float fActDieDelay,
        float fAct1Delay,
        float fAct2Delay,
        float fAct3Delay,
        float fAct4Delay,
        float fAct5Delay,
        float fAct6Delay,
        const ARR_ATTACK_ANI& roArrAttackAnis,
        float fBaseMoveSpeed,
        float fBaseAttackInterval,
        float fAttackEffectDelay,
        float fAttackMinRange,
        float fAttackRange,
        float fHostilityRange,
        CGameUnit::WEAPON_TYPE eWeaponType,
        int iProjectileKey,
        float fProjectileMoveSpeed,
        float fProjectileScale,
        float fProjectileMaxOffsetY,
        float fProjectileBirthOffsetX,
        float fProjectileBirthOffsetY,
        const CAttackValue& roBaseAttackValue,
        CArmorValue::ARMOR_TYPE eArmorType,
        float fBaseArmorValue,
        int iForceIndex,
        uint32_t dwForceAlly,
        float fMaxHp,
        bool bIsFixed,
        int iRewardGold,
        int iRewardExp
        );

    virtual ~CUnitInfo(void);

public:
    string m_sName;
    CCPoint m_oAnchor;
    float m_fHalfOfWidth;
    float m_fHalfOfHeight;
    float m_fScale;
    float m_fActMoveDelay;
    float m_fActDieDelay;
    float m_fAct1Delay;
    float m_fAct2Delay;
    float m_fAct3Delay;
    float m_fAct4Delay;
    float m_fAct5Delay;
    float m_fAct6Delay;
    CGameUnit::VEC_ANI m_vecAttackAni;
    float m_fBaseMoveSpeed;
    float m_fBaseAttackInterval;
    float m_fAttackEffectDelay;
    float m_fAttackMinRange;
    float m_fAttackRange;
    float m_fHostilityRange;
    CGameUnit::WEAPON_TYPE m_eWeaponType;
    int m_iProjectileKey;
    float m_fProjectileMoveSpeed;
    float m_fProjectileScale;
    float m_fProjectileMaxOffsetY;
    float m_fProjectileBirthOffsetX;
    float m_fProjectileBirthOffsetY;
    CAttackValue m_oBaseAttackValue;
    CArmorValue::ARMOR_TYPE m_eArmorType;
    float m_fBaseArmorValue;
    int m_iForceIndex;
    uint32_t m_dwForceAlly;
    float m_fMaxHp;
    bool m_bIsFixed;
    int m_iRewardGold;
    int m_iRewardExp;
};

class CUnitInfoPatch
{
public:
    static const uint16_t CONST_FILE_DATA_SIZE;
    typedef vector<int> VEC_SKILL_INDEX;

public:
    CUnitInfoPatch();

    virtual bool initWithUnitInfo(int iUnitInfoIndex);
    virtual bool initWithFileStream(CGameFile* pFile);
    virtual void writeToFileStream(FILE* pFile);

public:
    int m_iOrgUnitIndex;
    char m_szName[32];
    float m_fBaseMoveSpeed;
    float m_fBaseAttackInterval;
    float m_fAttackMinRange;
    float m_fAttackRange;
    float m_fHostilityRange;
    float m_fProjectileMoveSpeed;
    float m_fProjectileScale;
    CAttackValue m_oBaseAttackValue;
    CArmorValue::ARMOR_TYPE m_eArmorType;
    float m_fBaseArmorValue;
    float m_fMaxHp;
    bool m_bIsFixed;
    int m_iRewardGold;
    int m_iRewardExp;
    VEC_SKILL_INDEX m_vecSkills;
};

class CHeroUnit;

// 存在一个全局共享的，用于创建英雄等常驻单位，但是每个场景需要单独create一个，用于创建当前关卡的单位
class CUnitInfoPatchManager : public CCObject
{
public:
    typedef vector<CUnitInfoPatch> VEC_PATCHES;

public:
    virtual bool init();
    CREATE_FUNC_PARAM(CUnitInfoPatchManager, ());

    virtual bool initWithFile(const char* pFileName);
    CREATEWITH_FUNC_PARAM(File, CUnitInfoPatchManager, (const char* pFileName), pFileName);

    static CUnitInfoPatchManager* sharedUnitInfoPatchManager();

    int addPatch(const CUnitInfoPatch& roPatch); // return index
    int addNewPatch(int iUnitIndex); // return index
    int addPatches(const char* pFileName);
    void saveAsFile(const char* pFileName);
    void setPatch(int iIndex, const CUnitInfoPatch& roPatch);

    int count() const;
    CUnitInfoPatch* patchByIndex(int iIndex);

    CGameUnit* unitByIndex(int iIndex);
    CPathGameUnit* pathUnitByIndex(int iIndex);
    CHeroUnit* heroByIndex(int iIndex);

protected:
    void patch(CGameUnit* pUnit, CUnitInfoPatch* pPatch);

public:
    VEC_PATCHES m_vecPatches;
};

class CUnitManager : public CCObject
{
public:
    typedef map<int, CUnitInfo> UNIT_MAP;

public:
    CUnitManager();
    virtual ~CUnitManager();

    virtual bool init();
    CREATE_FUNC_PARAM(CUnitManager, ());

    static CUnitManager* sharedUnitManager();

    void addUnitInfo(int iUnitInfoIndex, const CUnitInfo& roUnitInfo);

    CGameUnit* unitByInfo(int iUnitInfoIndex);
    CPathGameUnit* pathUnitByInfo(int iUnitInfoIndex);
    CHeroUnit* heroByInfo(int iUnitInfoIndex);

    CUnitInfo* unitInfoByIndex(int iUnitIndex);

public:
    static CUnitManager* m_pInst;
    UNIT_MAP m_mapUnitInfo;
};

class CProjectileManager : public CCObject
{
public:
    CProjectileManager();
    virtual ~CProjectileManager();

    virtual bool init();
    CREATE_FUNC_PARAM(CProjectileManager, ());

    static CProjectileManager* sharedProjectileManager();
    virtual void cleanUp();

    void addProjectile(CProjectile* pProjectile);
    CProjectile* getProjectile(int iKey);
    CProjectile* copyProjectile(int iKey);

public:
    static CProjectileManager* m_pInst;
    CUnitGroup m_oArrProjectile;
};
