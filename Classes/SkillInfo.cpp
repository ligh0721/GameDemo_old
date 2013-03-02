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
    m_mapSkills[kSlowDown1].sName = "ºıÀŸ";
    m_mapSkills[kSlowDown1].sDesc = "ΩµµÕƒø±Í50%µƒ“∆∂ØÀŸ∂»∫Õ50%µƒπ•ª˜ÀŸ∂»";

    pSkill = CHpChangePas::create(0.1, 0.2, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange1].iIndex = iKey;
    m_mapSkills[kHpChange1].sName = "◊‘∂Ø»¨”˙";
    m_mapSkills[kHpChange1].sDesc = "√ø√Îª÷∏¥◊‘…Ì◊Ó¥Û…˙√¸µƒ2%";

    pSkill = CHpChangePas::create(0.1, 0.02, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange2].iIndex = iKey;
    m_mapSkills[kHpChange2].sName = "◊‘∂Ø»¨”˙";
    m_mapSkills[kHpChange2].sDesc = "√ø√Îª÷∏¥◊‘…Ì◊Ó¥Û…˙√¸µƒ0.2%";

    pSkill = CHpChangeBuff::create(3, false, 0.1, 0.5, true, -1);
    iKey = pSm->addSkill(pSkill);

    pSkill = CAuraPas::create(150, CAuraPas::kOwn | CAuraPas::kAlly, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChangeAura1].iIndex = iKey;
    m_mapSkills[kHpChangeAura1].sName = "ª÷∏¥π‚ª∑";
    m_mapSkills[kHpChangeAura1].sDesc = "Œ™÷‹Œß∞Îæ∂150∑∂Œßƒ⁄µƒ”—∑Ωµ•Œª(≤ª∫¨◊‘…Ì)Ã·π©√ø√Î5%µƒ◊Ó¥Û…˙√¸÷µª÷∏¥";

    pSkill = CSpeedBuff::create(1, false, CExtraCoeff(0.5, 0), CExtraCoeff(0.5, 0));
    iKey = pSm->addSkill(pSkill);

    pSkill = CAuraPas::create(150, CAuraPas::kOwn | CAuraPas::kAlly, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSpeedAura1].iIndex = iKey;
    m_mapSkills[kSpeedAura1].sName = "’Ò∑‹π‚ª∑";
    m_mapSkills[kSpeedAura1].sDesc = "Œ™÷‹Œß∞Îæ∂150∑∂Œßƒ⁄µƒ”—∑Ωµ•Œª(≤ª∫¨◊‘…Ì)Ã·π©50%“∆∂ØÀŸ∂»∫Õ50%π•ª˜ÀŸ∂»µƒÃ·…˝";

    pSkill = CHpChangeBuff::create(20, true, 0.1, -0.02, true, -1);
    iKey = pSm->addSkill(pSkill);

    pSkill = CAuraPas::create(150, CAuraPas::kEnemy, 0.5, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kImmo1].iIndex = iKey;
    m_mapSkills[kImmo1].sName = "µÚ¡„π‚ª∑";
    m_mapSkills[kImmo1].sDesc = "∂‘÷‹Œß∞Îæ∂150∑∂Œßƒ⁄µƒµ–∑Ωµ•Œª‘Ï≥…◊Ó¥Û…˙√¸0.2%µƒ…À∫¶£¨–ßπ˚√ø0.5sµ˛º”“ª¥Œ";

    pSkill = CThumpPas::create(20, CExtraCoeff(2, 0), 0);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kCritical1].iIndex = iKey;
    m_mapSkills[kCritical1].sName = "±©ª˜";
    m_mapSkills[kCritical1].sDesc = "π•ª˜ ±”–20%µƒ∏≈¬ ‘Ï≥…2±∂µƒπ•ª˜–ßπ˚";

    pSkill = CThumpPas::create(10, CExtraCoeff(5, 0), 0);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kCritical2].iIndex = iKey;
    m_mapSkills[kCritical2].sName = "±©ª˜";
    m_mapSkills[kCritical2].sDesc = "π•ª˜ ±”–10%µƒ∏≈¬ ‘Ï≥…5±∂µƒπ•ª˜–ßπ˚";

    pSkill = CVampirePas::create(0.2);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kVamprie1].iIndex = iKey;
    m_mapSkills[kVamprie1].sName = "Œ¸—™";
    m_mapSkills[kVamprie1].sDesc = "∂‘ƒø±Í‘Ï≥……À∫¶µƒ20%”√¿¥ª÷∏¥◊‘…Ì";

    pSkill = CVampirePas::create(0.5);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kVamprie2].iIndex = iKey;
    m_mapSkills[kVamprie2].sName = "Œ¸—™";
    m_mapSkills[kVamprie2].sDesc = "∂‘ƒø±Í‘Ï≥……À∫¶µƒ50%”√¿¥ª÷∏¥◊‘…Ì";

    pSkill = CDoubleAttackPas::create(80);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kDoubleAttack1].iIndex = iKey;
    m_mapSkills[kDoubleAttack1].sName = "¡¨ª˜";
    m_mapSkills[kDoubleAttack1].sDesc = "π•ª˜∫Û”–30%µƒ∏≈¬ ¡¢º¥∑¢∂Øµ⁄∂˛¥Œπ•ª˜";

    pSkill = CSplashPas::create(10, CExtraCoeff(0.6, 0), 30, CExtraCoeff(0.30, 0), 100, CExtraCoeff(0.15, 0));
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSplash1].iIndex = iKey;
    m_mapSkills[kSplash1].sName = "Ω¶…‰";
    m_mapSkills[kSplash1].sDesc = "π•ª˜∂‘ƒø±Í÷‹Œß100∞Îæ∂µƒµ–∑Ωµ•ŒªÕ¨—˘≤˙…˙15%~60%µƒΩ¶…‰–ßπ˚";

    pSkill = CSpeedBuff::create(5, false, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CSplashAct::create(8, 20, CAttackValue(1, CAttackValue::kPhysical, 80.0), 50, CAttackValue(1, CAttackValue::kPhysical, 40.0), 150, CAttackValue(1, CAttackValue::kPhysical, 20.0), iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThunderClap1].iIndex = iKey;
    m_mapSkills[kThunderClap1].sName = "’∫≥¥Ûµÿ";
    m_mapSkills[kThunderClap1].sDesc = "∂‘µÿ√Ê∑¢≥ˆ«ø”–¡¶µƒ“ªª˜£¨∂‘÷‹Œß150∞Îæ∂µƒµ–∑Ωµ•ŒªÕ¨—˘‘Ï≥…20~80µƒŒÔ¿Ì…À∫¶£¨≤¢«“5√Îƒ⁄ΩµµÕƒø±Í50%“∆∂ØÀŸ∂»∫Õ50%π•ª˜ÀŸ∂»";

    pSkill = CHpChangePas::create(0.1, 0.05, true, -1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kHpChange3].iIndex = iKey;
    m_mapSkills[kHpChange3].sName = "”¢–€◊ ÷ ";
    m_mapSkills[kHpChange3].sDesc = "√ø√Îª÷∏¥◊‘…Ì◊Ó¥Û…˙√¸µƒ0.5%";

    pSkill = CThumpPas::create(20, CExtraCoeff(1, 10), 2);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThump1].iIndex = iKey;
    m_mapSkills[kThump1].sName = "÷ÿª˜";
    m_mapSkills[kThump1].sDesc = "π•ª˜ ±”–20%µƒ∏≈¬ ª˜‘Œƒø±Í£¨≤¢∂ÓÕ‚‘Ï≥…10µ„…À∫¶";

    pSkill = CStunBuff::create(2, false);
    iKey = pSm->addSkill(pSkill);

    pSkill = CThrowBuff::create(5, false, 0, 150, 0.5, 100, CAttackValue(1, CAttackValue::kPhysical, 50.0), 50.0, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    
    
    // for test
    //pSkill = CHarmShieldBuff::create(2, false, CExtraCoeff(0, 0), true);
    //iKey = pSm->addSkill(pSkill);

	pSkill = CAttackBuffMakerPas::create(75, iKey, 1, CExtraCoeff(1, 0));
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThrowHit1].iIndex = iKey;
    m_mapSkills[kThrowHit1].sName = "ª˜∑…";
    m_mapSkills[kThrowHit1].sDesc = "75%µƒ∏≈¬ Ω´ƒø±Íª˜∑…÷¡150∑∂Œßƒ⁄µƒ“ªµ„£¨¬‰µÿ∫Û∂‘÷‹Œß50∞Îæ∂∑∂Œßƒ⁄µ•Œª‘Ï≥…50µ„…À∫¶≤¢—£‘Œ2√Î£¨±ª◊π¬‰µ•ŒªªπΩ´ ‹µΩ50µ„∂ÓÕ‚…À∫¶";

	
	vector<CCPoint> vecPoints;
	pSkill = CTransmitBuff::create(5, false, 151, vecPoints, 1.5, 1.5, iKey, 1);
	iKey = pSm->addSkill(pSkill);
	pSkill = CAttackBuffMakerPas::create(100, iKey, 1, CExtraCoeff(1, 0));
	iKey = pSm->addSkill(pSkill);

	m_mapSkills[kTransmit1].iIndex = iKey;
	m_mapSkills[kTransmit1].sName = "¥´ÀÕ";
	m_mapSkills[kTransmit1].sDesc = "100%∏≈¬ ¥´ÀÕµ–±¯¡Ì“ª∏ˆµ–∑Ω";

    pSkill = CSpeedBuff::create(7, true, CExtraCoeff(1.0, 0), CExtraCoeff(1.0, 0));
    iKey = pSm->addSkill(pSkill);

    // for test
    pSkill = CHarmShieldBuff::create(5, false, CExtraCoeff(0, 0), true);
    iKey = pSm->addSkill(pSkill);

    pSkill = CSelfBuffMakerAct::create(15, iKey, 1);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSpeedUp1].iIndex = iKey;
    m_mapSkills[kSpeedUp1].sName = "º”ÀŸ";
    m_mapSkills[kSpeedUp1].sDesc = "Ã·∏ﬂ◊‘…Ì100%µƒ“∆∂ØÀŸ∂»∫Õ100%µƒπ•ª˜ÀŸ∂»";

    pSkill = CChainBuff::create(0.5, false, 0, 300, 10, CAttackValue(1, CAttackValue::kMagical, 50.0), pPm->getProjectileByIndex(COrgUnitInfo::kLightning3));
    dynamic_cast<CChainBuff*>(pSkill)->setWeaponType(CGameUnit::kWTInstant);
    //pSkill = CChainBuff::create(0.5, false, 0, 100, 10, CAttackValue(1, CAttackValue::kMagical, 50.0), pPm->getProjectileByIndex(COrgUnitInfo::kBall2));
    //dynamic_cast<CChainBuff*>(pSkill)->setWeaponType(CGameUnit::kWTDelayed);
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CProjectileAct::create(1.0, 1500, CAttackValue(1, CAttackValue::kMagical, 50.0), pPm->getProjectileByIndex(COrgUnitInfo::kLightning3), iKey, 1);
    dynamic_cast<CProjectileAct*>(pSkill)->setWeaponType(CGameUnit::kWTInstant);
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kThunderAttack1].iIndex = iKey;
    m_mapSkills[kThunderAttack1].sName = "¿◊ˆ™“ªª˜";
    m_mapSkills[kThunderAttack1].sDesc = "√Õ¡“¥∏ª˜µÿ√Ê£¨œÚƒø±Í…‰≥ˆ“ªµ¿µÁπ‚£¨Ω´‘Ï≥…50µ„ƒß∑®…À∫¶";
    

    vector<int> vecUnitKey;
    pSkill = CChainLightingBuff::create(5, false, 152, 600, 400, 8, CAttackValue(1, CAttackValue::kMagical, 80.0), vecUnitKey);
    iKey = pSm->addSkill(pSkill);
    
    pSkill = CAttackBuffMakerPas::create(100, iKey, 1, CExtraCoeff(1, 0));
    iKey = pSm->addSkill(pSkill);

    m_mapSkills[kChainLighting1].iIndex = iKey;
    m_mapSkills[kChainLighting1].sName = "闪电链";
    m_mapSkills[kChainLighting1].sDesc = "使范围内的兵受到闪电";
    
    pSkill = CSwordStormSkill::create(100, 3.0, 100, CAttackValue(1, CAttackValue::kMagical, 50.0), CExtraCoeff(0.30, 0), "act4");
    iKey = pSm->addSkill(pSkill);
    m_mapSkills[kSwordStorm1].iIndex = iKey;
    m_mapSkills[kSwordStorm1].sName = "剑刃风暴";
    m_mapSkills[kSwordStorm1].sDesc = "旋转如风暴使一定范围内的敌人持续掉血";
    
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
