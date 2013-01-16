#pragma once

class CCPropUpdateLayer;

class CCPropUpdateScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCPropUpdateScene);

public:
    CCPropUpdateLayer* m_pPropUpdateLayer;
};


class CCPropUpdateLayer : public CCLayer
{
public:
    virtual bool init();
    CREATE_FUNC(CCPropUpdateLayer);

public:
	CCButtonPanel* m_pPropPanel;
	CCLabelAtlas* m_pHpLable;
};

