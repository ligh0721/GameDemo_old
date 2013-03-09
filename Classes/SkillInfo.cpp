#include "CommInc.h"

#include "GameDisplay.h"
#include "Unit.h"
#include "Skill.h"
#include "SkillInfo.h"
#include "GameCtrl.h"
#include "UnitInfo.h"


COrgSkillInfo g_oOrgSkillInfo;

COrgSkillInfo::COrgSkillInfo()
{
}

bool COrgSkillInfo::init()
{
    M_DEF_GM(pGm);
    M_DEF_SM(pSm);
    M_DEF_PM(pPm);
    M_DEF_OU(pOu);
    
    CSkill* pSkill;
    int iKey;
    //stringstream ss;
    
    pSkill = CSpeedBuff::create(1, true, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CAttackBuffMakerPas::create(100, iKey, 1, CExtraCoeff(1, 0));
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSlowDown1].iIndex = iKey;
    m_mapSkills[kSlowDown1].sName = "减速";
    m_mapSkills[kSlowDown1].sDesc = "降低目标50%的移动速度和50%的攻击速度";
    
    pSkill = CHpChangePas::create(0.1, 0.2, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange1].iIndex = iKey;
    m_mapSkills[kHpChange1].sName = "自动痊愈";
    m_mapSkills[kHpChange1].sDesc = "每秒恢复自身最大生命的2%";
    
    pSkill = CHpChangePas::create(0.1, 0.02, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange2].iIndex = iKey;
    m_mapSkills[kHpChange2].sName = "自动痊愈";
    m_mapSkills[kHpChange2].sDesc = "每秒恢复自身最大生命的0.2%";
    
    pSkill = CHpChangeBuff::create(3, false, 0.1, 0.5, true, -1);
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CAuraPas::create(150, CAuraPas::kOwn | CAuraPas::kAlly, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChangeAura1].iIndex = iKey;
    m_mapSkills[kHpChangeAura1].sName = "恢复光环";
    m_mapSkills[kHpChangeAura1].sDesc = "为周围半径150范围内的友方单位(不含自身)提供每秒5%的最大生命值恢复";
    
    pSkill = CSpeedBuff::create(1, false, CExtraCoeff(0.5, 0), CExtraCoeff(0.5, 0));
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CAuraPas::create(150, CAuraPas::kOwn | CAuraPas::kAlly, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSpeedAura1].iIndex = iKey;
    m_mapSkills[kSpeedAura1].sName = "振奋光环";
    m_mapSkills[kSpeedAura1].sDesc = "为周围半径150范围内的友方单位(不含自身)提供50%移动速度和50%攻击速度的提升";
    
    pSkill = CHpChangeBuff::create(20, true, 0.1, -0.02, true, -1);
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CAuraPas::create(150, CAuraPas::kEnemy, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kImmo1].iIndex = iKey;
    m_mapSkills[kImmo1].sName = "凋零光环";
    m_mapSkills[kImmo1].sDesc = "对周围半径150范围内的敌方单位造成最大生命0.2%的伤害，效果每0.5s叠加一次";
    
    pSkill = CThumpPas::create(20, CExtraCoeff(2, 0), 0);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kCritical1].iIndex = iKey;
    m_mapSkills[kCritical1].sName = "暴击";
    m_mapSkills[kCritical1].sDesc = "攻击时有20%的概率造成2倍的攻击效果";
    
    pSkill = CThumpPas::create(10, CExtraCoeff(5, 0), 0);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kCritical2].iIndex = iKey;
    m_mapSkills[kCritical2].sName = "暴击";
    m_mapSkills[kCritical2].sDesc = "攻击时有10%的概率造成5倍的攻击效果";
    
    pSkill = CVampirePas::create(0.2);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kVamprie1].iIndex = iKey;
    m_mapSkills[kVamprie1].sName = "吸血";
    m_mapSkills[kVamprie1].sDesc = "对目标造成伤害的20%用来恢复自身";
    
    pSkill = CVampirePas::create(0.5);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kVamprie2].iIndex = iKey;
    m_mapSkills[kVamprie2].sName = "吸血";
    m_mapSkills[kVamprie2].sDesc = "对目标造成伤害的50%用来恢复自身";
    
    pSkill = CDoubleAttackPas::create(30);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kDoubleAttack1].iIndex = iKey;
    m_mapSkills[kDoubleAttack1].sName = "连击";
    m_mapSkills[kDoubleAttack1].sDesc = "攻击后有30%的概率立即发动第二次攻击";
    
    pSkill = CSplashPas::create(5, CExtraCoeff(0.6, 0), 15, CExtraCoeff(0.30, 0), 50, CExtraCoeff(0.15, 0));
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSplash1].iIndex = iKey;
    m_mapSkills[kSplash1].sName = "溅射";
    m_mapSkills[kSplash1].sDesc = "攻击对目标周围50半径的敌方单位同样产生15%~60%的溅射效果";
    
    pSkill = CSpeedBuff::create(5, false, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CSplashAct::create(8, 20, CAttackValue(1, CAttackValue::kPhysical, 80.0), 50, CAttackValue(1, CAttackValue::kPhysical, 40.0), 150, CAttackValue(1, CAttackValue::kPhysical, 20.0), iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThunderClap1].iIndex = iKey;
    m_mapSkills[kThunderClap1].sName = "震撼大地";
    m_mapSkills[kThunderClap1].sDesc = "对地面发出强有力的一击，对周围150半径的敌方单位同样造成20~80的物理伤害，并且5秒内降低目标50%移动速度和50%攻击速度";
    
    pSkill = CHpChangePas::create(0.1, 0.05, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange3].iIndex = iKey;
    m_mapSkills[kHpChange3].sName = "英雄资质";
    m_mapSkills[kHpChange3].sDesc = "每秒恢复自身最大生命的0.5%";
    
    pSkill = CThumpPas::create(20, CExtraCoeff(1, 10), 2);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThump1].iIndex = iKey;
    m_mapSkills[kThump1].sName = "重击";
    m_mapSkills[kThump1].sDesc = "攻击时有20%的概率击晕目标，并额外造成10点伤害";
    
    pSkill = CStunBuff::create(2, false);
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CThrowBuff::create(5, false, 0, 150, 0.5, 100, CAttackValue(1, CAttackValue::kPhysical, 50.0), 50.0, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    
    // for test
	//pSkill = CTransmitBuff::create(5, false, 151, ccp(100, 100), 1, 7, 7, iKey, 1);
    //iKey = pSm->addSkill(pSkill);
    
    // for test
    //pSkill = CHarmShieldBuff::create(2, false, CExtraCoeff(0, 0), true);
    //iKey = pSm->addSkill(pSkill);
    
	pSkill = CAttackBuffMakerPas::create(20, iKey, 1, CExtraCoeff(1, 0));
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThrowHit1].iIndex = iKey;
    m_mapSkills[kThrowHit1].sName = "击飞";
    m_mapSkills[kThrowHit1].sDesc = "20%的概率将目标击飞至150范围内的一点，落地后对周围50半径范围内单位造成50点伤害并眩晕2秒，被坠落单位还将受到50点额外伤害";
    
    pSkill = CSpeedBuff::create(7, true, CExtraCoeff(1.0, 0), CExtraCoeff(1.0, 0));
    iKey = pSm->addSkill(pSkill);
    
    // for test
    //pSkill = CHarmShieldBuff::create(5, false, CExtraCoeff(0, 0), true);
    //iKey = pSm->addSkill(pSkill);

    //pSkill = CThunderBolt2Buff::create(5, false, 0, 5, 300, CAttackValue(1, CAttackValue::kMagical,200.0));
    //iKey = pSm->addSkill(pSkill);
    
    pSkill = CSelfBuffMakerAct::create(15, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSpeedUp1].iIndex = iKey;
    m_mapSkills[kSpeedUp1].sName = "加速";
    m_mapSkills[kSpeedUp1].sDesc = "提高自身100%的移动速度和100%的攻击速度";

    pSkill = CThunderBolt2Buff::create(5, false, 0, 1, 300, CAttackValue(1, CAttackValue::kMagical, 50.0));
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThunderBoltBuff1].iIndex = iKey;
    m_mapSkills[kThunderBoltBuff1].sName = "雷击";
    m_mapSkills[kThunderBoltBuff1].sDesc = "暂无";

    pSkill = CChainBuff::create(0.5, false, 0, 200, 5, CAttackValue(1, CAttackValue::kMagical, 50.0), pPm->getProjectileByIndex(COrgUnitInfo::kChain1));
    dynamic_cast<CChainBuff*>(pSkill)->setWeaponType(CGameUnit::kWTInstant);
    dynamic_cast<CChainBuff*>(pSkill)->setProjectileScale(0.75);
    //pSkill = CChainBuff::create(0.5, false, 0, 100, 10, CAttackValue(1, CAttackValue::kMagical, 50.0), pPm->getProjectileByIndex(COrgUnitInfo::kBall2));
    //dynamic_cast<CChainBuff*>(pSkill)->setWeaponType(CGameUnit::kWTDelayed);
    iKey = pSm->addSkill(pSkill);

    //pSkill = CSpeedBuff::create(1, true, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
    //iKey = pSm->addSkill(pSkill);
    
    pSkill = CProjectileAct::create(1.0, 300, CAttackValue(1, CAttackValue::kMagical, 50.0), pPm->getProjectileByIndex(COrgUnitInfo::kLightning3), iKey, 1);
    dynamic_cast<CProjectileAct*>(pSkill)->setWeaponType(CGameUnit::kWTInstant);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThunderAttack1].iIndex = iKey;
    m_mapSkills[kThunderAttack1].sName = "雷霆一击";
    m_mapSkills[kThunderAttack1].sDesc = "猛烈锤击地面，向目标射出一道电光，将造成50点魔法伤害";

    //pSkill = CSwordStormSkill::create(40, 3.0, 100, CAttackValue(1, CAttackValue::kPhysical, 5.0), CExtraCoeff(0.30, 0), "act4");
    //dynamic_cast<CSwordStormSkill*>(pSkill)->setDelayPerUnit(0.03);
    //iKey = pSm->addSkill(pSkill);
    pSkill = CSwordStormBuff::create(5, false, 0, 100, CAttackValue(1, CAttackValue::kPhysical, 5.0), CExtraCoeff(0.30, 0), "act4");
    dynamic_cast<CSwordStormBuff*>(pSkill)->setDelayPerUnit(0.03);
    iKey = pSm->addSkill(pSkill);

    pSkill = CSelfBuffMakerAct::create(15, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSwordStorm1].iIndex = iKey;
    m_mapSkills[kSwordStorm1].sName = "剑刃风暴";
    m_mapSkills[kSwordStorm1].sDesc = "如风暴般高速旋转手中剑刃，使周围单位受到伤害";

    pSkill = CJumpChopSkill::create(100, 200, 10, CAttackValue(1, CAttackValue::kPhysical, 50.0), "act5");
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kJumpChop1].iIndex = iKey;
    m_mapSkills[kJumpChop1].sName = "跳劈";
    m_mapSkills[kJumpChop1].sDesc = "跃起给敌人重重打击";

    
    pSkill = CHpChangeBuff::create(5, true, 0.1, 2, false, -1);
    iKey = pSm->addSkill(pSkill);

    pSkill = CSelfBuffMakerAct::create(15, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHealing1].iIndex = iKey;
    m_mapSkills[kHealing1].sName = "快速愈合";
    m_mapSkills[kHealing1].sDesc = "5秒内恢复100生命值";
    
    return true;
}

COrgSkillInfo* COrgSkillInfo::sharedOrgSkillInfo()
{
    static COrgSkillInfo* pInst = NULL;
    if (pInst)
    {
        return pInst;
    }
    pInst = COrgSkillInfo::create();
    CC_SAFE_RETAIN(pInst);
    return pInst;
}

CSkill* COrgSkillInfo::skill( int iSkillIndex )
{
    M_DEF_SM(pSm);
    return pSm->copySkill(m_mapSkills[iSkillIndex].iIndex);
}