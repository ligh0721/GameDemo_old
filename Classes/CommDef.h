#pragma once

#define M_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: inline virtual varType get##funName(void) const { return varName; }

#define M_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline virtual const varType& set##funName(void) const { return varName; }

#define M_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: inline virtual varType get##funName(void) const { return varName; }\
public: inline void set##funName(varType var){ varName = var; }

#define M_SYNTHESIZE_STR(varName, funName)\
protected: string varName;\
public: inline virtual const char* get##funName(void) const { return varName.c_str(); }\
public: inline void set##funName(const char* var){ varName = var; }

#define M_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: inline virtual const varType& get##funName(void) const { return varName; }\
public: inline virtual void set##funName(const varType& var){ varName = var; }


#define M_LOGIC_CONSTRUCTOR(sub, super) \
    sub(int iKey): super(iKey) {}

#define M_CREATE_FUNC_PARAM(__TYPE__, __PARAM__, ...) \
    static __TYPE__* create __PARAM__ \
    { \
        __TYPE__ *pRet = new __TYPE__(); \
        if (pRet && pRet->init(__VA_ARGS__)) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
        return NULL; \
        } \
    }

#define DECLARE_CREATE_WITH_SPRITE_FRAME_NAME(type) \
    static type* createWithSpriteFrameName(const char *pszSpriteFrameName)

#define IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME(type) \
    type* type::createWithSpriteFrameName(const char *pszSpriteFrameName) \
    { \
        type* pSprite = new type(); \
        if (pSprite && pSprite->initWithSpriteFrameName(pszSpriteFrameName)) \
        { \
            pSprite->autorelease(); \
            return pSprite; \
        } \
        CC_SAFE_DELETE(pSprite); \
        return NULL; \
    }

#define DECLARE_CREATE_WITH_SPRITE_FRAME_NAME_LOGIC(type) \
    static type* createWithSpriteFrameNameAndLogic(const char *pszSpriteFrameName, CBullet* pBullet)

#define IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME_LOGIC(type) \
    type* type::createWithSpriteFrameNameAndLogic(const char *pszSpriteFrameName, CBullet* pBullet) \
    { \
        type* pSprite = new type(); \
        if (pSprite && pSprite->initWithSpriteFrameNameAndLogic(pszSpriteFrameName, pBullet)) \
        { \
            pSprite->autorelease(); \
            return pSprite; \
        } \
        CC_SAFE_DELETE(pSprite); \
        return NULL; \
    }

#define DECLARE_CREATE_WITH_SPRITE_FRAME_NAME_GUN(type) \
    static type* createWithSpriteFrameNameAndLogicGun(const char *pszSpriteFrameName, CTank* pTank, CCSprite* pGunSprite)

#define IMPLIMENT_CREATE_WITH_SPRITE_FRAME_NAME_GUN(type) \
    type* type::createWithSpriteFrameNameAndLogicGun(const char *pszSpriteFrameName, CTank* pTank, CCSprite* pGunSprite) \
    { \
        type* pSprite = new type(); \
        if (pSprite && pSprite->initWithSpriteFrameNameAndLogicGun(pszSpriteFrameName, pTank, pGunSprite)) \
        { \
            pSprite->autorelease(); \
            return pSprite; \
        } \
        CC_SAFE_DELETE(pSprite); \
        return NULL; \
    }


#define CREATE_FUNC_PARAM(__TYPE__, __PARAM__, ...) \
    static __TYPE__* create __PARAM__ \
    { \
        __TYPE__* pRet = new __TYPE__(); \
        if (pRet && pRet->init(__VA_ARGS__)) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
        } \
    }

#define CREATEWITH_FUNC_PARAM(__WITH__, __TYPE__, __PARAM__, ...) \
    static __TYPE__* createWith##__WITH__ __PARAM__ \
    { \
        __TYPE__* pRet = new __TYPE__(); \
        if (pRet && pRet->initWith##__WITH__(__VA_ARGS__)) \
        { \
           pRet->autorelease(); \
           return pRet; \
        } \
        else \
        { \
           delete pRet; \
           pRet = NULL; \
           return NULL; \
        } \
    }

#define CREATE_INITWITH_FUNC_PARAM(__WITH__, __TYPE__, __PARAM__, ...) \
    static __TYPE__* create __PARAM__ \
    { \
        __TYPE__* pRet = new __TYPE__(); \
        if (pRet && pRet->initWith##__WITH__(__VA_ARGS__)) \
        { \
            pRet->autorelease(); \
            return pRet; \
        } \
        else \
        { \
            delete pRet; \
            pRet = NULL; \
            return NULL; \
        } \
    }

#define PTM_RATIO 32

#define M_SKILL_PATH(name)                                      name"/BTN"name".png"
#define M_SKILL_DOWN_PATH(name)                                 name"/BTN"name"DOWN.png"
#define M_SKILL_DIS_PATH(name)                                  name"/DISBTN"name".png"
#define M_SKILL_ON_PATH(name)                                   name"/BTN"name"ON.png"
#define M_CREATE_SKILL(name, unit, skill, layer)                       CCSkillButtonAdvance::create(M_SKILL_PATH(name), M_SKILL_DOWN_PATH(name), M_SKILL_DIS_PATH(name), M_SKILL_PATH("white"), "mask/mask.png", (unit), (skill), (layer))

#define M_RAND_HIT(probability) (rand() % 100 < (probability))

#define M_GET_TYPE_KEY \
    public: inline virtual int getTypeKey() const\
    { \
        static const int iTypeKey = CGameManager::keygen(); \
        return iTypeKey; \
    }

#define M_SKILL_BUTTON_CANCEL(node) dynamic_cast<CCSkillButtonBase*>((node))->setClickRetCode(-1)

#define M_DEF_DR(var) CCDirector* var = CCDirector::sharedDirector()
#define M_DEF_FU(var) CCFileUtils* var = CCFileUtils::sharedFileUtils()
#define M_DEF_GM(var) CGameManager* var = CGameManager::sharedGameManager()
#define M_DEF_FC(var) CCSpriteFrameCache* var = CCSpriteFrameCache::sharedSpriteFrameCache()
#define M_DEF_AC(var) CCAnimationCache* var = CCAnimationCache::sharedAnimationCache()
#define M_DEF_SM(var) CCSkillManager* var = CCSkillManager::sharedSkillManager()
#define M_DEF_PM(var) CProjectileManager* var = CProjectileManager::sharedProjectileManager()
#define M_DEF_UM(var) CUnitManager* var = CUnitManager::sharedUnitManager()
#define M_DEF_UPM(var) CUnitInfoPatchManager* var = CUnitInfoPatchManager::sharedUnitInfoPatchManager()
#define M_DEF_TB(var) CTowerBuilder* var = CTowerBuilder::sharedTowerBuilder()
#define M_DEF_OU(var) COrgUnitInfo* var = COrgUnitInfo::sharedOrgUnitInfo()
#define M_DEF_OS(var) COrgSkillInfo* var = COrgSkillInfo::sharedOrgSkillInfo()
#define M_DEF_PI(var) CPlayerInfo* var = CPlayerInfo::sharedPlayerInfo()

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#define CONDITION(func) ((CUnitGroup::CONDITIONFUNC)(func))

template <typename TYPE, typename ASTYPE = int>
class CInitArray
{
public:
    inline CInitArray(int iCount, ...)
        : m_iCount(iCount)
    {
        m_pData = new TYPE[iCount];
        va_list argv;
        va_start(argv, iCount);
        for (int i = 0; i < iCount; ++i)
        {
            m_pData[i] = (TYPE)va_arg(argv, int);
        }
        va_end(argv);
    }

    inline ~CInitArray()
    {
        delete[] m_pData;
    }

    inline operator TYPE*()
    {
        return m_pData;
    }

    inline operator const TYPE*() const
    {
        return m_pData;
    }

    inline int count() const
    {
        return m_iCount;
    }

protected:
    TYPE* m_pData;
    int m_iCount;
};



#ifdef WIN32
const char* GBKToUTF8(const char* pGBKStr);
#define UTEXT(str) GBKToUTF8(str)
#else
#define UTEXT(str) str
#endif

