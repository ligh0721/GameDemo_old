#include "CommInc.h"
#include "HeroInfoScene.h"


bool CHeroInfoLayer::init()
{
    if (!CCLayerColor::initWithColor(ccc4(0,128,0,128)))
    {
        return false;
    }


    return true;
}
