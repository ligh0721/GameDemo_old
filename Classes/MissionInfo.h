#pragma once

class SoldierAdvanceInfo;
class CSimpleGameUnit;
class MissionInfo;

class MissionInfo
{
public:
    virtual void init(CCArray* soldierAdvanceInfoArray);
    static MissionInfo* create(CCArray* soldierAdanveInfoArray);
    CCPoint getNextMoveToPoint(int appearIndex,int nextPosition);
    SoldierAdvanceInfo* getAdvanceInfoByIndex(int index);
    static MissionInfo* loadNextMissionInfo();
    int getAdvanceInfoNumber();
	static MissionInfo* getComingMissionInfo(void);
	void setAdvanceInfoArray(CCArray* advanceInfoArray);
	void addSingleAdvanceInfoToArray(SoldierAdvanceInfo* singleAdvanceInfo);
    void setHeroChosen(int i,int j,int k);
    int getFirstHero();
    int getSecondHero();
    int getThirdHero();
protected:
    int heroChosen[3];
	CCArray* m_advanceInfoArray;
	int m_iAdvanceInfoNumber;

};


class SoldierAdvanceInfo:public CCObject
{
public:
    virtual void init(CCPoint soldierAppearPoint,CCArray* soldierPathPointArray);
    static SoldierAdvanceInfo* create(CCPoint soldierAppearPoint,CCArray* soldierPathPointArray);
    CCPoint getPointAtPosition(int position);
    CCPoint getAppearPoint();
    virtual CCObject* copyWithZone(CCZone* pZone);
    int m_waveOfSoldier[2][4];
protected:
    int m_iPathPointNumber;
    CCPoint m_soldierAppearPoint;
    CCArray* m_soldierPathPointArray;
};


// typedef struct  
// {
// 	CUnitPath path;
// 	CCPoint appearPoint;
// 	int waveOfSoldierInfo[10][10];
// 	//other types of soldier appear info
// }appearPointInfo;
// 
// 
// class CMissionInfo:public CCObject
// {
// public:
// 	virtual bool initWithAppearPointNumber(const int& pointNumber)
// 	{
// 		m_iAppearPointNumber=pointNumber;
// 		m_sAppearInfo=new appearPointInfo[m_iAppearPointNumber];
// 		return true;
// 	}
// 	CREATEWITH_FUNC_PARAM(AppearPointNumber,CMissionInfo,(const int& pointNumber),pointNumber);
// 
// public:
// 	void addAppearPointInfo(appearPointInfo pointInfo)
// 	{
// 		int i=0;
// 		while ((i<m_iAppearPointNumber)&&(&m_sAppearInfo[i]!=NULL))
// 		{
// 			i++;
// 		}
// 		if (i<m_iAppearPointNumber)
// 		{
// 			m_sAppearInfo[i]=pointInfo;
// 		}
// 		else
// 		{
// 			return;
// 		}
// 	}
// 	appearPointInfo getAppearPointInfoByIndex(int pointIndex)
// 	{
// 		return m_sAppearInfo[pointIndex];
// 	}
// 
// public:
// 	appearPointInfo* m_sAppearInfo;
// 	int m_iAppearPointNumber;
// };

class CUnitRush
{
public:
    struct RUSH_UNITS
    {
        RUSH_UNITS(int iIndex_, int iCount_, int fDelay_) : iIndex(iIndex_), iCount(iCount_), fDelay(fDelay_) {}
        int iIndex;
        int iCount;
        float fDelay;
    };
    typedef vector<RUSH_UNITS> VEC_RUSH_UNITS;

public:
    CUnitRush();
    virtual bool init(int iPathIndex);

    int rush(float fDt);

    void addUnit(int iUnitIndex, int iCount, float fDelay);
    void setPath(int iPathIndex);

public:
    VEC_RUSH_UNITS m_vecRushUnits;
    int m_iRushPos;
    int m_iRushVecPos;
    float m_fPass;
    int m_iPathIndex;
};

class CGameMission
{
public:
    typedef vector<CUnitRush> VEC_ROUND_RUSHS;
    typedef vector<VEC_ROUND_RUSHS> VEC_ROUNDS;

    enum RUSH_RES
    {
        kInvalidIndex = -4,
        kNoRound = -3,
        kNoUnit = -2,
        kWaiting = -1
    };

public:
    CGameMission();

    int curRound() const;
    void nextRound();
    int rushCount() const;
    int rush(int iRushIndex, float fDt);
    CUnitPath* pathOfRush(int iRushIndex);

    int addPath(CUnitPath* pPath); // return path index
    int addNewRound(); // return round index
    void addRush(int iRound, const CUnitRush& roRush);

public:
    VEC_ROUNDS m_vecRounds;
    int m_iRoundPos;
    CCArray m_oArrPaths;
    
};

