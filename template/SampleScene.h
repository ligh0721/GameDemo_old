#pragma once

// �����ñ༭���滻���Sample�滻Ϊָ��������

class CCSampleSceneLayer;

class CCSampleScene : public CCScene
{
public:
    virtual bool init();
    CREATE_FUNC(CCSampleScene);

public:
    CCSampleSceneLayer* m_pSampleSceneLayer;
};

class CCSampleSceneLayer : public CCLayerColor
{
public:
    CCSampleSceneLayer();

    virtual bool init();
    CREATE_FUNC(CCSampleSceneLayer);
    void onBtnSampleClick(CCObject* pObject);

public:
    CCMenu m_oMenu;
    CCMenuItemFont m_oBtnSample;
};
