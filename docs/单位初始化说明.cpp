// demo code
M_DEF_GM(pGm); // 定义游戏管理器
// 加载图集
pFc->addSpriteFramesWithFile("unit.plist");
pFc->addSpriteFramesWithFile("UI.plist");
pFc->addSpriteFramesWithFile("tank.plist");

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

pGm->loadUnitAnimation("Ball1", "move");
pGm->loadUnitAnimation("Ball1", "die");
pGm->loadUnitAnimation("Ball1", "act1");

pGm->loadUnitAnimation("Ball2", "move");
pGm->loadUnitAnimation("Ball2", "die");

pGm->loadUnitAnimation("Lightning1", "die");

pGm->preloadEffectSound("Lightning1.wav");
pGm->preloadEffectSound("Ball1.wav");
pGm->preloadEffectSound("MalikAttack.wav");


M_DEF_PM(pPm); // 定义投射物管理器

// 创建投射物模板
g_pProj = CProjectile::createWithName("Lightning1");
g_pProj->setProjectileType(CProjectile::kLightning); // 设置投射物为闪电类

//g_pProj->prepareMoveAnimation("move", 0.1); // 加载投射物移动动画
g_pProj->prepareDieAnimation("die", 0.05); // 加载投射物死亡动画
pPm->addProjectile(g_pProj); // 将投射物添加到PM中

g_pProj2 = CProjectile::createWithName("Ball1");
g_pProj2->prepareMoveAnimation("move", 0.1);
g_pProj2->prepareDieAnimation("die", 0.1);
pPm->addProjectile(g_pProj2);

// 创建单位
CGameUnit* u = CGameUnit::createWithName("Malik", ccp(0.5, 0.1125)); // 单位资源名, 单位锚点
addUnit(u); // 加到UnitLayer中

// 为单位准备动画索引
u->prepareMoveAnimation("move", 0.05);
u->prepareDieAnimation("die", 0.1);
u->prepareAttackAnimation(2, // 准备攻击索引，多个动画随机组合 (索引, 资源名, 帧间隔)
    CGameUnit::kAnimationAct1, "act1", 0.08,
    CGameUnit::kAnimationAct2, "act2", 0.08
    );

u->setBaseMoveSpeed(80); // 基础移动速度
u->setBaseAttackInterval(1.5); // 基础攻击间隔，单位是秒
u->setAttackEffectDelay(0.4); // 攻击前摇延迟
//u->setExAttackSpeed(CExtraCoeff(2, 0)); // 攻击多项式系数 ax+b
u->setAttackMinRange(4); // 攻击最短距离，小于该距离不可立即攻击，参考投石车
u->setAttackRange(15); // 攻击距离，大于该距离不可立即攻击
u->setWeaponType(CGameUnit::kWTClose); // 武器类型为近战
CAttackValue oAv; // 定义攻击值对象
oAv.setAttack(eAttackTypeStrike, 10); // 冲击伤害10
u->setBaseAttackValue(oAv); // 基础攻击值
u->setArmorType(eArmorTypeBiology); // 护甲类型
u->setArmorValue(5); // 护甲值
u->setForceByIndex(1); // 所在势力1
u->setAlly((1 << 1) | (1 << 2)); // 势力1跟势力2结盟
u->setHostilityRange(200); // 仇恨引发范围

u->setPosition(ccp(450 + rand() % 100, 250 + rand() % 100)); // 设置位置
u->setHalfOfWidth(11); // 单位宽度的一半，可用用于触碰体积计算，默认是纹理宽的一半
u->setHalfOfHeight(13); // 单位从锚点向上的高度，不是一半！可用于投射物击中特效位置偏移计算，默认是纹理高的一半
u->getSprite()->setScale(1.0); // 单位缩放
g_iDemoTarget = u->getKey(); // 单位key
u->setMaxHp(200); // 最大HP，最大HP的变动，HP也会变动，百分比一致

// 为单位添加技能
M_DEF_SM(pSm); // 定义技能管理器

// 定义BUFF模板，某些技能作用时会产生BUFF，需要根据BUFF模板生产BUFF
CSkill* pSkill = CSlowDownBuff::create(1, true, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
g_iSlowDownKey = pSkill->getKey();
pSm->addSkill(pSkill); // 将模板技能添加到技能管理器中

u->addSkill(CStatusShowPas::create()); // 血条被动
u->addSkill(CHpChangePas::create(0.1, 0.2, true, -1)); // 每0.1秒恢复0.2%HP
u->addSkill(CVampirePas::create(0.3)); // 攻击30%吸血
u->addSkill(CThumpPas::create(20, CExtraCoeff(2, 0), 0)); // 20%造成2倍致命一击
u->addSkill(CThumpPas::create(20, CExtraCoeff(5, 0), 0)); // 20%造成5倍致命一击
u->addSkill(CAttackBuffMakerPas::create(100, pSkill->getKey(), 1)); // 100%降低10%目标攻击/移动速度，可叠加
u->addSkill(CDoubleAttackPas::create(80)); // 50%发动连续攻击

u = CGameUnit::createWithName("Magnus", ccp(0.5, 7.0 / 60));
addUnit(u);
u->prepareMoveAnimation("move", 0.05);
u->prepareDieAnimation("die", 0.1);
u->prepareAttackAnimation(4,
    CGameUnit::kAnimationAct1, "act1", 0.05,
    CGameUnit::kAnimationAct2, "act2", 0.05,
    CGameUnit::kAnimationAct3, "act3", 0.05,
    CGameUnit::kAnimationAct5, "act5", 0.05
    );

u->setBaseMoveSpeed(50);
u->setBaseAttackInterval(3);
u->setAttackEffectDelay(0.3);
u->setAttackMinRange(100);
u->setAttackRange(150);
u->setWeaponType(CGameUnit::kWTInstant); // 武器类型为立即，区别于近战、延迟
oAv.setZero();
oAv.setAttack(eAttackTypeFlame, 5);
u->setBaseAttackValue(oAv);
u->setArmorType(eArmorTypeBiology);
u->setArmorValue(5);
u->setPosition(ccp(450 + rand() % 100, 250 + rand() % 100));
u->setHalfOfWidth(7);
u->setHalfOfHeight(10);
u->setMaxHp(100);
u->setTemplateProjectile(g_pProj); // 单位攻击投射物
//u->setProjectileMoveSpeed(200); // 投射物移动速度
u->setProjectileScale(u->getSprite()->getScale()); // 投射物缩放
u->setProjectileMaxOffsetY(20); // 投射物抛物线运动最高点Y偏移
g_iDemoTarget2 = u->getKey();
u->setForceByIndex(2);
u->setAlly((1 << 1) | (1 << 2));
u->setHostilityRange(200);

u->addSkill(CStatusShowPas::create()); // 血条被动
u->addSkill(CHpChangePas::create(0.1, 0.2, true, -1)); // 每0.1秒恢复0.2%HP
u->addSkill(CVampirePas::create(0.2)); // 攻击20%吸血
u->addSkill(CThumpPas::create(20, CExtraCoeff(20, 0), 0)); // 20%造成20倍致命一击
u->addSkill(CAttackBuffMakerPas::create(100, pSkill->getKey(), 1)); // 100%降低10%目标攻击/移动速度，可叠加
u->addSkill(CDoubleAttackPas::create(50)); // 50%发动连续攻击
