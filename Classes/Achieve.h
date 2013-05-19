//
//  Achieve.h
//  GameDemo
//
//  Created by qiuhelin on 13-5-11.
//
//

#ifndef GameDemo_Achieve_h
#define GameDemo_Achieve_h
#include "Displayer.h"
//成就完成逻辑类型
enum ACHIEVE_LOGIC_TYPE
{
    LOGIC_TYPE_NO = 0, //无此类型成就
    LOGIC_TYPE_OR,   //即时成就
    LOGIC_TYPE_MIX,    //累积成就
    LOGIC_TYPE_ORDER,  //累积成就，有先后顺序
    LOGIC_TYPE_SEQ,    //累积成就，严格的序列方式
};
//成就事件
class CAchieveEvent : public CDisplayObj
{
public:
    CAchieveEvent()
    :m_ulID(0)
    ,m_iEvent(0)
    ,m_bFinsh(false){}
    CAchieveEvent(unsigned long ulID, int iEvent)
    :m_ulID(ulID)
    ,m_iEvent(iEvent){}
    
    virtual void display(ostream& os, int level = 0) const;
    unsigned long m_ulID;
    int m_iEvent;
    bool m_bFinsh;
};

//成就bean
class CAchieve : public CDisplayObj
{
public:
    CAchieve();
    virtual ~CAchieve();
    bool init(const string title, const map<string, string>& entries);
    void fromEntry(const string title, const map<string, string>& entries);
    virtual void display(ostream& os, int level = 0) const;
public:
    string m_sType; //成就类型
    string m_sName;//成就名称
    string m_sDesc;//成就描述
    string m_sPic;//图片名称
    short m_wLogicType;//达到成就的逻辑类型
    vector<CAchieveEvent> m_vecEvent; //事件数组
    map<int, float> m_mapReward;//奖励
    bool m_bReach;
};
typedef map<string, map<string, string>* > EntryContent;
//ini文件解析
class CEntryFile
{
public:
    CEntryFile();
    ~CEntryFile();
public:
    bool init(const char* pFileName);
    
public:
    string getFileName(){ return m_sFileName; }
    
protected:
    string m_sFileName;
public:
    EntryContent m_entryContent;
};
//成就处理基类
class CAchieveHandle
{
public:
    CAchieveHandle(CAchieveHandle* pNext=0, short wType=0);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
    virtual bool hasHandle(CAchieve* pAchieve);
    void setHandle(CAchieveHandle* pNext);
    void setType(short wType);
private:
    CAchieveHandle* m_pNextHandle;
    short m_wType;
};
class CNowAchieveHandle : public CAchieveHandle
{
public:
    CNowAchieveHandle(CAchieveHandle* pNext=0, short wType=LOGIC_TYPE_OR);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
};
class CMIXAchieveHandle : public CAchieveHandle
{
public:
    CMIXAchieveHandle(CAchieveHandle* pNext=0, short wType=LOGIC_TYPE_MIX);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
};
class COrderAchieveHandle : public CAchieveHandle
{
public:
    COrderAchieveHandle(CAchieveHandle* pNext=0, short wType=LOGIC_TYPE_OR);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
};
class CSeqAchieveHandle : public CAchieveHandle
{
public:
    CSeqAchieveHandle(CAchieveHandle* pNext=0, short wType=LOGIC_TYPE_SEQ);
    virtual bool handle(CAchieve*  pAchieve, const CAchieveEvent& event);
};
//成就管理器
class CAchieveManager : public CCObject
{
public:
    CAchieveManager():m_ulID(0){}
public:
    virtual ~CAchieveManager();
    
    bool init();
	CREATE_FUNC(CAchieveManager);
	static CAchieveManager* sharedAchieveManager();
    CAchieve* push(int iEvent);
public:
    vector<CAchieve*> m_vecAchieve;
private:
    unsigned long m_ulID;
    CAchieveHandle m_oContiner;
    CNowAchieveHandle m_oNow;
    CMIXAchieveHandle m_oMix;
    COrderAchieveHandle m_oOrder;
    CSeqAchieveHandle m_oSeq;
};

#endif
