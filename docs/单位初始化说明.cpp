// demo code
M_DEF_GM(pGm); // ������Ϸ������
// ����ͼ��
pFc->addSpriteFramesWithFile("unit.plist");
pFc->addSpriteFramesWithFile("UI.plist");
pFc->addSpriteFramesWithFile("tank.plist");

// ���ص�λ����
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


M_DEF_PM(pPm); // ����Ͷ���������

// ����Ͷ����ģ��
g_pProj = CProjectile::createWithName("Lightning1");
g_pProj->setProjectileType(CProjectile::kLightning); // ����Ͷ����Ϊ������

//g_pProj->prepareMoveAnimation("move", 0.1); // ����Ͷ�����ƶ�����
g_pProj->prepareDieAnimation("die", 0.05); // ����Ͷ������������
pPm->addProjectile(g_pProj); // ��Ͷ������ӵ�PM��

g_pProj2 = CProjectile::createWithName("Ball1");
g_pProj2->prepareMoveAnimation("move", 0.1);
g_pProj2->prepareDieAnimation("die", 0.1);
pPm->addProjectile(g_pProj2);

// ������λ
CGameUnit* u = CGameUnit::createWithName("Malik", ccp(0.5, 0.1125)); // ��λ��Դ��, ��λê��
addUnit(u); // �ӵ�UnitLayer��

// Ϊ��λ׼����������
u->prepareMoveAnimation("move", 0.05);
u->prepareDieAnimation("die", 0.1);
u->prepareAttackAnimation(2, // ׼������������������������� (����, ��Դ��, ֡���)
    CGameUnit::kAnimationAct1, "act1", 0.08,
    CGameUnit::kAnimationAct2, "act2", 0.08
    );

u->setBaseMoveSpeed(80); // �����ƶ��ٶ�
u->setBaseAttackInterval(1.5); // ���������������λ����
u->setAttackEffectDelay(0.4); // ����ǰҡ�ӳ�
//u->setExAttackSpeed(CExtraCoeff(2, 0)); // ��������ʽϵ�� ax+b
u->setAttackMinRange(4); // ������̾��룬С�ڸþ��벻�������������ο�Ͷʯ��
u->setAttackRange(15); // �������룬���ڸþ��벻����������
u->setWeaponType(CGameUnit::kWTClose); // ��������Ϊ��ս
CAttackValue oAv; // ���幥��ֵ����
oAv.setAttack(eAttackTypeStrike, 10); // ����˺�10
u->setBaseAttackValue(oAv); // ��������ֵ
u->setArmorType(eArmorTypeBiology); // ��������
u->setArmorValue(5); // ����ֵ
u->setForceByIndex(1); // ��������1
u->setAlly((1 << 1) | (1 << 2)); // ����1������2����
u->setHostilityRange(200); // ���������Χ

u->setPosition(ccp(450 + rand() % 100, 250 + rand() % 100)); // ����λ��
u->setHalfOfWidth(11); // ��λ��ȵ�һ�룬�������ڴ���������㣬Ĭ����������һ��
u->setHalfOfHeight(13); // ��λ��ê�����ϵĸ߶ȣ�����һ�룡������Ͷ���������Чλ��ƫ�Ƽ��㣬Ĭ��������ߵ�һ��
u->getSprite()->setScale(1.0); // ��λ����
g_iDemoTarget = u->getKey(); // ��λkey
u->setMaxHp(200); // ���HP�����HP�ı䶯��HPҲ��䶯���ٷֱ�һ��

// Ϊ��λ��Ӽ���
M_DEF_SM(pSm); // ���弼�ܹ�����

// ����BUFFģ�壬ĳЩ��������ʱ�����BUFF����Ҫ����BUFFģ������BUFF
CSkill* pSkill = CSlowDownBuff::create(1, true, CExtraCoeff(-0.5, 0), CExtraCoeff(-0.5, 0));
g_iSlowDownKey = pSkill->getKey();
pSm->addSkill(pSkill); // ��ģ�弼����ӵ����ܹ�������

u->addSkill(CStatusShowPas::create()); // Ѫ������
u->addSkill(CHpChangePas::create(0.1, 0.2, true, -1)); // ÿ0.1��ָ�0.2%HP
u->addSkill(CVampirePas::create(0.3)); // ����30%��Ѫ
u->addSkill(CThumpPas::create(20, CExtraCoeff(2, 0), 0)); // 20%���2������һ��
u->addSkill(CThumpPas::create(20, CExtraCoeff(5, 0), 0)); // 20%���5������һ��
u->addSkill(CAttackBuffMakerPas::create(100, pSkill->getKey(), 1)); // 100%����10%Ŀ�깥��/�ƶ��ٶȣ��ɵ���
u->addSkill(CDoubleAttackPas::create(80)); // 50%������������

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
u->setWeaponType(CGameUnit::kWTInstant); // ��������Ϊ�����������ڽ�ս���ӳ�
oAv.setZero();
oAv.setAttack(eAttackTypeFlame, 5);
u->setBaseAttackValue(oAv);
u->setArmorType(eArmorTypeBiology);
u->setArmorValue(5);
u->setPosition(ccp(450 + rand() % 100, 250 + rand() % 100));
u->setHalfOfWidth(7);
u->setHalfOfHeight(10);
u->setMaxHp(100);
u->setTemplateProjectile(g_pProj); // ��λ����Ͷ����
//u->setProjectileMoveSpeed(200); // Ͷ�����ƶ��ٶ�
u->setProjectileScale(u->getSprite()->getScale()); // Ͷ��������
u->setProjectileMaxOffsetY(20); // Ͷ�����������˶���ߵ�Yƫ��
g_iDemoTarget2 = u->getKey();
u->setForceByIndex(2);
u->setAlly((1 << 1) | (1 << 2));
u->setHostilityRange(200);

u->addSkill(CStatusShowPas::create()); // Ѫ������
u->addSkill(CHpChangePas::create(0.1, 0.2, true, -1)); // ÿ0.1��ָ�0.2%HP
u->addSkill(CVampirePas::create(0.2)); // ����20%��Ѫ
u->addSkill(CThumpPas::create(20, CExtraCoeff(20, 0), 0)); // 20%���20������һ��
u->addSkill(CAttackBuffMakerPas::create(100, pSkill->getKey(), 1)); // 100%����10%Ŀ�깥��/�ƶ��ٶȣ��ɵ���
u->addSkill(CDoubleAttackPas::create(50)); // 50%������������
