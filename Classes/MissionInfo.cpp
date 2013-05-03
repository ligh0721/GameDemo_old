#include "CommInc.h"
#include "Unit.h"
#include "MissionInfo.h"
#include "ChildOfGameUnit.h"




SoldierAdvanceInfo* SoldierAdvanceInfo::create( CCPoint soldierAppearPoint,CCArray* soldierPathPointArray )
{
    SoldierAdvanceInfo* retSoldierAdvanceInfo=new SoldierAdvanceInfo();
    retSoldierAdvanceInfo->init(soldierAppearPoint,soldierPathPointArray);
    return retSoldierAdvanceInfo;
}

void SoldierAdvanceInfo::init( CCPoint soldierAppearPoint,CCArray* soldierPathPointArray )
{
    m_soldierAppearPoint=soldierAppearPoint;
    m_iPathPointNumber=soldierPathPointArray->count();
    m_soldierPathPointArray=new CCArray(m_iPathPointNumber);
    m_soldierPathPointArray->initWithArray(soldierPathPointArray);
    for(int i=0;i<2;i++)
    {
        for (int j=0;j<4;j++)
        {
            m_waveOfSoldier[i][j]=0;
        }
    }
}

CCPoint SoldierAdvanceInfo::getPointAtPosition( int position )
{
    static CCPoint oZero = CCPointZero;
    assert((position>=0));
	CCPoint* retPoint=&oZero;
    if (position>=m_iPathPointNumber)
    {
        position=m_iPathPointNumber-1;
		return (*retPoint);
    }
	else
	{
		retPoint=(CCPoint*)(m_soldierPathPointArray->objectAtIndex(position));
		//retPoint->x=retPoint->x+rand()%50;
		//retPoint->y=retPoint->y+rand()%50;
		return (*retPoint);
	}
}

CCPoint SoldierAdvanceInfo::getAppearPoint()
{
    return m_soldierAppearPoint;
}

CCObject* SoldierAdvanceInfo::copyWithZone( CCZone* pZone )
{
    SoldierAdvanceInfo* retInfo=new SoldierAdvanceInfo();
    retInfo->init(this->m_soldierAppearPoint,this->m_soldierPathPointArray);
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<4;j++)
        {
            retInfo->m_waveOfSoldier[i][j]=this->m_waveOfSoldier[i][j];
        }
    }
    return retInfo;
}

void MissionInfo::init( CCArray* soldierAdvanceInfoArray )
{
    m_iAdvanceInfoNumber=soldierAdvanceInfoArray->count();
    m_advanceInfoArray=new CCArray();
    m_advanceInfoArray->initWithArray(soldierAdvanceInfoArray);
    heroChosen[0]=0;
    heroChosen[1]=0;
    heroChosen[2]=0;
}

MissionInfo* MissionInfo::create( CCArray* soldierAdanveInfoArray )
{
    MissionInfo* pMissionInfo=new MissionInfo();
    pMissionInfo->init(soldierAdanveInfoArray);
    return pMissionInfo;
}

CCPoint MissionInfo::getNextMoveToPoint(int appearIndex,int nextPosition)
{
    SoldierAdvanceInfo* thisAdvanceInfo=getAdvanceInfoByIndex(appearIndex);
    CCPoint nextMoveToPoint=thisAdvanceInfo->getPointAtPosition(nextPosition);
    return nextMoveToPoint;
}

SoldierAdvanceInfo* MissionInfo::getAdvanceInfoByIndex( int index )
{
    assert(index<m_iAdvanceInfoNumber);
    SoldierAdvanceInfo* pSoldierAdvanceInfo=(SoldierAdvanceInfo*)m_advanceInfoArray->objectAtIndex(index);
    return pSoldierAdvanceInfo;
}

MissionInfo* MissionInfo::loadNextMissionInfo()
{
    MissionInfo* m_comingMissionInfo=new MissionInfo();
    CCPoint *point1=new CCPoint(0,800);
    CCPoint *point2=new CCPoint(500,700);
    CCPoint *point3=new CCPoint(1000,700);
    CCPoint *point4=new CCPoint(2000,700);
    CCArray* soldierPathPointArray=new CCArray(4);
    CCArray* soldierPathPointArray2=new CCArray(4);
    soldierPathPointArray->addObject(point1);
    soldierPathPointArray->addObject(point2);
    soldierPathPointArray->addObject(point3);
    soldierPathPointArray->addObject(point4);

    CCPoint *point11=new CCPoint(2000,800);
    CCPoint *point12=new CCPoint(1500,700);
    CCPoint *point13=new CCPoint(1000,700);
    CCPoint *point14=new CCPoint(0,800);
    soldierPathPointArray2->addObject(point11);
    soldierPathPointArray2->addObject(point12);
    soldierPathPointArray2->addObject(point13);
    soldierPathPointArray2->addObject(point14);

    CCArray* soldierAdvanceInfoArray=new CCArray(2);
    SoldierAdvanceInfo* advanceInfoExp=SoldierAdvanceInfo::create(ccp(0,400),soldierPathPointArray);
    SoldierAdvanceInfo* advanceInfoExp2=SoldierAdvanceInfo::create(ccp(0,100),soldierPathPointArray2);
    advanceInfoExp->m_waveOfSoldier[1][0]=5;
    advanceInfoExp->m_waveOfSoldier[1][1]=2;
    advanceInfoExp2->m_waveOfSoldier[1][0]=3;
    advanceInfoExp2->m_waveOfSoldier[1][1]=4;
    soldierAdvanceInfoArray->addObject(advanceInfoExp);
    soldierAdvanceInfoArray->addObject(advanceInfoExp2);
    m_comingMissionInfo->init(soldierAdvanceInfoArray);
    return m_comingMissionInfo;
}

int MissionInfo::getAdvanceInfoNumber()
{
    return m_iAdvanceInfoNumber;
}

MissionInfo* MissionInfo::getComingMissionInfo(void)
{
	static MissionInfo* nextMissionInfo;
	if (nextMissionInfo!=NULL)
	{
		return nextMissionInfo;
	}
	else
	{
		nextMissionInfo=new MissionInfo();
		return nextMissionInfo;
	}
}

void MissionInfo::setAdvanceInfoArray( CCArray* advanceInfoArray )
{
	m_iAdvanceInfoNumber=advanceInfoArray->count();
	m_advanceInfoArray=new CCArray();
	m_advanceInfoArray->initWithArray(advanceInfoArray);
}

void MissionInfo::addSingleAdvanceInfoToArray( SoldierAdvanceInfo* singleAdvanceInfo )
{
	m_advanceInfoArray->addObject(singleAdvanceInfo);
	m_iAdvanceInfoNumber++;
}

void MissionInfo::setHeroChosen( int i,int j,int k )
{
    heroChosen[0]=i;
    heroChosen[1]=j;
    heroChosen[2]=k;
}

int MissionInfo::getFirstHero()
{
    return heroChosen[0];
}

int MissionInfo::getSecondHero()
{
    return heroChosen[1];
}

int MissionInfo::getThirdHero()
{
    return heroChosen[2];
}

CUnitRush::CUnitRush()
    : m_iRushPos(0)
    , m_iRushVecPos(0)
    , m_fPass(0)
    , m_iPathIndex(0)
{
}

bool CUnitRush::init( int iPathIndex )
{
    m_iRushPos = 0;
    m_iRushVecPos = 0;
    m_fPass = 0;
    m_vecRushUnits.clear();
    setPath(iPathIndex);
    return true;
}

int CUnitRush::rush( float fDt )
{
    int n = m_vecRushUnits.size();
    if (m_iRushVecPos >= n)
    {
        return CGameMission::kNoUnit; // no unit to rush
    }
    m_fPass += fDt;
    RUSH_UNITS& rRush = m_vecRushUnits[m_iRushVecPos];
    if (m_fPass > rRush.fDelay)
    {
        m_fPass -= rRush.fDelay;
        m_iRushPos = (m_iRushPos + 1) % rRush.iCount;
        if (m_iRushPos == 0)
        {
            ++m_iRushVecPos;
        }
        return rRush.iIndex;
    }
    return CGameMission::kWaiting; // waiting delay
}

void CUnitRush::addUnit( int iUnitIndex, int iCount, float fDelay )
{
    m_vecRushUnits.push_back(RUSH_UNITS(iUnitIndex, iCount, fDelay));
}

void CUnitRush::setPath( int iPathIndex )
{
    m_iPathIndex = iPathIndex;
}

CGameMission::CGameMission()
    : m_iRoundPos(0)
{
    m_oArrPaths.init();
}

bool CGameMission::init()
{
    m_iRoundPos = 0;
    m_vecRounds.clear();
    m_oArrPaths.init();
    return true;
}

int CGameMission::curRound() const
{
    if (m_iRoundPos < (int)m_vecRounds.size())
    {
        return m_iRoundPos;
    }
    return -1;
}

void CGameMission::nextRound()
{
    if (m_iRoundPos < (int)m_vecRounds.size())
    {
        ++m_iRoundPos;
    }
}

int CGameMission::rushCount() const
{
    int iRound = curRound();
    if (iRound < 0)
    {
        return -1;
    }

    return m_vecRounds[iRound].size();
}

int CGameMission::rush( int iRushIndex, float fDt )
{
    int iRound = curRound();
    if (iRound < 0)
    {
        return kNoRound;
    }
    if (iRushIndex < (int)m_vecRounds[iRound].size())
    {
        return m_vecRounds[iRound][iRushIndex].rush(fDt);
    }
    return kInvalidIndex;
}

CUnitPath* CGameMission::pathOfRush( int iRushIndex )
{
    int iRound = curRound();
    if (iRound < 0)
    {
        return NULL;
    }
    
    if (iRushIndex < (int)m_vecRounds[iRound].size())
    {
        CUnitRush& rRush = m_vecRounds[iRound][iRushIndex];
        if (rRush.m_iPathIndex < (int)m_oArrPaths.count())
        {
            return dynamic_cast<CUnitPath*>(m_oArrPaths.objectAtIndex(rRush.m_iPathIndex));
        }
    }

    return NULL;
}

void CGameMission::setUnitInfoPatch( const char* pUipName )
{
    m_oUipm.initWithFile(pUipName);
}

int CGameMission::addPath( CUnitPath* pPath )
{
    m_oArrPaths.addObject(pPath);
    return m_oArrPaths.count() - 1;
}

int CGameMission::addNewRound()
{
    m_vecRounds.push_back(VEC_ROUND_RUSHS());
    return m_vecRounds.size() - 1;
}

void CGameMission::addRush( int iRound, const CUnitRush& roRush )
{
    if (iRound < (int)m_vecRounds.size())
    {
        m_vecRounds[iRound].push_back(roRush);
    }
}

bool CDemoMission::init()
{
    
    return true;
}

CGameMission* CDemoMission::mission( int iMissIndex )
{
    return NULL;
}

CGameMission* CDemoMission::mission01()
{
    static CGameMission oMission;
    static const int CONST_LEVEL = 1;
    static const int CONST_MAX_PATH = 4;
    enum
    {
        kMalik,
        kMagnus,
        kJt
    };

    CUnitPath* pPath;
    int aiPath[4];
    CUnitRush oRush;
    int iRound;

    oMission.init();
    oMission.setUnitInfoPatch("levels/level01/Level.uip");

    // add paths
    for (int i = 0; i < CONST_MAX_PATH; ++i)
    {
        char sz[256];
        sprintf(sz, "levels/level%02d/LevelHD%02d.pth", CONST_LEVEL, i);
        pPath = CUnitPath::createWithFile(sz);
        aiPath[i] = oMission.addPath(pPath);
    }
    /*
    iRound = oMission.addNewRound();
    oRush.init(aiPath[rand() % CONST_MAX_PATH]);
    oRush.addUnit(kMatchstick, 1, 0);
    oRush.addUnit(kMatchstick, 3, 5);
    oMission.addRush(iRound, oRush);
    */
    // add a rush
    for (int i = 0; i < 1; ++i)
    {
        iRound = oMission.addNewRound();
        oRush.init(aiPath[rand() % CONST_MAX_PATH]);
        oRush.addUnit(kMalik, 1, 0);
        oRush.addUnit(kMalik, 1, 1);
        oRush.addUnit(kMalik, 1, 2);
        oRush.addUnit(kMagnus, 1, 3);
        oRush.addUnit(kMagnus, 1, 4);
        oRush.addUnit(kJt, 1, 6);
        oMission.addRush(iRound, oRush);

    }

    return &oMission;
}

CDemoMission g_oDemoMission;

