#pragma once


class COrgUnitInfo
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
};

extern COrgUnitInfo g_oOrgUnitInfo;

