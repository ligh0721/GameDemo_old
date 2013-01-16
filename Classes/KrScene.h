#pragma once

// �����ñ༭���滻���Battle�滻Ϊָ��������

class CCUnitLayer;
enum TOWER_KINDS
{
	kTagArrowTower = 0x010001,
	kTagSoilderTower,
	kTagMagicTower,
	kTagGunTower
};
enum GAME_STATUES
{
	kTagStop = 0x1001,
	kTagStart,
	kTagWait
};
enum  
{
	kTagBackground = 0x0001

};

enum ROLE
{
	kAttack = 0x101,  //������ɫ
	kDefense,		  //���ؽ�ɫ

};
typedef struct  
{
	string oName; //��λ����
	CCPoint oPos; //��λ��ʼ��λ��(��������percent,������������ֵ)
	string oAnim; //��λ��ʼ����

}CUnitNodeData;

typedef struct
{
	string oName;
	vector<string> vecAnim;

}CUnitAniData;


class CCKrScene : public CCScene
{
public:
	virtual bool init();
	CREATE_FUNC(CCKrScene);

	virtual bool initWithParam(int iRole, int iLevel);
	CREATEWITH_FUNC_PARAM(Param, CCKrScene, (int iRole, int iLevel), iRole, iLevel);

public:
	CCKrSceneLayer* m_pBattleSceneLayer;
};