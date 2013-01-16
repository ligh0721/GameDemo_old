#ifndef __GAMECHARPTERUI_H_
#define __GAMECHARPTERUI_H_

#include "GameDisplay.h"

class CCGameChapterSceneLayer;

class CCGameChapterScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCGameChapterScene);

	virtual bool initWithRole(int iRole);
	CREATEWITH_FUNC_PARAM(Role, CCGameChapterScene, (int iRole), iRole);

public:
    CCGameChapterSceneLayer* m_pGameChapterSceneLayer;
};

class CCGameChapterSceneLayer : public CCLayerColor
{
public:
    CCGameChapterSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCGameChapterSceneLayer);

	virtual bool initWithRole(int iRole);
	CREATEWITH_FUNC_PARAM(Role, CCGameChapterSceneLayer, (int iRole), iRole);

    void onShiftRightClick(CCObject* pObject);
    void onShiftLeftClick(CCObject* pObject);
	void onChapterItemClick(CCNode* pNode);
    void onBackClick(CCObject* pObject);
public:
    CCMenu m_oMenu;
	CCButtonPanel m_oSp;

protected:
	int m_iRole;
};
#endif
