#include "CommInc.h"

#include "GameDisplay.h"
#include "Unit.h"
#include "Skill.h"
#include "GameCtrl.h"
#include "UnitInfo.h"


COrgUnitInfo g_oOrgUnitInfo;

COrgUnitInfo::COrgUnitInfo()
{
}

bool COrgUnitInfo::init()
{
    M_DEF_UM(pUm);
    M_DEF_PM(pPm);
    M_DEF_FC(pFc);
    M_DEF_GM(pGm);
    CProjectile* pProj;
    enum
    {
        kAct1 = CGameUnit::kAnimationAct1,
        kAct2 = CGameUnit::kAnimationAct2,
        kAct3 = CGameUnit::kAnimationAct3,
        kAct4 = CGameUnit::kAnimationAct4,
        kAct5 = CGameUnit::kAnimationAct5,
        kAct6 = CGameUnit::kAnimationAct6,
    };

    // 下面加载资源部分，将改造为根据关卡，部分加载
    // 加载图集
    pFc->addSpriteFramesWithFile("unit.plist");

    // 加载单位动画
    pGm->loadUnitAnimation("Malik", "move");
    pGm->loadUnitAnimation("Malik", "die");
    pGm->loadUnitAnimation("Malik", "act1");
    pGm->loadUnitAnimation("Malik", "act2");
    pGm->loadUnitAnimation("Malik", "act3");
    pGm->loadUnitAnimation("Malik", "act4");
    pGm->loadUnitAnimation("Malik", "act5");

    pGm->loadUnitAnimation("Paladin", "move");
    pGm->loadUnitAnimation("Paladin", "die");
    pGm->loadUnitAnimation("Paladin", "act1");
    pGm->loadUnitAnimation("Paladin", "act2");
    pGm->loadUnitAnimation("Paladin", "act3");
    pGm->loadUnitAnimation("Paladin", "act4");

    pGm->loadUnitAnimation("Magnus", "move");
    pGm->loadUnitAnimation("Magnus", "die");
    pGm->loadUnitAnimation("Magnus", "act1");
    pGm->loadUnitAnimation("Magnus", "act2");
    pGm->loadUnitAnimation("Magnus", "act3");
    pGm->loadUnitAnimation("Magnus", "act4");
    pGm->loadUnitAnimation("Magnus", "act5");
    pGm->loadUnitAnimation("Magnus", "act6");

    pGm->loadUnitAnimation("Jt", "move");
    pGm->loadUnitAnimation("Jt", "die");
    pGm->loadUnitAnimation("Jt", "act1");

    pGm->loadUnitAnimation("Arcane", "die");
    pGm->loadUnitAnimation("Arcane", "act1");

    pGm->loadUnitAnimation("Tesla", "die");
    pGm->loadUnitAnimation("Tesla", "act1");

    pGm->loadUnitAnimation("Veznan", "move");
    pGm->loadUnitAnimation("Veznan", "die");
    pGm->loadUnitAnimation("Veznan", "act1");
    pGm->loadUnitAnimation("Veznan", "act2");
    pGm->loadUnitAnimation("Veznan", "act3");

    pGm->loadUnitAnimation("Ball1", "move");
    pGm->loadUnitAnimation("Ball1", "die");
    pGm->loadUnitAnimation("Ball1", "act1");

    pGm->loadUnitAnimation("Ball2", "move");
    pGm->loadUnitAnimation("Ball2", "die");

    pGm->loadUnitAnimation("Ball3", "move");
    pGm->loadUnitAnimation("Ball3", "die");

    pGm->loadUnitAnimation("Lightning1", "die");

    //pGm->preloadEffectSound("Lightning1.wav");
    //pGm->preloadEffectSound("Ball1.wav");
    //pGm->preloadEffectSound("MalikAttack.wav");

    pProj = CProjectile::createWithName("Ball1");
    pProj->setProjectileType(CProjectile::kFollow); // 设置投射物
    pProj->prepareMoveAnimation("move", 0.1); // 加载投射物移动动画
    pProj->prepareDieAnimation("die", 0.1); // 加载投射物死亡动画
    pProj->setBaseMoveSpeed(300.0);
    pPm->addProjectile(pProj); // 将投射物添加到PM中
    CProjectile* pProjBall1 = pProj;

    pProj = CProjectile::createWithName("Ball2");
    pProj->setProjectileType(CProjectile::kFollow); // 设置投射物
    pProj->prepareMoveAnimation("move", 0.1); // 加载投射物移动动画
    pProj->prepareDieAnimation("die", 0.1); // 加载投射物死亡动画
    pProj->setBaseMoveSpeed(300.0);
    pPm->addProjectile(pProj); // 将投射物添加到PM中
    CProjectile* pProjBall2 = pProj;

    pProj = CProjectile::createWithName("Ball3");
    pProj->setProjectileType(CProjectile::kFollow); // 设置投射物
    pProj->prepareMoveAnimation("move", 0.1); // 加载投射物移动动画
    pProj->prepareDieAnimation("die", 0.1); // 加载投射物死亡动画
    pProj->setBaseMoveSpeed(300.0);
    pPm->addProjectile(pProj); // 将投射物添加到PM中
    CProjectile* pProjBall3 = pProj;

    pProj = CProjectile::createWithName("Lightning1");
    pProj->setProjectileType(CProjectile::kLightning); // 设置投射物
    pProj->prepareDieAnimation("die", 0.1); // 加载投射物死亡动画
    pProj->setBaseMoveSpeed(0.0);
    pPm->addProjectile(pProj); // 将投射物添加到PM中
    CProjectile* pProjLightning1 = pProj;

    pProj = CProjectile::createWithName("Lightning1");
    pProj->setProjectileType(CProjectile::kLightning); // 设置投射物
    pProj->prepareDieAnimation("die", 0.05); // 加载投射物死亡动画
    pProj->setBaseMoveSpeed(0.0);
    pPm->addProjectile(pProj); // 将投射物添加到PM中
    CProjectile* pProjLightning2 = pProj;


    CUnitInfo oMalik(
        "Malik",
        ccp(0.5, 0.1125),
        7, 10,
        1,
        0.05, 0.1, 0.08, 0.08, 0.08, 0.08, 0.08, 0.0,
        CUnitInfo::ARR_ATTACK_ANI(2, kAct1, kAct2),
        40,
        2, 0.4, 0, 15, 150, CGameUnit::kWTClosely,
        0, 0, 0, 0, 0, 0,
        CAttackValue(1, CAttackValue::kPhysical, 10.0),
        0.150,
        CArmorValue::kNormal, 1,
        0, 0,
        100,
        false,
        10,
        10
        );
    pUm->addUnitInfo(COrgUnitInfo::kMalik, oMalik);

    CUnitInfo oMagnus(
        "Magnus",
        ccp(0.5, 7.0 / 60),
        7, 10,
        1,
        0.05, 0.1, 0.05, 0.05, 0.05, 0.05, 0.05, 0.0,
        CUnitInfo::ARR_ATTACK_ANI(4, kAct1, kAct2, kAct3, kAct5),
        40,
        2, 0.3, 0, 100, 150, CGameUnit::kWTDelayed,
        pProjBall1->getKey(), 200, 1, 20, 10, 10,
        CAttackValue(1, CAttackValue::kMagical, 10.0),
        0.150,
        CArmorValue::kNormal, 1,
        0, 0,
        70,
        false,
        10,
        10
        );
    pUm->addUnitInfo(COrgUnitInfo::kMagnus, oMagnus);

    CUnitInfo oPaladin(
        "Paladin",
        ccp(0.5, 0.25),
        9, 10,
        1,
        0.05, 0.1, 0.1, 0.1, 0.05, 0.0, 0.0, 0.0,
        CUnitInfo::ARR_ATTACK_ANI(3, kAct1, kAct2, kAct3),
        40,
        1.5, 0.2, 0, 13, 150, CGameUnit::kWTClosely,
        0, 0, 0, 0, 0, 0,
        CAttackValue(1, CAttackValue::kPhysical, 8.0),
        0.150,
        CArmorValue::kHeavy, 1,
        0, 0,
        100,
        false,
        15,
        15
        );
    pUm->addUnitInfo(COrgUnitInfo::kPaladin, oPaladin);

    CUnitInfo oJt(
        "Jt",
        ccp(0.5, 11.0 / 133),
        46, 50,
        1,
        0.1, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0,
        CUnitInfo::ARR_ATTACK_ANI(1, kAct1),
        40,
        6, 0.8, 1, 20, 150, CGameUnit::kWTClosely,
        0, 0, 0, 0, 0, 0,
        CAttackValue(1, CAttackValue::kSiege, 30.0),
        0.150,
        CArmorValue::kNormal, 1,
        0, 0,
        300,
        false,
        20,
        20
        );
    pUm->addUnitInfo(COrgUnitInfo::kJt, oJt);

    CUnitInfo oArcane(
        "Arcane",
        ccp(0.5, 0.125),
        28, 27,
        1,
        0.0, 0.1, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0,
        CUnitInfo::ARR_ATTACK_ANI(1, kAct1),
        0,
        2, 1.1, 0, 200, 200, CGameUnit::kWTDelayed,
        pProjBall3->getKey(), 300, 1.5, -20, 0, 60,
        CAttackValue(1, CAttackValue::kMagical, 20.0),
        0.150,
        CArmorValue::kWall, 1,
        0, 0,
        500,
        true,
        50,
        50
        );
    pUm->addUnitInfo(COrgUnitInfo::kArcane, oArcane);

    CUnitInfo oTesla(
        "Tesla",
        ccp(0.5, 13.0 / 73),
        35, 28,
        1,
        0.0, 0.1, 0.08, 0.0, 0.0, 0.0, 0.0, 0.0,
        CUnitInfo::ARR_ATTACK_ANI(1, kAct1),
        0,
        2, 1.6, 0, 200, 200, CGameUnit::kWTInstant,
        pProjLightning1->getKey(), 0, 1, 0, 11, 53,
        CAttackValue(1, CAttackValue::kPhysical, 20.0),
        0.150,
        CArmorValue::kWall, 1,
        0, 0,
        500,
        true,
        50,
        50
        );
    pUm->addUnitInfo(COrgUnitInfo::kTesla, oTesla);

    CUnitInfo oVeznan(
        "Veznan",
        ccp(0.5, 3.0 / 43),
        11, 15,
        1,
        0.05, 0.1, 0.05, 0.0, 0.0, 0.0, 0.0, 0.0,
        CUnitInfo::ARR_ATTACK_ANI(1, kAct1),
        40,
        2, 0.3, 0, 100, 150, CGameUnit::kWTDelayed,
        pProjBall2->getKey(), 300, 1, 5, 13, 17,
        CAttackValue(1, CAttackValue::kMagical, 10.0),
        0.150,
        CArmorValue::kCrystal, 1,
        0, 0,
        80,
        false,
        15,
        15
        );
    pUm->addUnitInfo(COrgUnitInfo::kVeznan, oVeznan);

    return true;
}

COrgUnitInfo* COrgUnitInfo::sharedOrgUnitInfo()
{
    static COrgUnitInfo* pInst = NULL;
    if (pInst)
    {
        return pInst;
    }
    pInst = COrgUnitInfo::create();
    CC_SAFE_RETAIN(pInst);
    return pInst;
}
