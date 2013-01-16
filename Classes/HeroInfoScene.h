class CHeroInfoLayer;

class CHeroInfoScene:public CCScene
{
public:
    virtual bool init()
    {
        CCScene::init();
        m_heroInfoLayer = CHeroInfoLayer::create();
        addChild(m_heroInfoLayer);
        return true;

    }
    CREATE_FUNC(CHeroInfoScene);
public:
    CHeroInfoLayer *m_heroInfoLayer;
};

class CHeroInfoLayer:public CCLayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(CHeroInfoLayer);
};