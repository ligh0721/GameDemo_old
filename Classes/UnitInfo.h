#pragma once


class COrgUnitInfo : public CCObject
{
public:
    enum UNITINFO_INDEX
    {
        kMalik,
        kPaladin,
        kMagnus,
        kJt,
        kArcane,
        //kBertha,
        kTesla,
        kVeznan
    };

public:
    COrgUnitInfo();
    virtual bool init();
    CREATE_FUNC_PARAM(COrgUnitInfo, ());
    static COrgUnitInfo* sharedOrgUnitInfo();
};


