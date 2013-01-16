class CSimpleGameUnit:public CGameUnit
{
    // 
public:
    virtual bool initWithName(const char* pUnit, const CCPoint& roAnchor = ccp(0.5f, 0.5f));
    //CREATEWITH_FUNC_PARAM(Name, CSimpleGameUnit, (const char* pUnit), pUnit);
    CREATEWITH_FUNC_PARAM(Name, CSimpleGameUnit, (const char* pUnit, const CCPoint& roAnchor = ccp(0.5f, 0.5f)), pUnit, roAnchor);
    virtual void onTick(float fDt);
    void setAppearPointIndex(int index);
    int getAppearPointIndex(void);
    void setNextMoveToPosition(int position);
    int getNextMoveToPosition(void);
protected:
    bool firstTick;
    int m_nextMoveToPosition;
    int m_appearPointIndex;
};



class CSimpleHeroUnit:public CGameUnit
{
public:
    virtual bool initWithName(const char* pUnit,const CCPoint& roAnchor = ccp(0.5f,0.5f));
    CREATEWITH_FUNC_PARAM(Name,CSimpleHeroUnit,(const char* pUnit,const CCPoint& roAnchor= ccp(0.5f,0.5f)),pUnit,roAnchor);

protected:


};
