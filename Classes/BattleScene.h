#pragma once

// �����ñ༭���滻���Battle�滻Ϊָ��������

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
