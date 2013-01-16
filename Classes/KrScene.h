#pragma once

// 可以用编辑器替换命令将Battle替换为指定的名字

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
	kAttack = 0x101,  //进攻角色
	kDefense,		  //防守角色

};
typedef struct  
{
	string oName; //单位名称
	CCPoint oPos; //单位初始化位置(负数代表percent,正数代表像素值)
	string oAnim; //单位初始动画

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