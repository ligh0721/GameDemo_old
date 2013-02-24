#pragma once


class COrgUnitInfo : public CCObject
{
public:
    enum PROJ_INDEX
    {
        kBall1,
        kBall2,
        kBall3,
        kLightning1,
        kLightning2
    };

    enum UNITINFO_INDEX
    {
        kMalik,
        kPaladin,
        kMagnus,
        kJt,
        kArcane,
        //kBertha,
        kTesla,
        kVeznan,
        kMatchstick
    };

public:
    COrgUnitInfo();
    virtual bool init();
    CREATE_FUNC_PARAM(COrgUnitInfo, ());
    static COrgUnitInfo* sharedOrgUnitInfo();
};


