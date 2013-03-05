#pragma once

// 可以用编辑器替换命令将Battle替换为指定的名字

class CCBattleSceneLayer;
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


class CCBattleScene : public CCScene
{
public:
    enum UNIT_KEY
    {
        kHeroUnit = 0
    };
    virtual bool init();
    CREATE_FUNC(CCBattleScene);

public:
    CCBattleSceneLayer* m_pBattleSceneLayer;
};

class CCBattleSceneLayer : public CCWinUnitLayer
{
public:
	
    CCBattleSceneLayer();
	virtual ~CCBattleSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCBattleSceneLayer);

	bool ccTouchBegan(CCTouch* touch, CCEvent* event);

    void onBtnBattleClick(CCObject* pObject);
	 
	void onBtnUpgradeClick(CCObject* pObject);

	void onBtnHomeClick(CCObject* pObject);
    
    void onBtnLightgingSkillClick(CCObject* pObject);

	
	virtual void onTick(float fDt);

	void showTower(CCObject* pObject);

	void buildTower(CCNode* pNode);

	void execBuildTower(CCNode* pNode);

	void afterBuildTower(CCObject* pObject);

	virtual void initTower(CCObject* pObject);

protected:

	bool loadNPCNodeData();

	bool loadNPCAniData();

	bool loadUnitRoute();

	bool loadUnitRes();

	bool loadTowerPos();

	CGameUnit* randomSoldiers(int iForce);

public:
	vector<CUnitNodeData*> m_vecNPCNodeData;
    vector<CUnitAniData*> m_vecNPCAniData;
	CCMenu m_oMenu;
    CCMenuItemFont m_oBtnBattle;  
	CCMenuItemFont m_oBtnUpGrade;
	CCMenuItemFont m_oBtnHome;
    CCMenuItemFont m_oBtnLightingSkill;
    CCProgressBar m_oHpBar;
    CCButtonPanel m_oSkillPanel;
    int m_iHeroUnitKey;
	//vector<vector<CCPoint>* > m_vecUnitRoute;
	CUnitPath m_oUnitPath;
    CUnitPath m_oReverseUnitPath;
	int m_iGameStatus;
	vector<CCPoint> m_vecTowerPos;
	CCMenu m_oTowerPosMenu;
	CCButtonPanel m_oTowerShowMenu;
	CCPoint m_oCurTowerPos;
	vector<CGameUnit*> m_vecCoalition;
	vector<CGameUnit*> m_vecTribe;
};
