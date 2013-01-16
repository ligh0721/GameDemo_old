#include "CommInc.h"
#include "Unit.h"
#include "ChildOfGameUnit.h"
#include "MissionInfo.h"

void CSimpleGameUnit::onTick(float fDt)
{
    //随时判断动作改变
    //动作结束继续moveTo下个点
	CGameUnit::onTick(fDt);
    static MissionInfo* infoExp=MissionInfo::loadNextMissionInfo();
   // MissionInfo* infoExp=MissionInfo::getComingMissionInfo();
    if (this->isDoingNothing())
    {
        CCPoint destinationPoint=infoExp->getNextMoveToPoint(this->getAppearPointIndex(),this->getNextMoveToPosition()); 
        CCLog("destination(%f,%f)",destinationPoint.x,destinationPoint.y);

        if ((this->getPosition().x==destinationPoint.x)&&(this->getPosition().y==destinationPoint.y))
        {
			m_nextMoveToPosition++;
			destinationPoint=infoExp->getNextMoveToPoint(this->getAppearPointIndex(),this->getNextMoveToPosition());
        }
       if ((destinationPoint.x==0)&&(destinationPoint.y==0))
       {
		   CCLOG("MOVE TO END");
       }
	   else
	   {
		   //destinationPoint.x=destinationPoint.x+rand()%20;
		   //destinationPoint.y=destinationPoint.y+rand()%20;
		   this->moveTo(destinationPoint);
	   }
    }
}

bool CSimpleGameUnit::initWithName(const char* pUnit, const CCPoint& roAnchor)
{
    if(!CGameUnit::initWithName(pUnit, roAnchor))
    {
        return false;
    }
    firstTick=true;
    m_nextMoveToPosition=0;
    m_appearPointIndex=0;
    return true;
}

void CSimpleGameUnit::setAppearPointIndex( int index )
{
    m_appearPointIndex=index;
}

void CSimpleGameUnit::setNextMoveToPosition( int position )
{
    m_nextMoveToPosition=position;
}

int CSimpleGameUnit::getAppearPointIndex( void )
{
    return m_appearPointIndex;
}

int CSimpleGameUnit::getNextMoveToPosition( void )
{
    return m_nextMoveToPosition;
}
