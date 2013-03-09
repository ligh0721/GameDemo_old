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
    m_mapSkills[kSlowDown1].sName = "����";
    m_mapSkills[kSlowDown1].sDesc = "����Ŀ��50%���ƶ��ٶȺ�50%�Ĺ����ٶ�";
    
    pSkill = CHpChangePas::create(0.1, 0.2, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange1].iIndex = iKey;
    m_mapSkills[kHpChange1].sName = "�Զ�Ȭ��";
    m_mapSkills[kHpChange1].sDesc = "ÿ��ָ��������������2%";
    
    pSkill = CHpChangePas::create(0.1, 0.02, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange2].iIndex = iKey;
    m_mapSkills[kHpChange2].sName = "�Զ�Ȭ��";
    m_mapSkills[kHpChange2].sDesc = "ÿ��ָ��������������0.2%";
    
    pSkill = CHpChangeBuff::create(3, false, 0.1, 0.5, true, -1);
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CAuraPas::create(150, CAuraPas::kOwn | CAuraPas::kAlly, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChangeAura1].iIndex = iKey;
    m_mapSkills[kHpChangeAura1].sName = "�ָ��⻷";
    m_mapSkills[kHpChangeAura1].sDesc = "Ϊ��Χ�뾶150��Χ�ڵ��ѷ���λ(��������)�ṩÿ��5%���������ֵ�ָ�";
    
    pSkill = CSpeedBuff::create(1, false, CExtraCoeff(0.5, 0), CExtraCoeff(0.5, 0));
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CAuraPas::create(150, CAuraPas::kOwn | CAuraPas::kAlly, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSpeedAura1].iIndex = iKey;
    m_mapSkills[kSpeedAura1].sName = "��ܹ⻷";
    m_mapSkills[kSpeedAura1].sDesc = "Ϊ��Χ�뾶150��Χ�ڵ��ѷ���λ(��������)�ṩ50%�ƶ��ٶȺ�50%�����ٶȵ�����";
    
    pSkill = CHpChangeBuff::create(20, true, 0.1, -0.02, true, -1);
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CAuraPas::create(150, CAuraPas::kEnemy, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kImmo1].iIndex = iKey;
    m_mapSkills[kImmo1].sName = "����⻷";
    m_mapSkills[kImmo1].sDesc = "����Χ�뾶150��Χ�ڵĵз���λ����������0.2%���˺���Ч��ÿ0.5s����һ��";
    
    pSkill = CThumpPas::create(20, CExtraCoeff(2, 0), 0);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kCritical1].iIndex = iKey;
    m_mapSkills[kCritical1].sName = "����";
    m_mapSkills[kCritical1].sDesc = "����ʱ��20%�ĸ������2���Ĺ���Ч��";
    
    pSkill = CThumpPas::create(10, CExtraCoeff(5, 0), 0);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kCritical2].iIndex = iKey;
    m_mapSkills[kCritical2].sName = "����";
    m_mapSkills[kCritical2].sDesc = "����ʱ��10%�ĸ������5���Ĺ���Ч��";
    
    pSkill = CVampirePas::create(0.2);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kVamprie1].iIndex = iKey;
    m_mapSkills[kVamprie1].sName = "��Ѫ";
    m_mapSkills[kVamprie1].sDesc = "��Ŀ������˺���20%�����ָ�����";
    
    pSkill = CVampirePas::create(0.5);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kVamprie2].iIndex = iKey;
    m_mapSkills[kVamprie2].sName = "��Ѫ";
    m_mapSkills[kVamprie2].sDesc = "��Ŀ������˺���50%�����ָ�����";
    
    pSkill = CDoubleAttackPas::create(30);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kDoubleAttack1].iIndex = iKey;
    m_mapSkills[kDoubleAttack1].sName = "����";
    m_mapSkills[kDoubleAttack1].sDesc = "��������30%�ĸ������������ڶ��ι���";
    
    pSkill = CSplashPas::create(5, CExtraCoeff(0.6, 0), 15, CExtraCoeff(0.30, 0), 50, CExtraCoeff(0.15, 0));
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSplash1].iIndex = iKey;
    m_mapSkills[kSplash1].sName = "����";
    m_mapSkills[kSplash1].sDesc = "������Ŀ����Χ50�뾶�ĵз���λͬ������15%~60%�Ľ���Ч��";
    
    pSkill = CSpeedBuff::create(5, false, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CSplashAct::create(8, 20, CAttackValue(1, CAttackValue::kPhysical, 80.0), 50, CAttackValue(1, CAttackValue::kPhysical, 40.0), 150, CAttackValue(1, CAttackValue::kPhysical, 20.0), iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThunderClap1].iIndex = iKey;
    m_mapSkills[kThunderClap1].sName = "�𺳴��";
    m_mapSkills[kThunderClap1].sDesc = "�Ե��淢��ǿ������һ��������Χ150�뾶�ĵз���λͬ�����20~80�������˺�������5���ڽ���Ŀ��50%�ƶ��ٶȺ�50%�����ٶ�";
    
    pSkill = CHpChangePas::create(0.1, 0.05, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange3].iIndex = iKey;
    m_mapSkills[kHpChange3].sName = "Ӣ������";
    m_mapSkills[kHpChange3].sDesc = "ÿ��ָ��������������0.5%";
    
    pSkill = CThumpPas::create(20, CExtraCoeff(1, 10), 2);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThump1].iIndex = iKey;
    m_mapSkills[kThump1].sName = "�ػ�";
    m_mapSkills[kThump1].sDesc = "����ʱ��20%�ĸ��ʻ���Ŀ�꣬���������10���˺�";
    
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
    m_mapSkills[kThrowHit1].sName = "����";
    m_mapSkills[kThrowHit1].sDesc = "20%�ĸ��ʽ�Ŀ�������150��Χ�ڵ�һ�㣬��غ����Χ50�뾶��Χ�ڵ�λ���50���˺���ѣ��2�룬��׹�䵥λ�����ܵ�50������˺�";
    
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
    m_mapSkills[kSpeedUp1].sName = "����";
    m_mapSkills[kSpeedUp1].sDesc = "�������100%���ƶ��ٶȺ�100%�Ĺ����ٶ�";

    pSkill = CThunderBolt2Buff::create(5, false, 0, 1, 300, CAttackValue(1, CAttackValue::kMagical, 50.0));
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThunderBoltBuff1].iIndex = iKey;
    m_mapSkills[kThunderBoltBuff1].sName = "�׻�";
    m_mapSkills[kThunderBoltBuff1].sDesc = "����";

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
    m_mapSkills[kThunderAttack1].sName = "����һ��";
    m_mapSkills[kThunderAttack1].sDesc = "���Ҵ������棬��Ŀ�����һ����⣬�����50��ħ���˺�";

    //pSkill = CSwordStormSkill::create(40, 3.0, 100, CAttackValue(1, CAttackValue::kPhysical, 5.0), CExtraCoeff(0.30, 0), "act4");
    //dynamic_cast<CSwordStormSkill*>(pSkill)->setDelayPerUnit(0.03);
    //iKey = pSm->addSkill(pSkill);
    pSkill = CSwordStormBuff::create(5, false, 0, 100, CAttackValue(1, CAttackValue::kPhysical, 5.0), CExtraCoeff(0.30, 0), "act4");
    dynamic_cast<CSwordStormBuff*>(pSkill)->setDelayPerUnit(0.03);
    iKey = pSm->addSkill(pSkill);

    pSkill = CSelfBuffMakerAct::create(15, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSwordStorm1].iIndex = iKey;
    m_mapSkills[kSwordStorm1].sName = "���з籩";
    m_mapSkills[kSwordStorm1].sDesc = "��籩�������ת���н��У�ʹ��Χ��λ�ܵ��˺�";

    pSkill = CJumpChopSkill::create(100, 200, 10, CAttackValue(1, CAttackValue::kPhysical, 50.0), "act5");
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kJumpChop1].iIndex = iKey;
    m_mapSkills[kJumpChop1].sName = "����";
    m_mapSkills[kJumpChop1].sDesc = "Ծ����������ش��";

    
    pSkill = CHpChangeBuff::create(5, true, 0.1, 2, false, -1);
    iKey = pSm->addSkill(pSkill);

    pSkill = CSelfBuffMakerAct::create(15, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHealing1].iIndex = iKey;
    m_mapSkills[kHealing1].sName = "��������";
    m_mapSkills[kHealing1].sDesc = "5���ڻָ�100����ֵ";
    
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