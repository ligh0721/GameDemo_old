#pragma once

#include "GameDisplay.h"

class CCSkillUpdateLayer;

class CCSkillUpdateScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCSkillUpdateScene);

public:
    CCSkillUpdateLayer* m_pSkillUpdateLayer;
};


class CCSkillDataSource : public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    CCSkillDataSource();

    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);

    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);

    static CCSkillDataSource* shareSkillDataSource();

public:
    static const int CONST_SEL_tAG = 1689727548;
    CCSprite m_oSel;

};

class CCSkillUpdateLayer : public CCLayer
{
public:
    CCSkillUpdateLayer();

    virtual bool init();
    CREATE_FUNC(CCSkillUpdateLayer);

    void onSkillImgClick(CCNode* pNode);
    void onSkillUpdateCancel(CCNode* pNode);

public:
    CCButtonPanel m_oSp;
    CCSprite m_oImage;
    CCLabelTTF m_oName;
    CCLabelTTF m_oDescribe;
    CCLabelTTF m_oInfo;
    CCMenuItemImage m_oUpdateBtn;
    CCMenu m_oCtrl;
    
};
